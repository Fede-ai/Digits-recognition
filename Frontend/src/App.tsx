import React, {useEffect, useState} from 'react';
import Canvas from './components/canvas';
import Ranking from './components/ranking';
import './App.css';

//var socket = new WebSocket('ws://192.168.10.25:9002');	
var socket = new WebSocket('ws://192.168.1.191:9002');	

function App() {	
	const [ranking, setRanking] = useState([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);

	useEffect(() => {
    socket.addEventListener('message', (event) => {
      let data:String = event.data;
			let valuesStr = data.split(',');
			let values:number[] = [];
			for (let i = 0; i < valuesStr.length; i++)
				values.push(Number(valuesStr[i]));
			setRanking(values);
    });
    socket.addEventListener('close', (event) => {
      //console.log('WebSocket connection closed:', event);
    });
	});
	
	const processImg = (data: Uint8Array) => {
		//lower resolution
		const ratio = Math.sqrt(data.length) / 28;
    const newData = new Uint8Array(28 * 28);
    for (let y = 0; y < 28; y++) {
      for (let x = 0; x < 28; x++) {
        let sum = 0;
        for (let i = 0; i < ratio; i++) {
          for (let j = 0; j < ratio; j++) {
            const originalX = Math.floor(x * ratio) + i;
            const originalY = Math.floor(y * ratio) + j;
            const index = originalY * Math.sqrt(data.length) + originalX;
            sum += data[index];
          }
        }
        const average = sum / (ratio * ratio);
        newData[y * 28 + x] = Math.min(Math.round(average), 255);
      }
    }

		//transform the image to a string and send it
		let packet:string = "";
		for (let i = 0; i < newData.length; i++)
			packet = packet + newData[i] + ",";
		socket.send(packet);
	};

  return (
    <>
			<Canvas setImg={async(data: Uint8Array) => {processImg(data)}}/>
			<Ranking rank={ranking}/>
		</>
  );
}

export default App;
