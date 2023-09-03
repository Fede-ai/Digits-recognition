#include "app.h"
#include <fstream>

App::App()
{
	std::fstream file;
	file.open("info.txt", std::ios::in | std::ios::out);
	std::string line;	
	getline(file, line);
	iteration = std::stoi(line);
	line.clear();
	getline(file, line);
	currentObj = std::stoi(line);
	file.close();

	image.create(imageSize, imageSize, sf::Color::White);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setScale(viewSize.y / imageSize, viewSize.y / imageSize);
	sprite.setPosition((viewSize.x - viewSize.y) / 2, 0);
	font.loadFromFile("resources/font.ttf");
	text.setFont(font);
	text.setCharacterSize(30);
}

void App::run()
{	
	window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 2 / 3, sf::VideoMode::getDesktopMode().width * 3 / 8), "Data creation");
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
			}
		}

		update();

		draw();
	}
}

void App::update()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		if (sprite.getGlobalBounds().contains(mousePos))
		{
			sf::Vector2i imagePos((mousePos.x - (viewSize.x - viewSize.y) / 2) / (viewSize.y / imageSize), mousePos.y / (viewSize.y / imageSize));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				image.setPixel(imagePos.x, imagePos.y, sf::Color::Black);
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				image.setPixel(imagePos.x, imagePos.y, sf::Color::White);
			texture.loadFromImage(image);
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		if (canFinish)
		{			
			image.saveToFile("data/" + objNames[currentObj][1] + "/" + objNames[currentObj][1] + std::to_string(iteration) + ".png");
			image.create(imageSize, imageSize, sf::Color::White);
			texture.loadFromImage(image);

			if (currentObj == 9)
				iteration++;
			currentObj = (currentObj + 1) % 10;
			canFinish = false;
			write();
		}
	}
	else
	{
		canFinish = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
	{
		if (canDelete)
		{
			currentObj = std::max(currentObj - 1, 0);
			canDelete = false;
			write();
		}
	}
	else
	{
		canDelete = true;
	}
}

void App::draw()
{		
	window.clear(sf::Color(180, 180, 180));

	window.draw(sprite);
	for (int i = 0; i < 10; i++)
	{
		text.setString(objNames[i][0]);
		text.setPosition((viewSize.x - viewSize.y) / 4, viewSize.y / 11 * (i + 1) - 10);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);

		if (i == currentObj)
			text.setFillColor(sf::Color::Yellow);
		else if (i < currentObj)
			text.setFillColor(sf::Color::Green);
		else
			text.setFillColor(sf::Color::Red);

		window.draw(text);
	}

	text.setPosition(viewSize.y + (viewSize.x - viewSize.y) / 4 * 3, 100);
	text.setString("iteration n. " + std::to_string(iteration + 1));
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setFillColor(sf::Color::Black);
	window.draw(text);

	window.display();
}

void App::write()
{
	std::fstream file;
	file.open("info.txt", std::ios::in | std::ios::out);
	file.clear();
	file << iteration << "\n";
	file << currentObj;
	file.close();
}
