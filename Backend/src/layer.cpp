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
		//weightedValues.push_back(value);
		values.push_back(hiddenAct(value));
	}

	//activatedValues = values;
	return values;
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
		values.push_back(outputAct(value));
	}

	//activatedValues = values;
	return values;
}

double Layer::hiddenAct(double value)
{
	//sigmoid
	return 1 / (1 + exp(-value));

	//ReLU
	//return std::max(value, double(0));
}
double Layer::outputAct(double value)
{
	return 1 / (1 + exp(-value));
}