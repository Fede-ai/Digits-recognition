#pragma once
#include "datapoint.h"
#include "ai.h"

class App {
public:
	App();
	int run();

private:
	void fillDatasets();
	static std::vector<DataPoint> createBatch(std::vector<DataPoint> dataset, int num);

	Ai ai;

	std::vector<DataPoint> trainingDataset;
	int trainingDatasetSize = 10'000;
	std::vector<DataPoint> testingDataset;
	int testDatasetSize = 1'000;
};