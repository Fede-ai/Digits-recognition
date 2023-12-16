#pragma once
#include <vector>

class Layer {
public:
	Layer(int inNumBef, int inNumAft);
	std::vector<double> computeHidden(std::vector<double> inputs);
	std::vector<double> computeHiddenNodeValues(std::vector<double> nodeValuesAfter, Layer layerAft);
	std::vector<double> computeOutput(std::vector<double> inputs);
	std::vector<double> computeOutputNodeValues(std::vector<double> targets);

	void updateGradients(std::vector<double> nodeValues);
	void applyGradients(double learnRate);
	void clearGradients();

	static double hiddenAct(double value);
	static double hiddenActDerivative(double value);
	static double outputAct(double value);
	static double outputActDerivative(double value);

	static int random(int min, int max);
	int numBef = 0, numAft = 0;

	//relative to the nodes after
	std::vector<double> biases;
	std::vector<double> biasesGradients;
	//notation = weights[nodeBef][nodeAft]
	std::vector<std::vector<double>> weights;
	std::vector<std::vector<double>> weightsGradients;

	//the activatedValues of the layer before, one for each node bef
	std::vector<double> inputValues;
	//weighted sum of the activatedValues of the layer before, one for each node aft
	std::vector<double> weightedValues;
	//weightedValues after passing through the activation function, one for each node aft
	std::vector<double> activatedValues;
private:

};