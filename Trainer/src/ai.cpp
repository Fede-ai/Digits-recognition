#include "ai.h"
#include <Windows.h>
#include <fstream>
#include <sstream>

Ai::Ai(std::vector<int> inSizes)
{
	sizes = inSizes;

	for (int layer = 1; layer < sizes.size(); layer++)
		layers.push_back(Layer(sizes[layer - 1], sizes[layer]));
}
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

std::vector<double> Ai::forwardProp(DataPoint datapoint)
{
	std::vector<double> dataDouble;
	for (auto d : datapoint.data)
		dataDouble.push_back(d/255.f);

	std::vector<double> values = layers[0].computeHidden(dataDouble);
	for (int layer = 1; layer < layers.size() - 1; layer++)
		values = layers[layer].computeHidden(values);
	values = layers[layers.size() - 1].computeOutput(values);

	return values;
}
void Ai::backProp(DataPoint datapoint)
{
	forwardProp(datapoint);

	std::vector<double> targetDouble;
	for (auto t : datapoint.target)
		targetDouble.push_back(t);

	std::vector<double> nodeValues = layers[layers.size() - 1].computeOutputNodeValues(targetDouble);
	layers[layers.size() - 1].updateGradients(nodeValues);

	for (int layer = layers.size() - 2; layer >= 0; layer--)
	{
		nodeValues = layers[layer].computeHiddenNodeValues(nodeValues, layers[layer + 1]);
		layers[layer].updateGradients(nodeValues);
	}
}
double Ai::loss(std::vector<DataPoint> datapoints)
{
	double loss = 0;
	for (auto datapoint : datapoints)
	{
		std::vector<double> predictions = forwardProp(datapoint);
		for (int nOut = 0; nOut < predictions.size(); nOut++)
		{
			loss += Layer::loss(predictions[nOut], datapoint.target[nOut]);
		}
	}
	return (loss / datapoints.size());
}

void Ai::learn(std::vector<DataPoint> datapoints, double learnRate)
{
	for (auto& datapoint : datapoints)
		backProp(datapoint);

	for (auto& layer : layers)
		layer.applyGradients(learnRate/datapoints.size());

	for (auto& layer : layers)
		layer.clearGradients();
}
void Ai::save() const
{
	std::fstream file;
	file.open("save.txt", std::ios::out | std::ios::trunc);

	if (!file.is_open())
		return;

	//write size
	for (auto size : sizes)
	{
		file << size << ',';
	}
	file << '\n';

	//write biases and weights
	for (auto layer : layers)
	{
		for (int bef = 0; bef < layer.numBef; bef++)
		{
			for (int aft = 0; aft < layer.numAft; aft++)
			{
				file << std::to_string(layer.weights[bef][aft]) << ',';
			}
		}

		for (int aft = 0; aft < layer.numAft; aft++)
		{
			file << std::to_string(layer.biases[aft]) << ',';
		}

		file << '\n';
	}

	file.close();
}