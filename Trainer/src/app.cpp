#include "app.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
//#include <SFML/Graphics.hpp>

App::App()
	:
	ai({ 784, 100, 10 })
{
	fillDatasets();
	std::cout << "finished loading the images\n";
}

int App::run()
{
	int i = 0;
	while (true)
	{
		auto testingBatch = createBatch(testingDataset, 128);
		int correct = 0;
		for (auto& datapoint : testingBatch)
		{
			if (getPrediction(datapoint) == getTarget(datapoint))
				correct++;
		}

		if (i % 10 == 0)
			std::cout << i << ", loss: " << ai.loss(testingBatch) << ", correct: " << correct << "/128 - " << correct / 1.28 << "%\n";

		auto trainingBatch = createBatch(trainingDataset, 64);
		ai.learn(trainingBatch, 0.3);
		i++;

		if (i % 100 == 1 && i != 1)
		{
			ai.save();
			std::cout << "saved\n";
		}
	}

	return 0;
}

void App::fillDatasets()
{
	//skip n lines
	/*for (int i = 0; i < 0; ++i) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}*/
	
	//save image to file
	/*sf::Image image;
	image.create(28, 28);
	for (int y = 0; y < 28; y++)
	{
		for (int x = 0; x < 28; x++)
		{
			int c = datapoint.data[y * 28 + x];
			image.setPixel(x, y, sf::Color(c, c, c));
		}
	}
	image.saveToFile("images/image" + std::to_string(nImg) + ".png");*/

	std::fstream test("data/test.csv");
	test.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip labels row
	for (int nImg = 0; nImg < testDatasetSize; nImg++)
	{
		std::string line;
		getline(test, line);
	
		int target = std::stoi(std::string(1, line[0]));
		line.erase(0, 1);
		std::vector<int> targets {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		targets[target] = 1;
	
		std::vector<int> data;
		for (int pixel = 0; pixel < 784; pixel++)
		{
			line.erase(0, 1);
			std::string dataStr = "";
			while (line != "" && line[0] != ',')
			{
				dataStr = dataStr + line[0];
				line.erase(0, 1);
			}
			data.push_back(stoi(dataStr));
		}
	
		testingDataset.push_back(DataPoint(data, targets));
	}
	test.close();

	std::fstream train("data/train.csv");
	train.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip labels row
	for (int nImg = 0; nImg < trainingDatasetSize; nImg++)
	{
		std::string line;
		getline(train, line);

		int target = std::stoi(std::string(1, line[0]));
		line.erase(0, 1);
		std::vector<int> targets{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		targets[target] = 1;

		std::vector<int> data;
		for (int pixel = 0; pixel < 784; pixel++)
		{
			line.erase(0, 1);
			std::string dataStr = "";
			while (line != "" && line[0] != ',')
			{
				dataStr = dataStr + line[0];
				line.erase(0, 1);
			}
			data.push_back(stoi(dataStr));
		}

		trainingDataset.push_back(DataPoint(data, targets));
	}
	train.close();
}

std::vector<DataPoint> App::createBatch(std::vector<DataPoint> dataset, int num)
{
	if (dataset.size() < num)
		std::exit(1000);

	std::vector<DataPoint> batch;	
	for (int i = 0; i < num; i++)
	{
		int index = Layer::random(0, dataset.size() - 1);
		batch.push_back(dataset[index]);
		dataset.erase(dataset.begin() + index);
	}

	return batch;
}

int App::getPrediction(DataPoint datapoint)
{
	auto predictions = ai.forwardProp(datapoint);
	int highest = 0;

	for (int i = 1; i < predictions.size(); i++)
	{
		if (predictions[i] > predictions[highest])
			highest = i;
	}
	return highest;
}
int App::getTarget(DataPoint datapoint)
{
	int highest = 0;

	for (int i = 1; i < datapoint.target.size(); i++)
	{
		if (datapoint.target[i] > datapoint.target[highest])
			highest = i;
	}
	return highest;
}