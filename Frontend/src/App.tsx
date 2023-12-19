import React, {useState} from 'react';
import Canvas from './components/canvas';
import './App.css';

function App() {
	//const [img, setImg] = useState(new Uint8Array());

	const handleImgChage = (newImg: Uint8Array) => {
		console.log(newImg);
	};

  return (
    <>
			<Canvas setImg={handleImgChage}/>
		</>
  );
}

export default App;
