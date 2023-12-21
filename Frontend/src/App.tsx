import React, {useEffect} from 'react';
import Canvas from './components/canvas';
import './App.css';

var socket:WebSocket = new WebSocket('ws://192.168.10.25:9002');	
var connected:boolean = false;
function App() {	
	
	useEffect(() => {
    socket.addEventListener('open', (event) => {
      console.log('WebSocket connection opened:', event);
    });
    socket.addEventListener('message', (event) => {
      console.log('WebSocket message received:', event.data);
    });
    socket.addEventListener('close', (event) => {
      console.log('WebSocket connection closed:', event);
    });
    socket.addEventListener('error', (event) => {
      console.error('WebSocket error:', event);
    });
	});
	
	const handleImgChage = (newImg: Uint8Array) => {
		socket.send(newImg);
	};

  return (
    <>
			<Canvas setImg={handleImgChage}/>
		</>
  );
}

export default App;
