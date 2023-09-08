#include "app.h"
#include <fstream>
#include <iostream>

App::App()
{
	extractData();
	lastPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	drawingImage.create(imageSize, imageSize, sf::Color::White);
	texture.loadFromImage(drawingImage);
	bg.setSize(sf::Vector2f(32, 32));
	bg.setScale(31, 31);
	bg.setOrigin(bg.getLocalBounds().width / 2, bg.getLocalBounds().height / 2);
	bg.setPosition(bg.getGlobalBounds().width / 2 + (viewSize.y - imageSize * 31) / 2, viewSize.y / 2);
	bg.setFillColor(sf::Color(10, 30, 50));
	sprite.setTexture(texture);
	sprite.setScale(28, 28);
	sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
	sprite.setPosition(sprite.getGlobalBounds().width / 2 + (viewSize.y - imageSize*28)/2, viewSize.y / 2);
	font.loadFromFile("resources/font.ttf");
	text.setFont(font);
	text.setCharacterSize(55);
}

void App::run()
{	
	window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 2 / 3, sf::VideoMode::getDesktopMode().width * 3 / 8), "Doodles recognition");
	window.setView(sf::View(sf::Vector2f(viewSize.x / 2, viewSize.y / 2), viewSize));
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Space)
				{
					drawingImage.create(imageSize, imageSize, sf::Color::White);
					texture.loadFromImage(drawingImage);
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			if (canFullscreen)
			{
				if (window.getSize().x == sf::VideoMode::getDesktopMode().width)
				{
					window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 2 / 3, sf::VideoMode::getDesktopMode().width * 3 / 8), "Doodles recognition");
					window.setView(sf::View(sf::Vector2f(viewSize.x / 2, viewSize.y / 2), viewSize));
				}
				else
				{
					window.create(sf::VideoMode::getDesktopMode(), "Doodles recognition", sf::Style::Fullscreen);
					window.setView(sf::View(sf::Vector2f(viewSize.x / 2, viewSize.y / 2), viewSize));
				}
			}
			canFullscreen = false;
		}
		else
		{
			canFullscreen = true;
		}

		update();

		draw();
	}
}

void App::update()
{
	bool left = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool right = sf::Mouse::isButtonPressed(sf::Mouse::Right);
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	if ((left || right) && window.hasFocus() && sprite.getGlobalBounds().contains(mousePos))
	{
		sf::Vector2f imagePos((mousePos.x - 92.f) / 896.f * imageSize, (mousePos.y - 92.f) / 896.f * imageSize);
		sf::Vector2f lastImagePos((lastPos.x - 92.f) / 896.f * imageSize, (lastPos.y - 92.f) / 896.f * imageSize);

		if (mousePos != lastPos)
			drawLine(drawingImage, imagePos.x, imagePos.y, lastImagePos.x, lastImagePos.y, right);
		else
			drawPixel(drawingImage, imagePos.x, imagePos.y, right);

		texture.loadFromImage(drawingImage);
	}
	lastPos = mousePos;
}

void App::draw()
{		
	window.clear(sf::Color(15, 40, 70));

	window.draw(bg);
	window.draw(sprite);
	for (int i = 0; i < 10; i++)
	{
		text.setString(objNames[i][0]);
		text.setPosition(1400, viewSize.y / 11 * (i + 1) - 20);
		text.setOrigin(0, text.getGlobalBounds().height / 2);
		window.draw(text);
	}

	window.display();
}

void App::extractData()
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
			line.erase(line.end() - 1);
			line.erase(line.end() - 1);
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

	//convert data to image and save it to file
	for (int i = 0; i < 10; i++)
	{
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
				drawLine(image, strokeX[point], strokeY[point], strokeX[point+1], strokeY[point+1], false);
		}

		image.saveToFile("C:/Users/feder/Downloads/but/image" + std::to_string(i) + ".png");
	}

	file.close();
}

void App::drawLine(sf::Image& image, float x1, float y1, float x2, float y2, bool erase)
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

void App::drawPixel(sf::Image& image, float x, float y, bool erase)
{
	auto cap = [this](float pos)
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