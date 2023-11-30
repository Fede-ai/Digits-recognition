#pragma once
#include <vector>

class Layer {
public:
	Layer(int inNumBef, int inNumAft);
	std::vector<double> compute(std::vector<double> inputs);
	std::vector<double> computeOutput(std::vector<double> inputs);

	static int random(int min, int max);

private:
	int numBef = 0, numAft = 0;

	//relative to the nodes after
	std::vector<double> biases;
	//notation = weights[nodeBef][nodeAft]
	std::vector<std::vector<double>> weights;
};