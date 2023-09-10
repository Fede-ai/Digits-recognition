#pragma once
#include "SFML/Graphics.hpp"
#include "ai.h"

class Trainer
{
public:
	void run();

private:
	void processRaw();
	void fillBatch(bool test = false);

	static void drawPixel(sf::Image& image, float x, float y, bool erase);
	static void drawLine(sf::Image& image, float x1, float y1, float x2, float y2, bool erase);

	static constexpr int imageSize = 32;
	Ai* ai = nullptr;

	int iteration = 0;
	std::vector<DataPoint> batch;

	std::string objNames[10] = {
	{""},
	{""},
	{""},
	{""},
	{""},
	{""},
	{""},
	{""},
	{""},
	{""}
	};
};