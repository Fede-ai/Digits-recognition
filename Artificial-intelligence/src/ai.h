#pragma once
#include <vector>
#include "dataPoint.h"
#include "layer.h"

class Ai {
public:
	Ai(std::vector<int> inSize);
	std::vector<double> forwardProp(std::vector<int> data);
	double loss(std::vector<DataPoint> datapoints);
	void learn(std::vector<DataPoint> datapoints, double learnRate);

private:	
	void backProp(std::vector<DataPoint> datapoints);
	std::vector<int> sizes;
	std::vector<Layer> layers;
};