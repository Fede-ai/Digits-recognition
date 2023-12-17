#pragma once
#include <vector>
#include "dataPoint.h"
#include "layer.h"

class Ai {
public:
	Ai(std::vector<int> inSize);
	std::vector<double> forwardProp(DataPoint datapoint);
	double loss(std::vector<DataPoint> datapoints);
	void learn(std::vector<DataPoint> datapoints, double learnRate);
	void save() const;

private:	
	void backProp(DataPoint datapoint);
	std::vector<int> sizes;
	std::vector<Layer> layers;
};