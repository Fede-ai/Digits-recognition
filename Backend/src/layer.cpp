#include "layer.h"
#include <random>

Layer::Layer(int inNumBef, int inNumAft)
	:
	numBef(inNumBef),
	numAft(inNumAft)
{
	//setup biases
	for (int bias = 0; bias < numAft; bias++)
	{
		biases.push_back(0);
	}
	//setup weights
	for (int bef = 0; bef < numBef; bef++)
	{
		std::vector<double> partialWeights;
		for (int aft = 0; aft < numAft; aft++)
		{
			partialWeights.push_back(0);
		}
		weights.push_back(partialWeights);
	}
}

std::vector<double> Layer::computeHidden(std::vector<double> inputs) const
{
	if (inputs.size() != numBef)
		std::exit(1000);

	//inputValues = inputs;
	std::vector<double> values;
	//weightedValues.clear();

	for (int aft = 0; aft < numAft; aft++)
	{
		double value = biases[aft];
		for (int bef = 0; bef < numBef; bef++)
		{
			value += inputs[bef] * weights[bef][aft];
		}
		values.push_back(value);
		//weightedValues.push_back(value);
	}

	//activatedValues = values;
	return hiddenAct(values);
}
std::vector<double> Layer::computeOutput(std::vector<double> inputs) const
{
	if (inputs.size() != numBef)
		std::exit(1000);

	//inputValues = inputs;
	std::vector<double> values;
	//weightedValues.clear();

	for (int aft = 0; aft < numAft; aft++)
	{
		double value = biases[aft];
		for (int bef = 0; bef < numBef; bef++)
		{
			value += inputs[bef] * weights[bef][aft];
		}
		//weightedValues.push_back(value);
		values.push_back(value);
	}

	//activatedValues = values;
	return outputAct(values);
}

std::vector<double> Layer::hiddenAct(std::vector<double> values)
{
	std::vector<double> activated;

	//sigmoid
	//return 1 / (1 + exp(-value));

	//ReLU
	for (auto v : values)
		activated.push_back(std::max(v, double(0)));

	return activated;
}
std::vector<double> Layer::outputAct(std::vector<double> values)
{
	std::vector<double> activated;

	//softmax
	double expSum = 0;
	for (double v : values) {
		double exp = std::exp(v);
		activated.push_back(exp);
		expSum += exp;
	}
	for (double& v : activated) {
		v /= expSum;
	}

	return activated;
}