#include "app.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

App::App()
	:
	//ai("best save.txt")
	ai(std::vector<int>{ 784, 800, 10 })
{
	testDataset = createDatasets("data/test.csv", testDatasetSize);
	std::cout << "finished loading the test images\n";
	trainDataset = createDatasets("data/train.csv", trainDatasetSize);
	std::cout << "finished loading the train images\n";
}

int App::run()
{
	int i = 0;
	while (true)
	{	
		//test every 25 'learn's
		if (i % 25 == 0)
		{
			auto testingBatch = createBatch(testDataset, 128);
			int normalCorrect = 0, modifiedCorrect = 0;
			double normalLoss = ai.loss(testingBatch);
			for (auto& datapoint : testingBatch)
			{
				if (getPrediction(datapoint) == getTarget(datapoint))
					normalCorrect++;
				datapoint = applyRandomChanges(datapoint);
				if (getPrediction(datapoint) == getTarget(datapoint))
					modifiedCorrect++;
			}
			std::cout << "training number " << i << "\n";
			std::cout << "normal loss: " << normalLoss << ", correct: " << normalCorrect << "/128 - " << normalCorrect / 1.28 << "%\n";
			std::cout << "modified loss: " << ai.loss(testingBatch) << ", correct: " << modifiedCorrect << "/128 - " << modifiedCorrect / 1.28 << "%\n";
		}

		auto trainingBatch = createBatch(trainDataset, 64);
		for (auto& image : trainingBatch)
			image = applyRandomChanges(image);
		ai.learn(trainingBatch, 0.05, 0.3);
		i++;

		//save nn to file every 100 'learn's 
		if (i % 100 == 1 && i != 1)
		{
			ai.save();
			std::cout << "saved\n";
		}
	}

	auto image = createBatch(testDataset, 1)[0];
	saveToFile(image, "images/img0.png");
	for (i = 0; i < 100; i++)
		saveToFile(applyRandomChanges(image), "images/img" + std::to_string(i + 1) + ".png");

	return 0;
}

std::vector<DataPoint> App::createDatasets(std::string path, int num)
{
	//skip n lines
	/*for (int i = 0; i < 0; ++i) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}*/

	std::vector<DataPoint> dataset;

	std::ifstream file(path);
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip labels row
	for (int nImg = 0; nImg < num; nImg++)
	{
		std::string line;
		getline(file, line);
	
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
	
		dataset.push_back(DataPoint(data, targets));
	}
	file.close();

	return dataset;
}
std::vector<DataPoint> App::createBatch(std::vector<DataPoint> dataset, int size)
{
	if (dataset.size() < size)
		std::exit(1000);

	std::vector<DataPoint> batch;	
	for (int i = 0; i < size; i++)
	{
		int index = Layer::random(0, dataset.size() - 1);
		batch.push_back(dataset[index]);
		dataset.erase(dataset.begin() + index);
	}

	return batch;
}

DataPoint App::applyRandomChanges(DataPoint datapoint)
{
	//calculate a position's color based on the surrounding pixels and the distance to them
	//used for scale and rotation
	auto getColor = [](std::vector<int> vector, float x, float y) {
		auto i = [vector](int x, int y) {
			return vector[x + 28 * y];
		};
		
		int xInt = std::floor(x), yInt = std::floor(y);
		if (xInt == 27)
			return int(i(xInt, yInt) * (yInt + 1 - y) + i(xInt, yInt+1) * (y - yInt));
		if (yInt == 27)
			return int(i(xInt, yInt) * (xInt + 1 - x) + i(xInt+1,yInt) * (x - xInt));

		//first calculate how white should the top and bot part be
		float xFraction = x - xInt;
		float topInterpolation = (1 - xFraction) * i(yInt, xInt) + xFraction * i(yInt, xInt + 1);
		float bottomInterpolation = (1 - xFraction) * i(yInt + 1, xInt) + xFraction * i(yInt + 1, xInt + 1);

		//than merge this two parts in the same way that you merged the two pixels of the bot/top part
		float yFraction = y - yInt;
		return int((1 - yFraction) * topInterpolation + yFraction * bottomInterpolation);
	};

	//scale
	float zoom = Layer::random(750, 1150) / 1000.f;
	float topLeftCut = 14.f * (zoom - 1);
	std::vector<int> scaledImage;
	for (int y = 0; y < 28; y++)
	{
		for (int x = 0; x < 28; x++)
		{
			float xSrc = (x + topLeftCut) / zoom;
			float ySrc = (y + topLeftCut) / zoom;
			if (xSrc >= 28 || xSrc < 0 || ySrc >= 28 || ySrc < 0)
				scaledImage.push_back(0);
			else
				scaledImage.push_back(getColor(datapoint.data, xSrc, ySrc));
		}
	}

	//rotation
	float ang = (Layer::random(0, 60) - 30) / 180.f * 3.1415;
	std::vector<int> rotatedImage;
	for (int y = 0; y < 28; y++)
	{
		for (int x = 0; x < 28; x++)
		{
			if (x == 14 && y == 14)
			{
				rotatedImage.push_back(scaledImage[x + y * 28]);
				continue;
			}
			int rx = x - 14, ry = y - 14;
			float ipot = sqrt(rx * rx + ry * ry);

			float srcAng = acos(rx / ipot);
			if (ry < 0)
				srcAng = 2 * 3.1415 - srcAng;
			srcAng += ang;

			float xSrc = ipot * cos(srcAng) + 14;
			float ySrc = ipot * sin(srcAng) + 14;
			if (xSrc >= 28 || xSrc < 0 || ySrc >= 28 || ySrc < 0)
				rotatedImage.push_back(0);
			else
				rotatedImage.push_back(getColor(scaledImage, xSrc, ySrc));
		}
	}

	//translation
	int xTrans = Layer::random(0, 10) - 5;
	int yTrans = Layer::random(0, 6) - 3;
	std::vector<int> translatedImage;
	for (int y = 0; y < 28; y++)
	{
		for (int x = 0; x < 28; x++)
		{
			int xSrc = x + xTrans;
			int ySrc = y + yTrans;
			if (xSrc >= 28 || xSrc < 0|| ySrc >= 28 || ySrc < 0)
				translatedImage.push_back(0);
			else
				translatedImage.push_back(rotatedImage[xSrc + ySrc * 28]);
		}
	}

	//noise
	int noiseProb = Layer::random(5, 9); //0.4% -> 0.8%
	for (auto& p : translatedImage)
	{
		if (Layer::random(0, 1000) < noiseProb)
			p = Layer::random(130, 200);
	}

	return DataPoint(translatedImage, datapoint.target);
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

void App::saveToFile(DataPoint datapoint, std::string path)
{
	sf::Image file;
	file.create(28, 28);

	for (int x = 0; x < 28; x++)
	{
		for (int y = 0; y < 28; y++)
		{
			int c = datapoint.data[x + 28 * y];
			file.setPixel(x, y, sf::Color(c, c, c));
		}
	}
	
	file.saveToFile(path);
}