import React, {FC} from "react";
import rankignStyles from './ranking.module.css'

interface rankingProps {
	rank:Number[];
}

const Ranking:FC<rankingProps> = (props) => {

	return (
		<div className={rankignStyles.background}>
		{props.rank.map((num, index)=>{
			return(
				<div key={index}> {index}: {num.toString()} </div>
			)})}
		</div>
	);
}

export default Ranking;