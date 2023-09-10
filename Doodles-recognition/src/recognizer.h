#pragma once
#include "SFML/Graphics.hpp"
#include "ai.h"

class Recognizer
{
public:
	Recognizer();
	void run();

private:
	void update();
	void draw();

	void drawLine(sf::Image& image, float x1, float y1, float x2, float y2, bool erase);
	void drawPixel(sf::Image& image, float x, float y, bool erase);

	void updatePredictions();

	sf::Vector2f viewSize = sf::Vector2f(1920, 1080);
	sf::RenderWindow window;
	bool canFullscreen = false;
	sf::Vector2f lastPos;

	static constexpr int imageSize = 32;
	sf::Image drawingImage;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape bg;
	sf::Font font;
	sf::Text text;

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

	Ai ai;
	bool failed;
	std::vector<double> out;
};