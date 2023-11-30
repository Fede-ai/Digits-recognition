#include "layer.h"
#include <random>

Layer::Layer(int inNumBef, int inNumAft)
{
	numBef = inNumBef;
	numAft = inNumAft;

	//setup biases
	for (int bias = 0; bias < numAft; bias++)
	{
		biases.push_back(random(0, 100'000) / 100'000.f - 0.5);
	}
	//setup weights
	for (int bef = 0; bef < numBef; bef++)
	{
		std::vector<double> partialWeights;
		for (int aft = 0; aft < numAft; aft++)
		{
			partialWeights.push_back(random(0, 100'000) / 100'000.f - 0.5);
		}
		weights.push_back(partialWeights);
	}
}

std::vector<double> Layer::compute(std::vector<double> inputs)
{
	if (inputs.size() != numBef)
		std::exit(1000);

	std::vector<double> values;

	for (int aft = 0; aft < numAft; aft++)
	{
		double value = biases[aft];
		for (int bef = 0; bef < numBef; bef++)
		{
			value += inputs[bef] * weights[bef][aft];
		}
		values.push_back(std::max(value, double(0)));
	}

	return values;
}

std::vector<double> Layer::computeOutput(std::vector<double> inputs)
{
	if (inputs.size() != numBef)
		std::exit(1000);

	std::vector<double> values;

	for (int aft = 0; aft < numAft; aft++)
	{
		double value = biases[aft];
		for (int bef = 0; bef < numBef; bef++)
		{
			value += inputs[bef] * weights[bef][aft];
		}
		values.push_back(value);
	}

	double sum = 0;
	for (auto value : values)
		sum += exp(value);

	for (auto& value : values)
		value = exp(value) / sum;

	return values;
}

int Layer::random(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max); 

	return dist6(rng);
}