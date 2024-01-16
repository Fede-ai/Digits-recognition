import React, {useEffect, useState} from 'react';
import Canvas from './components/canvas';
import Ranking from './components/ranking';
import './App.css';

//var socket = new WebSocket('ws://192.168.10.25:9002');	
//var socket = new WebSocket('ws://192.168.1.191:9002');	
var socket = new WebSocket('ws:/192.168.100.41:9002');
//var socket = new WebSocket('ws://2.235.241.210:9002');

function App() {	
	const [ranking, setRanking] = useState([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);

	useEffect(() => {
    socket.addEventListener('message', (event) => {
      let data:string = event.data;
			let valuesStr = data.split(',');

			let values:number[] = [];
			valuesStr.forEach(str => {
				values.push(Number(str) * 100);
			});
			setRanking(values);
    });
    socket.addEventListener('close', (event) => {
      //console.log('WebSocket connection closed:', event);
    });
	}, []);
	
	const sendImg = async (newImg: Uint8ClampedArray) => {
		if (socket.readyState !== socket.OPEN) return
		//only take one value out of four
		let essData = new Uint8Array(newImg.length/4);
		for (let i = 0; i < newImg.length; i++) {
			if (i % 4 === 0)
				essData[i/4] = newImg[i];
		}
		//lower resolution
		const ratio = Math.sqrt(essData.length) / 28;
    const compData = new Uint8Array(28 * 28);
    for (let y = 0; y < 28; y++) {
      for (let x = 0; x < 28; x++) {
        let sum = 0;
        for (let i = 0; i < ratio; i++) {
          for (let j = 0; j < ratio; j++) {
            const originalX = Math.floor(x * ratio) + i;
            const originalY = Math.floor(y * ratio) + j;
            const index = originalY * Math.sqrt(essData.length) + originalX;
            sum += essData[index];
          }
        }
        const average = sum / (ratio * ratio);
        compData[y * 28 + x] = Math.min(Math.round(average), 255);
      }
    }
		//add img to a string and send
		let packet = "";
		for (let i = 0; i < compData.length; i++)
			packet = packet + compData[i] + ",";
		socket.send(packet);
		console.log("data processed and sent");
	};

  return (
    <>
			<Canvas passImg={sendImg}/>
			<Ranking values={ranking}/>
		</>
  );
}

export default App;
