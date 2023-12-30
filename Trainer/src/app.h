#pragma once
#include "datapoint.h"
#include "ai.h"
#include <string>

class App {
public:
	App();
	int run();

private:
	void fillDatasets();
	static std::vector<DataPoint> createBatch(std::vector<DataPoint> dataset, int num);
	static DataPoint applyRandomChanges(DataPoint datapoint);
	int getPrediction(DataPoint datapoint);
	static int getTarget(DataPoint datapoint);
	static void saveToFile(DataPoint datapoint, std::string path);

	Ai ai;
	std::vector<DataPoint> trainingDataset;
	int trainingDatasetSize = 50'000;
	std::vector<DataPoint> testingDataset;
	int testDatasetSize = 5'000;
};