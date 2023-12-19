import React, {useEffect, useState, FC} from "react";
import canvasStyle from './canvas.module.css';

interface Point {
  x: number;
  y: number;
}

interface canvasProps {
	setImg: (newImg: Uint8Array) => void;
}

const Canvas:FC<canvasProps> = (props) => {
	const [height, setHeight] = useState(window.innerWidth);
  const [isDrawing, setIsDrawing] = useState(false);
  const [context, setContext] = useState<CanvasRenderingContext2D | null>(null);
  const [prevPoint, setPrevPoint] = useState<Point | null>(null);
	
	var canvasSide = 0.3 * height;

	useEffect(() => {	
		const canvas = document.getElementById('canvas') as HTMLCanvasElement;
		const ctx = canvas.getContext('2d');
		if (ctx) {		
			ctx.fillStyle = '#000';
			ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
      setContext(ctx);	
    }		
		
		window.addEventListener('resize', () => {
			setHeight(window.innerWidth);
			if(context) context.fillRect(0, 0, context.canvas.width, context.canvas.height);
		});
	}, [context])

	const startDrawing = (e: React.MouseEvent<HTMLCanvasElement, MouseEvent>) => {
    if (e.button !== 0) return; // Ignore non-left mouse button
    setIsDrawing(true);
    setPrevPoint({ x: e.nativeEvent.offsetX, y: e.nativeEvent.offsetY });
  };

  const endDrawing = () => {
    setIsDrawing(false);
    setPrevPoint(null);
  };

  const draw = (e: React.MouseEvent<HTMLCanvasElement, MouseEvent>) => {
		if (!isDrawing || !context) return;

    const currentPoint: Point = { x: e.nativeEvent.offsetX, y: e.nativeEvent.offsetY };

    context.strokeStyle = '#FFF	'; // Set stroke color
    context.lineJoin = 'round';
    context.lineWidth = canvasSide / 14;

    context.beginPath();
    context.moveTo(prevPoint?.x || 0, prevPoint?.y || 0);
    context.lineTo(currentPoint.x, currentPoint.y);
    context.closePath();
    context.stroke();
    setPrevPoint(currentPoint);
  };

	const outputData = () => {
		if (!context) return;
		const imageData = context.getImageData(0, 0, canvasSide, canvasSide);
		let data = imageData.data;
		let essData: Uint8Array = new Uint8Array(data.length/4);
		for (let i = 0; i < data.length; i++) {
			if (i % 4 === 0)
				essData[i/4] = data[i];
		}
		props.setImg(essData);
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
			onMouseDown={startDrawing}
      onMouseUp={endDrawing}
      onMouseMove={draw}	
			onMouseLeave={()=>{setIsDrawing(false)}}
    	/>
			<button onClick={outputData}>Log Pixels</button>
			<button onClick={clearCanvas}>Clear Canvas</button>
		</>
	)
}

export default Canvas;