import React, {useEffect, useState, FC} from "react";

interface Point {
  x: number;
  y: number;
}

const Canvas:FC = (props) => {
	const canvasWidth = 400;
  const canvasHeight = 400;

  const [isDrawing, setIsDrawing] = useState(false);
  const [context, setContext] = useState<CanvasRenderingContext2D | null>(null);
  const [prevPoint, setPrevPoint] = useState<Point | null>(null);

	useEffect(() => {
		const canvas = document.getElementById('canvas') as HTMLCanvasElement;;
		const ctx = canvas?.getContext('2d');
		if (ctx) {
      setContext(ctx);
    }
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

    context.strokeStyle = '#000'; // Set stroke color
    context.lineJoin = 'round';
    context.lineWidth = 30;

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
      width={canvasWidth}
      height={canvasHeight}
      style={{ border: '1px solid #000' }}
			onMouseDown={startDrawing}
      onMouseUp={endDrawing}
      onMouseMove={draw}	
			onMouseLeave={()=>{setIsDrawing(false)}}
    	/>
		</>
	)
}

export default Canvas;