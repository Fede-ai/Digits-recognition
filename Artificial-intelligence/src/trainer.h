#pragma once
#include "SFML/Graphics.hpp"

class Trainer
{
public:
	Trainer();
	void run();

private:
	void processImage();

	static void drawPixel(sf::Image& image, float x, float y, bool erase);
	static void drawLine(sf::Image& image, float x1, float y1, float x2, float y2, bool erase);

	static constexpr int imageSize = 32;

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