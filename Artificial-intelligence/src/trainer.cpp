#include "trainer.h"
#include <fstream>
#include <iostream>
#include <filesystem>

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

	ai = new Ai(std::vector<int>{784, 30, 10});
	ai->setRandomValues();

	while (true)
	{		
		std::vector<DataPoint> trainBatch;
		fillBatch(trainBatch);
		ai->learn(trainBatch, 0.1);

		std::vector<DataPoint> testBatch;
		fillBatch(testBatch, true);
		std::cout << iteration << ", test cost: " << ai->cost(testBatch) << ", ";

		int correct = 0;
		for (auto test : testBatch)
		{
			std::vector<double> outs = ai->calculateOutput(test.inputs);
			int pos = 0;
			int exp = 0;
			double value = outs[0];
			for (int out = 1; out < 10; out++)
			{
				if (test.targets[out] == 1)
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
		std::cout << "test correctness: " << correct << "/" << testBatch.size() << "\n";
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

		//format the file and eliminate troll ones
		for (int i = 0; i < 1000; i++)
		{		
			std::fstream separated;
			std::filesystem::create_directories("separated/" + name);
			separated.open("separated/" + name + "/" + name + std::to_string(i) + ".txt", std::ios::out | std::ios::trunc);

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

				separated << line + "\n";
			}
			else
			{
				i--;
			}
			separated.close();
		}

		raw.close();
	}
}
void Trainer::fillBatch(std::vector<DataPoint>& batch, bool test)
{
	batch.clear();
	for (int obj = 0; obj < 10; obj++)
	{		
		for (int nImage = 0; nImage < 10; nImage++)
		{	
			int n;
			if (!test)
				n = Layer::random(0, 799);
			else
				n = Layer::random(800, 999);

			//transfer info from file to image
			std::fstream file;
			file.open("separated/" + objNames[obj] + "/" + objNames[obj] + std::to_string(n) + ".txt", std::ios::in);
			
			sf::Image image;
			image.create(imageSize, imageSize, sf::Color::White);
			char div = ' ';
			//each loop iteration is a different stroke
			while (div != ';')
			{
				std::vector<float> strokeX;
				std::vector<float> strokeY;
				int num;
				file >> div;
				file >> div;

				//take all x info
				while (div != ']')
				{
					file >> num;
					strokeX.push_back(num / 256.f * imageSize);
					file >> div;
				}
				file >> div;
				file >> div;

				//take all y info
				while (div != ']')
				{
					file >> num;
					strokeY.push_back(num / 256.f * imageSize);
					file >> div;
				}

				file >> div;
				file >> div;

				for (int point = 0; point < strokeX.size() - 1; point++)
					drawLine(image, strokeX[point], strokeY[point], strokeX[point + 1], strokeY[point + 1], false);
			}
			file.close();

			DataPoint datapoint;
			fillDatapoint(datapoint, image, obj);
			batch.push_back(datapoint);
		}
	}
}
void Trainer::fillDatapoint(DataPoint& datapoint, sf::Image image, int obj)
{
	for (int x = 0; x < imageSize; x++)
	{
		for (int y = 0; y < imageSize; y++)
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

		float brightness = 120;
		float strenght = 0;//std::max(abs(int(x) + 0.5 - x), abs(int(y) + 0.5 - y)) * brightness;
		if (image.getPixel(cap(x), cap(y)).b > strenght)
			image.setPixel(cap(x), cap(y), sf::Color(strenght, strenght, strenght));
		if (xOff != 0)
		{
			//strenght = std::max(abs(int(x + xOff) + 0.5 - x), abs(int(y) + 0.5 - y)) * brightness;
			if (image.getPixel(cap(x + xOff), cap(y)).b > strenght)
				image.setPixel(cap(x + xOff), cap(y), sf::Color(strenght, strenght, strenght));

			if (yOff != 0)
			{
				//strenght = std::max(abs(int(x + xOff) + 0.5 - x), abs(int(y + yOff) + 0.5 - y)) * brightness;
				if (image.getPixel(cap(x + xOff), cap(y + yOff)).b > strenght)
					image.setPixel(cap(x + xOff), cap(y + yOff), sf::Color(strenght, strenght, strenght));
			}
		}
		if (yOff != 0)
		{
			//strenght = std::max(abs(int(x) + 0.5 - x), abs(int(y + yOff) + 0.5 - y)) * brightness;
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