#include "app.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

App::App()
{

}

int App::run()
{
	/*std::fstream file("data/train.csv");
	
	for (int i = 0; i < 0; ++i) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	for (int nImg = 0; nImg < 5; nImg++)
	{
		std::string line;
		getline(file, line);

		int target = std::stoi(std::string(1, line[0]));
		line.erase(0, 1);
		std::vector<int> targets {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		targets[target] = 1;

		std::vector<int> data;
		for (int pixel = 0; pixel < 784; pixel++)
		{
			line.erase(0, 1);
			std::string dataStr = "";
			while (line != "" && line[0] != ',')
			{
				dataStr = dataStr + line[0];
				line.erase(0, 1);
			}
			data.push_back(stoi(dataStr));
		}

		DataPoint datapoint(data, targets);

		sf::Image image;
		image.create(28, 28);
		for (int y = 0; y < 28; y++)
		{
			for (int x = 0; x < 28; x++)
			{
				int c = datapoint.data[y * 28 + x];
				image.setPixel(x, y, sf::Color(c, c, c));
			}
		}
		image.saveToFile("images/image" + std::to_string(nImg) + ".png");
	}*/

	Ai ai({ 784, 30, 10 });

	while (true)
	{

	}

	return 0;
}
