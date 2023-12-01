#pragma once
#include "datapoint.h"
#include "ai.h"

class App {
public:
	App();
	int run();

private:
	void fillDatasets();

	std::vector<DataPoint> trainingData;
	std::vector<DataPoint> testData;
};