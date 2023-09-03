#pragma once
#include "SFML/Graphics.hpp"

class App
{
public:
	App();
	void run();

private:
	void update();
	void draw();
	void write();

	sf::Vector2f viewSize = sf::Vector2f(1920, 1080);
	sf::RenderWindow window;

	bool canFinish = false;
	bool canDelete = false;

	const int imageSize = 30;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Font font;
	sf::Text text;

	int iteration = 0;
	int currentObj = 0;
	const std::string objNames[10][2] = {
		{"mulino a vento",	"mul"},
		{"casa",			"cas"},
		{"lavatrice",		"lav"},
		{"forchetta",		"for"},
		{"coltello",		"col"},
		{"macchina",		"mac"},
		{"bicicletta",		"bic"},
		{"barca",			"bar"},
		{"aeroplano",		"aer"},
		{"elicottero",		"eli"}
	};
};