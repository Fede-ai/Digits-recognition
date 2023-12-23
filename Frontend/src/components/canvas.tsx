import React, {useEffect, useState, FC} from "react";
import canvasStyle from './canvas.module.css';

interface Point {
  x: number;
  y: number;
}

interface canvasProps {
	setImg: (newImg: Uint8Array) => void;
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

	const startDrawing = (e: React.MouseEvent<HTMLCanvasElement, MouseEvent>) => {
    if (e.button === 0) lineColor = '#FFF';
		else if (e.button === 2) lineColor = '#000'
		else return;

    setIsDrawing(true);
    setPrevPoint({ x: e.nativeEvent.offsetX, y: e.nativeEvent.offsetY });
  };  
	const draw = (e: React.MouseEvent<HTMLCanvasElement, MouseEvent>) => {
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
  const endDrawing = () => {
    setIsDrawing(false);
    setPrevPoint(null);
  };

	const passData = () => {
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
			onContextMenu={(event)=>{event.preventDefault()}}

			onMouseDown={startDrawing}      
			onMouseMove={draw}	
      onMouseUp={endDrawing}
			onMouseLeave={endDrawing}

			//onTouchStart={}
			//onTouchMove={}
			//onTouchEnd={}
			//onTouchCancel={}
    	/>
			<div className={canvasStyle.buttons}>
				<button onClick={clearCanvas} className={canvasStyle.button}>Clear canvas</button>
				<button onClick={passData} className={canvasStyle.button}>Process data</button>				
			</div>
		</>
	)
}

export default Canvas;