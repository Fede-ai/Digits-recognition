#pragma once
#include <vector>

class Layer {
public:
	Layer(int inNumBef, int inNumAft);
	std::vector<double> compute(std::vector<double> inputs);
	std::vector<double> computeOutput(std::vector<double> inputs);
	void applyGradients(double learnRate);

	static int random(int min, int max);
	int numBef = 0, numAft = 0;

	//relative to the nodes after
	std::vector<double> biases;
	std::vector<double> biasesGradients;
	//notation = weights[nodeBef][nodeAft]
	std::vector<std::vector<double>> weights;
	std::vector<std::vector<double>> weightsGradients;

public:

};