#pragma once
#include <vector>

struct DataPoint
{
	DataPoint(std::vector<int> inData, std::vector<int> inTarget) {
		data = inData;
		target = inTarget;
	}
	std::vector<int> data;	
	std::vector<int> target;
};