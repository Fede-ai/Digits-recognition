#pragma once
#include <vector>
#include "dataPoint.h"
#include "layer.h"

class Ai {
public:
	Ai(std::vector<int> inSize);
	std::vector<double> forwardProp(std::vector<int> data);
	double cost(std::vector<DataPoint> datapoints);
	void backProp(std::vector<DataPoint> datapoints);
	void applyAllGradients();

private:
	std::vector<int> sizes;
	std::vector<Layer> layers;
};