import React, {useEffect, useState, FC} from "react";
import canvasStyle from './canvas.module.css';

interface Point {
  x: number;
  y: number;
}

const Canvas:FC = (props) => {
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
			const ctx = canvas.getContext('2d');
			if(ctx) ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
		});
	}, [])

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
		</>
	)
}

export default Canvas;