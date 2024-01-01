#include "app.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

App::App()
	:
	//ai("best save.txt")
	ai(std::vector<int>{ 784, 400, 10 })
{
	fillDatasets();
	std::cout << "finished loading the images\n";
}

int App::run()
{
	int i = 0;
	while (true)
	{	
		//test every 25 'learn's
		if (i % 25 == 0)
		{
			auto testingBatch = createBatch(testingDataset, 128);
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

		auto trainingBatch = createBatch(trainingDataset, 64);
		for (auto& image : trainingBatch)
			image = applyRandomChanges(image);
		ai.learn(trainingBatch, 0.05);
		i++;

		//save nn to file every 100 'learn's 
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

	std::ifstream test("data/test.csv");
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
	//scale
	float num = Layer::random(700, 1200) / 1000.f;
	float zoom = int(28.f * num) / 28.f;
	int scaledSize = 28 * zoom;
	int topLeftCut = round((scaledSize - 28) / 2.f);
	std::vector<int> scaledImage;
	for (int y = 0; y < 28; y++)
	{
		for (int x = 0; x < 28; x++)
		{
			int xSrc = std::max(std::min(round((x + topLeftCut) / zoom), 27.f), 0.f);
			int ySrc = std::max(std::min(round((y + topLeftCut) / zoom), 27.f), 0.f);;
			scaledImage.push_back(datapoint.data[xSrc + 28 * ySrc]);
		}
	}

	//rotation

	//translation
	std::vector<int> translatedImage;
	int xTrans = Layer::random(0, 7) - 3; //it should be from 0 to 6 but I wanted to
	int yTrans = Layer::random(0, 7) - 3; //move everything to top-left a bit
	for (int y = 0; y < 28; y++)
	{
		for (int x = 0; x < 28; x++)
		{
			int xSrc = x + xTrans;
			int ySrc = y + yTrans;
			if (xSrc >= 28 || xSrc < 0|| ySrc >= 28 || ySrc < 0)
				translatedImage.push_back(0);
			else
				translatedImage.push_back(scaledImage[xSrc + ySrc * 28]);
		}
	}

	//noise
	int noiseProb = Layer::random(4, 8);
	for (auto& p : translatedImage)
	{
		if (Layer::random(0, 1000) < noiseProb)
			p = Layer::random(50, 200);
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