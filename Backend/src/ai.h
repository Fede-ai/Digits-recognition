#pragma once
#include <vector>
#include "layer.h"
#include <string>

class Ai {
public:
	Ai(std::string path);
	std::vector<double> forwardProp(std::vector<int> data) const;

private:	
	std::vector<int> sizes;
	std::vector<Layer> layers;
};