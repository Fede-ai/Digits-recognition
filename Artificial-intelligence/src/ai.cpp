#include "ai.h"

Ai::Ai(std::vector<int> inSizes)
{
	sizes = inSizes;

	for (int layer = 1; layer < sizes.size(); layer++)
		layers.push_back(Layer(sizes[layer - 1], sizes[layer]));
}

std::vector<double> Ai::forwardProp(DataPoint datapoint)
{
	std::vector<double> dataDouble;
	for (auto d : datapoint.data)
		dataDouble.push_back(d/255.f); //idk if the /255.f is necessary

	std::vector<double> values = layers[0].computeHidden(dataDouble);
	for (int layer = 1; layer < layers.size() - 1; layer++)
		values = layers[layer].computeHidden(values);
	values = layers[layers.size() - 1].computeOutput(values);

	return values;
}
double Ai::loss(std::vector<DataPoint> datapoints)
{
	double loss = 0;
	for (auto datapoint : datapoints)
	{
		std::vector<double> predictions = forwardProp(datapoint);
		for (int nOut = 0; nOut < predictions.size(); nOut++)
		{
			loss += (predictions[nOut] - datapoint.target[nOut]) * (predictions[nOut] - datapoint.target[nOut]);
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
void Ai::backProp(DataPoint datapoint)
{
	/*const double h = 0.0001;
	const double initialLoss = loss(datapoints);

	for (auto& layer : layers)
	{
		for (int bef = 0; bef < layer.numBef; bef++)
		{
			for (int aft = 0; aft < layer.numAft; aft++)
			{
				layer.weights[bef][aft] += h;
				double deltaLoss = loss(datapoints) - initialLoss;
				layer.weights[bef][aft] -= h;
				layer.weightsGradients[bef][aft] = deltaLoss / h;
			}
		}

		for (int aft = 0; aft < layer.numAft; aft++)
		{
			layer.biases[aft] += h;
			double deltaLoss = loss(datapoints) - initialLoss;
			layer.biases[aft] -= h;
			layer.biasesGradients[aft] = deltaLoss / h;
		}
	}*/

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