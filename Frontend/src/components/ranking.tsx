import React, {FC} from "react";
import rankingStyles from './ranking.module.css'

interface rankingProps {
	values:number[];
}

interface dataPoint {
	digit:number;
	value:number;
}

const Ranking:FC<rankingProps> = (props) => {

	const valuesInOrder = (values:number[]) => {
		let data:dataPoint[] = [];
		values.map((value, index) => (data.push({digit: index, value: value})));
		data.sort((a, b) => b.value - a.value);
		return (data);
	}

	return (
		<div className={rankingStyles.background}>
			{valuesInOrder(props.values).map((data, index) => {
				let style:string;
				if (index===0)
					style = rankingStyles.first;
				else
					style = rankingStyles.notFirst;

				return(
					<div className={style} key={index}>
					n° {data.digit}: {data.value.toFixed(3)}% </div>
			)})}
		</div>
	);
}

export default Ranking;