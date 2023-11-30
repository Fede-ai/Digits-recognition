#pragma once
#include <vector>
#include "layer.h"

class Ai {
public:
	Ai(std::vector<int> inSize);
	std::vector<double> forward(std::vector<int> data);

private:
	std::vector<int> sizes;
	std::vector<Layer> layers;
};