#include "trainer.h"
#include <fstream>
#include <iostream>

void Trainer::run()
{	
	std::fstream namesFile;
	namesFile.open("names.txt", std::ios::in);
	for (auto& name : objNames)
	{
		getline(namesFile, name);
	}
	namesFile.close();

	processRaw();

	ai = new Ai(std::vector<int>{1024, 1500, 700, 10});
	ai->setRandomValues();

	while (true)
	{
		fillBatch();
		ai->learn(batch, 0.08);
		fillBatch(true);
		std::cout << iteration << ", " << ai->cost(batch) << ", ";
		int correct = 0;
		for (auto text : batch)
		{
			std::vector<double> outs = ai->calculateOutput(text.inputs);
			int pos = 0;
			int exp = 0;
			double value = outs[0];
			for (int out = 1; out < 10; out++)
			{
				if (text.targets[out] == 1)
					exp = out;
				
				if (outs[out] > value)
				{
					value = outs[out];
					pos = out;
				}
			}

			if (pos == exp)
				correct++;
		}
		std::cout << correct << "/40\n";
		iteration++;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			ai->save();
		}
	}
}

void Trainer::processRaw()
{
	for (auto name : objNames)
	{
		std::fstream raw;
		raw.open("raw/" + name + ".txt", std::ios::in);

		if (!raw.good())
			break;

		std::cout << "processing " << name << "\n";

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

		shortened.close();
		raw.close();
	}
}
void Trainer::fillBatch(bool test)
{
	batch.clear();
	if (!test)
	{
		for (int obj = 0; obj < 10; obj++)
		{		
			std::fstream file;
			file.open("shortened/" + objNames[obj] + ".txt", std::ios::in);
			for (int nImage = 0; nImage < 4; nImage++)
			{		
				std::string line;
				for (int skip = 0; skip < Layer::random(0, 200); skip++)
					getline(file, line);

				sf::Image image;
				image.create(32, 32, sf::Color::White);
				char div = ' ';
				while (div != ';')
				{
					std::vector<float> strokeX;
					std::vector<float> strokeY;
					int num;
					file >> div;
					file >> div;

					sf::Vector2i value;
					while (div != ']')
					{
						file >> num;
						strokeX.push_back(num / 8.f);
						file >> div;
					}
					file >> div;
					file >> div;

					while (div != ']')
					{
						file >> num;
						strokeY.push_back(num / 8.f);
						file >> div;
					}

					file >> div;
					file >> div;

					for (int point = 0; point < strokeX.size() - 1; point++)
						drawLine(image, strokeX[point], strokeY[point], strokeX[point + 1], strokeY[point + 1], false);
				}

				DataPoint datapoint;
				for (int x = 0; x < 32; x++)
				{
					for (int y = 0; y < 32; y++)
					{
						datapoint.inputs.push_back(image.getPixel(x, y).r / 255.f);
					}
				}
				for (int tar = 0; tar < 10; tar++)
				{
					if (tar == obj)
						datapoint.targets.push_back(1);
					else
						datapoint.targets.push_back(0);
				}

				batch.push_back(datapoint);
			}
			file.close();
		}
	}
	else
	{
		for (int obj = 0; obj < 10; obj++)
		{
			std::fstream file;
			file.open("shortened/" + objNames[obj] + ".txt", std::ios::in);

			std::string line;
			for (int skip = 0; skip < 800; skip++)
				getline(file, line);
			
			for (int nImage = 0; nImage < 4; nImage++)
			{
				std::string line;
				for (int skip = 0; skip < Layer::random(0, 50); skip++)
					getline(file, line);

				sf::Image image;
				image.create(32, 32, sf::Color::White);
				char div = ' ';
				while (div != ';')
				{
					std::vector<float> strokeX;
					std::vector<float> strokeY;
					int num;
					file >> div;
					file >> div;

					sf::Vector2i value;
					while (div != ']')
					{
						file >> num;
						strokeX.push_back(num / 8.f);
						file >> div;
					}
					file >> div;
					file >> div;

					while (div != ']')
					{
						file >> num;
						strokeY.push_back(num / 8.f);
						file >> div;
					}

					file >> div;
					file >> div;

					for (int point = 0; point < strokeX.size() - 1; point++)
						drawLine(image, strokeX[point], strokeY[point], strokeX[point + 1], strokeY[point + 1], false);
				}

				DataPoint datapoint;
				for (int x = 0; x < 32; x++)
				{
					for (int y = 0; y < 32; y++)
					{
						datapoint.inputs.push_back(image.getPixel(x, y).r / 255.f);
					}
				}
				for (int tar = 0; tar < 10; tar++)
				{
					if (tar == obj)
						datapoint.targets.push_back(1);
					else
						datapoint.targets.push_back(0);
				}

				batch.push_back(datapoint);
			}
			file.close();
		}
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