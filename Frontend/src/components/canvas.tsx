import React, {useEffect, useState, FC} from "react";
import canvasStyle from './canvas.module.css';

interface Point {
  x: number;
  y: number;
}

interface canvasProps {
	sendData: (newImg: Uint8Array) => void;
}

var lineColor:string;

const Canvas:FC<canvasProps> = (props) => {
	const [width, setWidth] = useState(window.innerWidth);
  const [isDrawing, setIsDrawing] = useState(false);
  const [context, setContext] = useState<CanvasRenderingContext2D | null>(null);
  const [prevPoint, setPrevPoint] = useState<Point | null>(null);
	
	var canvasSide = 0.3 * width;

	useEffect(() => {	
		const canvas = document.getElementById('canvas') as HTMLCanvasElement;
		const ctx = canvas.getContext('2d');
		if (ctx) {		
			ctx.fillStyle = '#000';
			ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
      setContext(ctx);	
		}

		window.addEventListener('resize', () => {
			setWidth(window.innerWidth);
			if(context) context.fillRect(0, 0, context.canvas.width, context.canvas.height);
		});
	}, [context])

	const startClick = (e: React.MouseEvent<HTMLCanvasElement, MouseEvent>) => {
    if (e.button === 0) lineColor = '#FFF';
		else if (e.button === 2) lineColor = '#000'
		else return;

    setIsDrawing(true);
    setPrevPoint({ x: e.nativeEvent.offsetX, y: e.nativeEvent.offsetY });
  };  
	const moveClick = (e: React.MouseEvent<HTMLCanvasElement, MouseEvent>) => {
		if (!isDrawing || !context) return;

    const currentPoint: Point = { x: e.nativeEvent.offsetX, y: e.nativeEvent.offsetY };
		context.strokeStyle = lineColor;
		context.lineJoin = 'round';
		context.lineWidth = canvasSide / 14;
		if (lineColor === '#000')
			context.lineWidth = canvasSide / 5
    context.beginPath();
    context.moveTo(prevPoint?.x || 0, prevPoint?.y || 0);
    context.lineTo(currentPoint.x, currentPoint.y);
    context.closePath();
    context.stroke();
    setPrevPoint(currentPoint);
  };
  const endClick = () => {
    setIsDrawing(false);
    setPrevPoint(null);
  };

	const processAndSendData = () => {
		if (!context) return;
		const data = context.getImageData(0, 0, canvasSide, canvasSide).data;
		let essData: Uint8Array = new Uint8Array(data.length/4);
		for (let i = 0; i < data.length; i++) {
			if (i % 4 === 0)
				essData[i/4] = data[i];
		}
		props.sendData(essData);
	};
	const clearCanvas = () => {
		if(context) context.fillRect(0, 0, context.canvas.width, context.canvas.height);
	}

	return(
		<>
			<canvas
      id="canvas"
      width={canvasSide}
      height={canvasSide}
      className={canvasStyle.canvas}
			onContextMenu={(event) => {event.preventDefault()}}

			onMouseDown={startClick}      
			onMouseMove={moveClick}	
      onMouseUp={endClick}
			onMouseLeave={endClick}
    	/>
			<div className={canvasStyle.buttons}>
				<button onClick={clearCanvas} className={canvasStyle.button}>
					Clear canvas
				</button>
				<button onClick={processAndSendData} className={canvasStyle.button}>
					Process data
				</button>				
			</div>
		</>
	)
}

export default Canvas;