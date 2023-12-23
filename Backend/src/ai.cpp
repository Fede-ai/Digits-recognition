#include "ai.h"
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <iostream>

Ai::Ai(std::string path)
{
	std::fstream file;
	std::string line, token;
	file.open(path, std::ios::in);
	if (!file.is_open())
		return;

	//extract and load layers sizes
	getline(file, line);
	std::istringstream sizesStream(line);
	while (getline(sizesStream, token, ','))
		sizes.push_back(stoi(token));
	for (int i = 1; i < sizes.size(); i++)
		layers.push_back(Layer(sizes[i - 1], sizes[i]));

	for (auto& layer : layers)
	{
		getline(file, line);
		std::istringstream layerStrean(line);

		for (int bef = 0; bef < layer.numBef; bef++)
		{
			for (int aft = 0; aft < layer.numAft; aft++)
			{
				getline(layerStrean, token, ',');
				layer.weights[bef][aft] = stod(token);
			}
		}

		for (int aft = 0; aft < layer.numAft; aft++)
		{
			getline(layerStrean, token, ',');
			layer.biases[aft] = stod(token);
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