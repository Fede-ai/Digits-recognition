import React, {useEffect, useRef} from 'react';
import Canvas from './components/canvas';
import './App.css';

function App() {
	const socket = useRef(new WebSocket('ws://192.168.1.191:9002'));

	useEffect(() => {
    socket.current.addEventListener('open', (event) => {
      console.log('WebSocket connection opened:', event);
    });
    socket.current.addEventListener('message', (event) => {
      console.log('WebSocket message received:', event.data);
    });
    socket.current.addEventListener('close', (event) => {
      console.log('WebSocket connection closed:', event);
    });
    socket.current.addEventListener('error', (event) => {
      console.error('WebSocket error:', event);
    });
	}, [socket]);
	
	const handleImgChage = (newImg: Uint8Array) => {
		socket.current.send(newImg);
	};

  return (
    <>
			<Canvas setImg={handleImgChage}/>
		</>
  );
}

export default App;
