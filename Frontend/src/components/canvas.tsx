import React, {useEffect, useState, FC} from "react";
import canvasStyle from './canvas.module.css';

interface Point {
  x: number;
  y: number;
}

interface canvasProps {
	passImg: (newImg: Uint8ClampedArray) => void;
}

const Canvas:FC<canvasProps> = ({passImg}) => {
	const [color, setColor] = useState('#FFF');
	const [width, setWidth] = useState(window.innerWidth);
  const [isDrawing, setIsDrawing] = useState(false);
  const [context, setContext] = useState<CanvasRenderingContext2D | null>(null);
  const [prevPoint, setPrevPoint] = useState<Point | null>(null);
	const [needSend, setNeedSend] = useState(true);
	const canvasSide = 0.3 * width;

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
			if (context) context.fillRect(0, 0, context.canvas.width, context.canvas.height);
		});
	}, [context])

	useEffect(() => {
		const sendPeriodically = setInterval(() => {
			if (!context || !needSend) return;
			passImg(context.getImageData(0, 0, context.canvas.width, context.canvas.height).data);
			setNeedSend(false)	
		}, 200);

		return () => clearInterval(sendPeriodically);
	}, [context, needSend, passImg]);

	const startClick = (e: React.MouseEvent<HTMLCanvasElement, MouseEvent>) => {
    if (e.button === 0) setColor('#FFF');
		else if (e.button === 2) setColor('#000');
		else return;

    setIsDrawing(true);
    setPrevPoint({ x: e.nativeEvent.offsetX, y: e.nativeEvent.offsetY });
  };  
	const moveClick = (e: React.MouseEvent<HTMLCanvasElement, MouseEvent>) => {	
		if (!isDrawing || !context) return;

    const currentPoint: Point = { x: e.nativeEvent.offsetX, y: e.nativeEvent.offsetY };
		context.strokeStyle = color;
		context.lineJoin = 'round';
		context.lineWidth = canvasSide / 14;
		if (color === '#000')
			context.lineWidth = canvasSide / 5;
    context.beginPath();
    context.moveTo(prevPoint?.x || 0, prevPoint?.y || 0);
    context.lineTo(currentPoint.x, currentPoint.y);
    context.closePath();
    context.stroke();
    setPrevPoint(currentPoint);
		setNeedSend(true);
  };	
  const endClick = () => {
    setIsDrawing(false);
    setPrevPoint(null);
  };
	const clearCanvas = () => {
		if(!context) return;
		context.fillRect(0, 0, context.canvas.width, context.canvas.height);
		setNeedSend(true);
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
			</div>
		</>
	)
}

export default Canvas;