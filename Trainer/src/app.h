#pragma once
#include "datapoint.h"
#include "ai.h"
#include <string>

class App {
public:
	App();
	int run();

private:
	static std::vector<DataPoint> createDatasets(std::string path, int num);
	static std::vector<DataPoint> createBatch(std::vector<DataPoint> dataset, int num);
	static DataPoint applyRandomChanges(DataPoint datapoint);
	int getPrediction(DataPoint datapoint);
	static int getTarget(DataPoint datapoint);
	static void saveToFile(DataPoint datapoint, std::string path);

	Ai ai;
	std::vector<DataPoint> trainDataset;
	int trainDatasetSize = 50'000;
	std::vector<DataPoint> testDataset;
	int testDatasetSize = 5'000;
};