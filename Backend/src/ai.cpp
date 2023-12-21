#include "ai.h"
#include <Windows.h>
#include <fstream>

Ai::Ai(std::string path)
{
	std::fstream file;
	file.open(path, std::ios::in);

	char div;
	//extract layers size
	do {
		int value;
		file >> value;
		sizes.push_back(value);
		file >> div;
	} while (div == ',');
	//load layers size
	for (int i = 1; i < sizes.size(); i++)
	{
		layers.push_back(Layer(sizes[i - 1], sizes[i]));
	}
	//load biases
	for (int layer = 0; layer < layers.size(); layer++)
	{
		for (int bias = 0; bias < sizes[layer + 1]; bias++)
		{
			double value;
			file >> value;
			layers[layer].biases[bias] = value;
			file >> div;
		}
	}
	//load weights
	for (int layer = 0; layer < layers.size(); layer++)
	{
		for (int nodeAft = 0; nodeAft < layers[layer].numAft; nodeAft++)
		{
			for (int nodeBef = 0; nodeBef < layers[layer].numBef; nodeBef++)
			{
				double value;
				file >> value;
				layers[layer].weights[nodeAft][nodeBef] = value;
				file >> div;
			}
		}
	}
	file.close();
}

std::vector<double> Ai::forwardProp(std::vector<int> data) const
{
	std::vector<double> dataDouble;
	for (auto d : data)
		dataDouble.push_back(d/255.f);

	std::vector<double> values = layers[0].computeHidden(dataDouble);
	for (int layer = 1; layer < layers.size() - 1; layer++)
		values = layers[layer].computeHidden(values);
	values = layers[layers.size() - 1].computeOutput(values);

	return values;
}