#include "ai.h"
#include <Windows.h>
#include <fstream>
#include <string>

Ai::Ai(std::vector<int> inSizes)
{
	sizes = inSizes;

	for (int layer = 1; layer < sizes.size(); layer++)
		layers.push_back(Layer(sizes[layer - 1], sizes[layer]));
}

std::vector<double> Ai::forwardProp(DataPoint datapoint)
{
	std::vector<double> dataDouble;
	for (auto d : datapoint.data)
		dataDouble.push_back(d/255.f);

	std::vector<double> values = layers[0].computeHidden(dataDouble);
	for (int layer = 1; layer < layers.size() - 1; layer++)
		values = layers[layer].computeHidden(values);
	values = layers[layers.size() - 1].computeOutput(values);

	return values;
}
void Ai::backProp(DataPoint datapoint)
{
	forwardProp(datapoint);

	std::vector<double> targetDouble;
	for (auto t : datapoint.target)
		targetDouble.push_back(t);

	std::vector<double> nodeValues = layers[layers.size() - 1].computeOutputNodeValues(targetDouble);
	layers[layers.size() - 1].updateGradients(nodeValues);

	for (int layer = layers.size() - 2; layer >= 0; layer--)
	{
		nodeValues = layers[layer].computeHiddenNodeValues(nodeValues, layers[layer + 1]);
		layers[layer].updateGradients(nodeValues);
	}
}
double Ai::loss(std::vector<DataPoint> datapoints)
{
	double loss = 0;
	for (auto datapoint : datapoints)
	{
		std::vector<double> predictions = forwardProp(datapoint);
		for (int nOut = 0; nOut < predictions.size(); nOut++)
		{
			loss += Layer::loss(predictions[nOut], datapoint.target[nOut]);
		}
	}
	return (loss / datapoints.size());
}

void Ai::learn(std::vector<DataPoint> datapoints, double learnRate)
{
	for (auto& datapoint : datapoints)
		backProp(datapoint);

	for (auto& layer : layers)
		layer.applyGradients(learnRate/datapoints.size());

	for (auto& layer : layers)
		layer.clearGradients();
}
void Ai::save() const
{
	//OPENFILENAMEA select;
	//char path[100];
	//
	//ZeroMemory(&select, sizeof(select));
	//select.lStructSize = sizeof(select);
	//select.hwndOwner = NULL;
	//select.lpstrFilter = (PSTR)"Text (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	//select.lpstrFile = path;
	//select.lpstrFile[0] = '\0';
	//select.nMaxFile = sizeof(path);
	//select.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	//select.lpstrDefExt = (LPCSTR)L"txt";
	//
	//if (GetSaveFileNameA(&select) == true)
	{
		std::fstream file;
		file.open("save.txt", std::ios::out | std::ios::trunc);

		if (!file.is_open())
			return;

		//write size
		for (int layer = 0; layer < sizes.size(); layer++)
		{
			file << sizes[layer];
			if (layer == sizes.size() - 1)
				file << ';';
			else
				file << ',';
		}
		file << '\n';

		//write weights
		for (int layer = 0; layer < layers.size(); layer++)
		{
			for (int bias = 0; bias < layers[layer].biases.size(); bias++)
			{
				file << std::to_string(layers[layer].biases[bias]);
				if (bias == layers[layer].biases.size() - 1)
					file << ';';
				else
					file << ',';
			}
			file << '\n';
		}

		//write biases
		for (int layer = 0; layer < layers.size(); layer++)
		{
			for (int nodeAft = 0; nodeAft < layers[layer].numAft; nodeAft++)
			{
				for (int nodeBef = 0; nodeBef < layers[layer].numBef; nodeBef++)
				{
					file << std::to_string(layers[layer].weights[nodeAft][nodeBef]);
					if (nodeBef == layers[layer].numBef - 1)
						file << ';';
					else
						file << ',';
				}
				file << '\n';
			}
		}

		file.close();
	}
}