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
		biasesGradients.push_back(0);
	}
	//setup weights
	for (int bef = 0; bef < numBef; bef++)
	{
		std::vector<double> partialWeights;
		std::vector<double> partialWeightsGradients;
		for (int aft = 0; aft < numAft; aft++)
		{
			partialWeights.push_back(random(0, 100'000) / 100'000.f - 0.5);
			partialWeightsGradients.push_back(0);
		}
		weights.push_back(partialWeights);
		weightsGradients.push_back(partialWeightsGradients);
	}
}

std::vector<double> Layer::computeHidden(std::vector<double> inputs)
{
	if (inputs.size() != numBef)
		std::exit(1000);

	inputValues = inputs;
	std::vector<double> values;
	weightedValues.clear();

	for (int aft = 0; aft < numAft; aft++)
	{
		double value = biases[aft];
		for (int bef = 0; bef < numBef; bef++)
		{
			value += inputs[bef] * weights[bef][aft];
		}
		weightedValues.push_back(value);
		values.push_back(hiddenAct(value));
	}

	activatedValues = values;
	return values;
}
std::vector<double> Layer::computeHiddenNodeValues(std::vector<double> nodeValuesAfter, Layer layerAft)
{
	std::vector<double> nodeValues;

	for (int aft = 0; aft < numAft; aft++)
	{
		double nodeValue = 0;
		for (int aftAft = 0; aftAft < layerAft.numAft; aftAft++)
		{
			double inputDerivative = layerAft.weights[aft][aftAft];
			nodeValue += inputDerivative * nodeValuesAfter[aftAft];
		}
		nodeValues.push_back(nodeValue * hiddenActDerivative(weightedValues[aft]));
	}

	return nodeValues;
}
std::vector<double> Layer::computeOutput(std::vector<double> inputs)
{
	if (inputs.size() != numBef)
		std::exit(1000);

	inputValues = inputs;
	std::vector<double> values;
	weightedValues.clear();

	for (int aft = 0; aft < numAft; aft++)
	{
		double value = biases[aft];
		for (int bef = 0; bef < numBef; bef++)
		{
			value += inputs[bef] * weights[bef][aft];
		}
		weightedValues.push_back(value);
		values.push_back(outputAct(value));
	}

	activatedValues = values;
	return values;
}
std::vector<double> Layer::computeOutputNodeValues(std::vector<double> targets)
{
	std::vector<double> nodeValues;

	for (int aft = 0; aft < numAft; aft++)
	{
		double costDerivative = 2 * (activatedValues[aft] - targets[aft]);
		double activationDerivative = outputActDerivative(weightedValues[aft]);
		nodeValues.push_back(costDerivative * activationDerivative);
	}

	return nodeValues;
}

void Layer::updateGradients(std::vector<double> nodeValues)
{
	for (int bef = 0; bef < numBef; bef++)
	{
		for (int aft = 0; aft < numAft; aft++)
		{
			double weightDerivative = inputValues[bef] * nodeValues[aft];
			weightsGradients[bef][aft] += weightDerivative;
		}
	}

	for (int aft = 0; aft < numAft; aft++)
	{
		biasesGradients[aft] += nodeValues[aft];
	}
}
void Layer::applyGradients(double learnRate)
{
	for (int bef = 0; bef < numBef; bef++)
	{
		for (int aft = 0; aft < numAft; aft++)
		{
			weights[bef][aft] -= weightsGradients[bef][aft] * learnRate;
		}
	}
		
	for (int aft = 0; aft < numAft; aft++)
	{
		biases[aft] -= biasesGradients[aft] * learnRate;
	}
}
void Layer::clearGradients() 
{
	for (int bef = 0; bef < numBef; bef++)
	{
		for (int aft = 0; aft < numAft; aft++)
		{
			weightsGradients[bef][aft] = 0;
		}
	}

	for (int aft = 0; aft < numAft; aft++)
	{
		biasesGradients[aft] = 0;
	}
}

double Layer::hiddenAct(double value)
{
	return 1 / (1 + exp(-value));
}
double Layer::hiddenActDerivative(double value)
{
	double activated = outputAct(value);
	return activated * (1 - activated);
}
double Layer::outputAct(double value)
{
	return 1 / (1 + exp(-value));
}
double Layer::outputActDerivative(double value)
{
	double activated = outputAct(value);
	return activated * (1 - activated);
}

int Layer::random(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max); 

	return dist6(rng);
}