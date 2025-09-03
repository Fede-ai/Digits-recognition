const url = new URL(window.location.href);
const ip = new URLSearchParams(url.search).get('ip');
let socket = new WebSocket('ws:/' + ip + ':9002');

const canvas = document.getElementById('canvas') as HTMLCanvasElement;
const clearButton = document.querySelector('.button') as HTMLButtonElement;
const ctx: CanvasRenderingContext2D = (() => {
	let possibleContext = canvas.getContext('2d');

	if (possibleContext)
		return possibleContext;
	else
		throw new Error('Failed to get 2d contex');
})();

canvas.width = window.innerWidth * 0.3;
canvas.height = window.innerWidth * 0.3;
window.addEventListener('resize', () => {
	canvas.width = window.innerWidth * 0.3;
	canvas.height = window.innerWidth * 0.3;
})

socket.addEventListener('message', (event) => {
	let data: string = event.data;
	let valuesStr = data.split(',');

	const values: [number, number][] = [];
	valuesStr.forEach((str, i) => {
		values.push([Number(str) * 100, i]);
	});
	values.sort((a, b) => a[0] - b[0]);
	values.forEach((v, i) => {
		let r = document.getElementById((9 - i).toString()) as HTMLDivElement;
		r.textContent = 'n° ' + v[1] + ': ' + v[0].toFixed(2) + '%'
	});
});
socket.addEventListener('close', (event) => {
	console.log('WebSocket connection closed:', event);
});

let lastSend = performance.now();
function sendImg () {
	const canvasWidth = ctx.canvas.width;
	const canvasHeight = ctx.canvas.height;
	let newImg: Uint8ClampedArray = ctx.getImageData(0, 0, canvasWidth, canvasHeight).data;

	// Check WebSocket state
	if (socket.readyState !== socket.OPEN) return;

	// Extract only the red channel
	let essData = new Uint8Array(newImg.length / 4);
	for (let i = 0; i < newImg.length; i += 4)
  	essData[i / 4] = newImg[i + 3];

	// Lower resolution to 28x28
	const ratio = Math.floor(canvasWidth / 28);
	const compData = new Uint8Array(28 * 28);

	for (let y = 0; y < 28; y++) {
  	for (let x = 0; x < 28; x++) {
    	let sum = 0;
    	for (let i = 0; i < ratio; i++) {
      	for (let j = 0; j < ratio; j++) {
        	const originalX = Math.floor(x * ratio) + i;
        	const originalY = Math.floor(y * ratio) + j;
        	const index = originalY * canvasWidth + originalX;
        	sum += essData[index];
      	}
    	}
    	const average = sum / (ratio * ratio);
    	compData[y * 28 + x] = Math.min(Math.round(average), 255);
  	}
	}

	// Create a packet string
	let packet = compData.join(',');
  socket.send(packet);
};

// Variables to track the drawing state
let isDrawing = false;
let lastX = 0;
let lastY = 0;

// Function to start drawing
const startDrawing = (event: MouseEvent | TouchEvent): void => {
  isDrawing = true;
  const { x, y } = getEventPosition(event);
  lastX = x;
  lastY = y;
};
// Function to draw on the canvas
const draw = (event: MouseEvent | TouchEvent): void => {
  if (!isDrawing) return;

  const { x, y } = getEventPosition(event);

	ctx.strokeStyle = '#000';
	ctx.lineJoin = 'round';
	ctx.lineWidth = canvas.width / 10;
	ctx.beginPath();
	ctx.moveTo(lastX - canvas.width / 20, lastY - canvas.width / 20);
	ctx.lineTo(x - canvas.width / 20, y - canvas.width / 20);
	ctx.closePath();
	ctx.stroke();

  lastX = x;
  lastY = y;

	if (performance.now() - lastSend > 200) {
		sendImg();
		lastSend = performance.now();
	}
};
// Function to stop drawing
const stopDrawing = (): void => {
  isDrawing = false;
};
// Utility function to get mouse or touch position
const getEventPosition = (event: MouseEvent | TouchEvent): { x: number; y: number } => {
  const rect = canvas.getBoundingClientRect();
  let x: number, y: number;

  if (event instanceof MouseEvent) {
    x = event.clientX - rect.left;
    y = event.clientY - rect.top;
  } else {
    const touch = event.touches[0];
    x = touch.clientX - rect.left;
    y = touch.clientY - rect.top;
  }

  return { x, y };
};

// Add event listeners for mouse and touch input
canvas.addEventListener('mousedown', (event) => {
	if (event.button == 0)
		startDrawing(event);
});
canvas.addEventListener('mousemove', draw);
canvas.addEventListener('mouseup', stopDrawing);
canvas.addEventListener('mouseout', stopDrawing);
canvas.addEventListener('touchstart', (event) => {
  event.preventDefault(); // Prevent scrolling on touch devices
  startDrawing(event);
});
canvas.addEventListener('touchmove', (event) => {
  event.preventDefault(); // Prevent scrolling on touch devices
  draw(event);
});
canvas.addEventListener('touchend', stopDrawing);
canvas.addEventListener('touchcancel', stopDrawing);

// Add event listener for the clear button
clearButton.addEventListener('click', () => {
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	sendImg();
});