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

	weightedValues.clear();
	for (int aft = 0; aft < numAft; aft++)
	{
		double weightedValue = biases[aft];
		for (int bef = 0; bef < numBef; bef++)
			weightedValue += inputs[bef] * weights[bef][aft];
		weightedValues.push_back(weightedValue);
	}

	activatedValues = hiddenAct(weightedValues);
	return activatedValues;
}
std::vector<double> Layer::computeHiddenNodeValues(std::vector<double> nodeValuesAfter, Layer layerAft) const
{
	std::vector<double> nodeValues;
	std::vector<double> actDer = hiddenActDer(weightedValues);

	for (int aft = 0; aft < numAft; aft++)
	{
		double nodeValue = 0;
		for (int aftAft = 0; aftAft < layerAft.numAft; aftAft++)
		{
			double inputDerivative = layerAft.weights[aft][aftAft];
			nodeValue += inputDerivative * nodeValuesAfter[aftAft];
		}
		nodeValues.push_back(nodeValue * actDer[aft]);
	}

	return nodeValues;
}
std::vector<double> Layer::computeOutput(std::vector<double> inputs)
{
	if (inputs.size() != numBef)
		std::exit(1000);

	inputValues = inputs;

	weightedValues.clear();
	for (int aft = 0; aft < numAft; aft++)
	{
		double weightedValue = biases[aft];
		for (int bef = 0; bef < numBef; bef++)
			weightedValue += inputs[bef] * weights[bef][aft];
		weightedValues.push_back(weightedValue);
	}

	activatedValues = outputAct(weightedValues);
	return activatedValues;
}
std::vector<double> Layer::computeOutputNodeValues(std::vector<double> targets) const
{
	std::vector<double> nodeValues;
	std::vector<double> lossAndActDer = lossAndOutputActDer(activatedValues, targets);

	for (int aft = 0; aft < numAft; aft++)
		nodeValues.push_back(lossAndActDer[aft]);

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
std::vector<double> Layer::hiddenActDer(std::vector<double> values)
{
	std::vector<double> derivatives;

	//sigmoid
	//double activated = outputAct(value);
	//return activated * (1 - activated);

	//ReLU
	for (auto v : values)
	{
		if (v < 0)
			derivatives.push_back(0);
		else
			derivatives.push_back(1);
	}
	
	return derivatives;
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

double Layer::loss(std::vector<double> values, std::vector<double> targets)
{
	double loss = 0.0;

	//cross entropy
	for (int i = 0; i < values.size(); i++)
		loss += -targets[i] * std::log(values[i] + 1e-15);

	return loss;
}
std::vector<double> Layer::lossAndOutputActDer(std::vector<double> values, std::vector<double> targets)
{
	std::vector<double> nodesLossesDer;

	//softmax + cross entropy
	for (int i = 0; i < values.size(); i++)
		nodesLossesDer.push_back(values[i] - targets[i]);

	return nodesLossesDer;
}

int Layer::random(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max); 

	return dist6(rng);
}