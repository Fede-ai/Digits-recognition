#include "trainer.h"
#include <fstream>
#include <iostream>

Trainer::Trainer()
{
	std::fstream namesFile;
	namesFile.open("names.txt", std::ios::in);
	for (auto& name : objNames)
	{
		getline(namesFile, name);
	}
	namesFile.close();

	processImage();
}

void Trainer::run()
{
}

void Trainer::processImage()
{
	for (auto name : objNames)
	{
		std::fstream raw;
		raw.open("raw/" + name + ".txt", std::ios::in);

		std::fstream shortened;
		shortened.open("shortened/" + name + ".txt", std::ios::out | std::ios::trunc);

		//format the file and eliminate troll ones
		for (int i = 0; i < 10000; i++)
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
				line.erase(line.end() - 1);
				line.erase(line.end() - 1);
				line.append(";");

				shortened << line + "\n";
			}
			else
			{
				i--;
			}
		}

		raw.close();
		shortened.close();
		shortened.open("shortened/" + name + ".txt", std::ios::in);

		//convert data to image and save it to file
		for (int i = 0; i < 3; i++)
		{
			sf::Image image;
			image.create(32, 32, sf::Color::White);
			char div = ' ';

			while (div != ';')
			{
				std::vector<float> strokeX;
				std::vector<float> strokeY;
				int num;
				shortened >> div;
				shortened >> div;

				sf::Vector2i value;
				while (div != ']')
				{
					shortened >> num;
					strokeX.push_back(num / 8.f);
					shortened >> div;
				}
				shortened >> div;
				shortened >> div;

				while (div != ']')
				{
					shortened >> num;
					strokeY.push_back(num / 8.f);
					shortened >> div;
				}

				shortened >> div;
				shortened >> div;

				for (int point = 0; point < strokeX.size() - 1; point++)
					drawLine(image, strokeX[point], strokeY[point], strokeX[point + 1], strokeY[point + 1], false);
			}
		}

		shortened.close();
	}
}

void Trainer::drawLine(sf::Image& image, float x1, float y1, float x2, float y2, bool erase)
{
	float num;
	if (abs(x1 - x2) > abs(y1 - y2))
		num = x1 - x2;
	else
		num = y1 - y2;

	for (int a = 0; a < abs(num); a++)
	{
		float per = float(a) / abs(num);
		drawPixel(image, x1 + (x2 - x1) * per, y1 + (y2 - y1) * per, erase);
	}
}

void Trainer::drawPixel(sf::Image& image, float x, float y, bool erase)
{
	auto cap = [](float pos)
	{
		return std::min(std::max(pos, 0.f), (float)imageSize-1);
	};

	if (!erase)
	{
		int xOff = 0, yOff = 0;
		if (int(x) + 0.5 - x < -0.3)
			xOff = 1;
		else if (int(x) + 0.5 - x > 0.3)
			xOff = -1;
		if (int(y) + 0.5 - y < -0.3)
			yOff = 1;
		else if (int(y) + 0.5 - y > 0.3)
			yOff = -1;

		float brightness = 150;
		float strenght = std::max(abs(int(x) + 0.5 - x), abs(int(y) + 0.5 - y)) * brightness;
		if (image.getPixel(cap(x), cap(y)).b > strenght)
			image.setPixel(cap(x), cap(y), sf::Color(strenght, strenght, strenght));
		if (xOff != 0)
		{
			strenght = std::max(abs(int(x + xOff) + 0.5 - x), abs(int(y) + 0.5 - y)) * brightness;
			if (image.getPixel(cap(x + xOff), cap(y)).b > strenght)
				image.setPixel(cap(x + xOff), cap(y), sf::Color(strenght, strenght, strenght));

			if (yOff != 0)
			{
				strenght = std::max(abs(int(x + xOff) + 0.5 - x), abs(int(y + yOff) + 0.5 - y)) * brightness;
				if (image.getPixel(cap(x + xOff), cap(y + yOff)).b > strenght)
					image.setPixel(cap(x + xOff), cap(y + yOff), sf::Color(strenght, strenght, strenght));
			}
		}
		if (yOff != 0)
		{
			strenght = std::max(abs(int(x) + 0.5 - x), abs(int(y + yOff) + 0.5 - y)) * brightness;
			if (image.getPixel(cap(x), cap(y + yOff)).b > strenght)
				image.setPixel(cap(x), cap(y + yOff), sf::Color(strenght, strenght, strenght));
		}
	}
	else
	{
		int xOff = 0, yOff = 0;
		if (int(x) + 0.5 - x > 0)
			xOff = -1;
		else
			xOff = 1;

		if (int(y) + 0.5 - y > 0)
			yOff = -1;
		else
			yOff = 1;

		image.setPixel(cap(x), cap(y), sf::Color::White);
		image.setPixel(cap(x + xOff), cap(y), sf::Color::White);
		image.setPixel(cap(x), cap(y + yOff), sf::Color::White);
		image.setPixel(cap(x + xOff), cap(y + yOff), sf::Color::White);
	}
}