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

	//format the file and eliminate troll ones
	for (int i = 0; i < 16000; i++)
	{
		std::string line;
		getline(raw, line);
		bool valid = true;
		int apost = 0;
		for (int pos = 0; pos < line.size(); pos++)
		{
			if (line.at(pos) == '"')
			{
				apost++;
				if (apost == 14)
				{
					valid = (line.at(pos + 2) == 't');
					break;
				}
			}
		}
		if (valid)
		{
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
		else
		{
			i--;
		}
	}

	raw.close();
	file.close();
	file.open("C:/Users/feder/Downloads/butterfly.txt", std::ios::in);

	for (int i = 0; i < 1; i++)
	{
		sf::Image image;
		image.create(256, 256, sf::Color::White);
		char div = ' ';

		while (div != ';')
		{
			std::vector<int> strokeX;
			std::vector<int> strokeY;
			int num;
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

			file >> div;		
			file >> div;
			
			simulateStroke(image, strokeX, strokeY);
		}

		image.saveToFile("C:/Users/feder/Downloads/but/image" + std::to_string(i) + ".png");
	}

	file.close();
	return 0;
}

void simulateStroke(sf::Image& image, std::vector<int> strokeX, std::vector<int> strokeY)
{
	for (int i = 0; i < strokeX.size() - 1; i++)
	{
		float num;
		if (abs(strokeX[i] - strokeX[i + 1]) > abs(strokeY[i] - strokeY[i + 1]))
			num = strokeX[i] - strokeX[i + 1];
		else
			num = strokeY[i] - strokeY[i + 1];

		for (int a = 0; a < abs(num); a++)
		{
			float per = float(a) / abs(num);
			image.setPixel(strokeX[i] + (strokeX[i + 1] - strokeX[i]) * per, strokeY[i] + (strokeY[i+1] - strokeY[i]) * per, sf::Color::Black);
		}
	}
}