#include "ai.h"

Ai::Ai(std::vector<int> inSizes)
{
	sizes = inSizes;

	for (int layer = 1; layer < sizes.size(); layer++)
		layers.push_back(Layer(sizes[layer - 1], sizes[layer]));
}

std::vector<double> Ai::forwardProp(std::vector<int> data)
{
	std::vector<double> dataDouble;
	for (auto d : data)
		dataDouble.push_back(d);

	std::vector<double> values = layers[0].compute(dataDouble);
	for (int layer = 1; layer < layers.size() - 1; layer++)
		values = layers[layer].compute(values);
	values = layers[layers.size() - 1].computeOutput(values);

	return values;
}

double Ai::cost(std::vector<DataPoint> datapoints)
{
	double cost = 0;
	for (auto datapoint : datapoints)
	{
		std::vector<double> predictions = forwardProp(datapoint.data);
		for (int nOut = 0; nOut < predictions.size(); nOut++)
		{
			cost += (predictions[nOut] - datapoint.target[nOut]) * (predictions[nOut] - datapoint.target[nOut]);
		}
	}
	return (cost / datapoints.size());
}

void Ai::backProp(std::vector<DataPoint> datapoints)
{
}

void Ai::applyAllGradients()
{
}
