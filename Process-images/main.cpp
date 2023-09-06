#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

void simulateStroke(sf::Image& image, std::vector<int> strokeX, std::vector<int> strokeY);

int main()
{
	std::fstream raw;
	raw.open("C:/Users/feder/Downloads/full_simplified_butterfly.txt", std::ios::in);

	std::fstream file;
	file.open("C:/Users/feder/Downloads/butterfly.txt", std::ios::out | std::ios::trunc);

	for (int i = 0; i < 16000; i++)
	{
		std::string line;
		getline(raw, line);
		while (line.at(0) != '[')
		{
			line.erase(line.begin());
		}
		line.erase(line.begin());
		line.erase(line.end()-1);
		line.erase(line.end()-1);
		line.append(";");

		file << line + "\n";
	}

	raw.close();
	file.close();
	file.open("C:/Users/feder/Downloads/butterfly.txt", std::ios::in);

	for (int i = 0; i < 1; i++)
	{
		sf::Image image;
		image.create(256, 256, sf::Color::White);

		std::vector<int> strokeX;
		std::vector<int> strokeY;
		int num;
		char div;
		file >> div;
		file >> div;
		
		sf::Vector2i value;
		while (div != ']')
		{
			file >> num;
			strokeX.push_back(num);
			file >> div;
		}
		file >> div; 
		file >> div;

		while (div != ']')
		{
			file >> num;
			strokeY.push_back(num);
			file >> div;
		}

		simulateStroke(image, strokeX, strokeY);
		image.saveToFile("image.png");
	}

	file.close();
	return 0;
}

void simulateStroke(sf::Image& image, std::vector<int> strokeX, std::vector<int> strokeY)
{
	for (int i = 0; i < strokeX.size(); i++)
	{
		image.setPixel(strokeX[i], strokeY[i], sf::Color::Black);
	}
}