#pragma once
#include <vector>

class Layer {
public:
	Layer(int inNumBef, int inNumAft);
	std::vector<double> computeHidden(std::vector<double> inputs) const;
	std::vector<double> computeOutput(std::vector<double> inputs) const;

	const int numBef, numAft;	
	
	//relative to the nodes after
	std::vector<double> biases;	
	//notation: weights[nodeBef][nodeAft]
	std::vector<std::vector<double>> weights;

private:
	static double hiddenAct(double value);
	static double outputAct(double value);
};