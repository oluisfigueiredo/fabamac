#include "Platform/Platform.hpp"
#define BALLNUMBER 50

int addCredits(int credits)
{
	std::cout << credits << std::endl;
	return ++credits;
}

int removeCredits(int credits)
{
	std::cout << credits << std::endl;
	return --credits;
}

int resetCredits()
{
	return 0;
}

int saveCredits(int credits, int savedCredits)
{
	int totalCredits = savedCredits + credits;
	return totalCredits;
}

int addPlays(int plays)
{
	std::cout << plays << std::endl;
	return ++plays;
}

int main()
{
	util::Platform platform;
	sf::Clock clock;
	sf::Clock TimerClock;
	sf::Time ballShotTimer = sf::milliseconds(100);
	sf::Time elapsed;

	float dt;
	const float movementSpeed = 1000.f;
	sf::Vector2f velocity;

	//sf::RenderWindow window;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Fabamaq Mini Game");

	sf::Texture ballTexture;
	if (!ballTexture.loadFromFile("content/ball.png"))
	{
		std::cout << "Load Failed!" << std::endl;
		system("pause");
	}

	//int ballPositionX = 0;

	sf::Sprite ballArray[BALLNUMBER+1];
	int cutx = 217, cuty = 217, cutoffset = 0;
	int isMultipleTen = 0;

	//creating 50 balls
	for (int i = 0; i < BALLNUMBER+1; i++)
	{
		if (i % 10 == 0 && i !=0)
		{
			// change to next color
			cutoffset = cutoffset + cutx;
			isMultipleTen = 0;
		}

		sf::Sprite ballSprite;
		ballSprite.setTexture(ballTexture);
		ballSprite.setTextureRect(sf::IntRect(cutoffset, 0, cutx, cuty));
		ballSprite.scale(0.369f, 0.369f);
		ballSprite.setPosition(-cutx, int(i / 10) * 0.369 * cuty);

		ballArray[i] = ballSprite;
		isMultipleTen++;
	}

	sf::Font scoreFont;
	if (!scoreFont.loadFromFile("content/Roguedash-Solid.ttf"))
	{
		std::cout << "Load Failed!" << std::endl;
		system("pause");
	}

	sf::Text scoreText;
	scoreText.setFont(scoreFont);
	scoreText.setString("GAME PLAYS");
	scoreText.setPosition(0, 480);

	sf::Text creditsInText;
	creditsInText.setFont(scoreFont);
	creditsInText.setString("CREDITS IN ");
	creditsInText.setPosition(0, 510);

	sf::Text creditsOutText;
	creditsOutText.setFont(scoreFont);
	creditsOutText.setString("CREDITS OUT ");
	creditsOutText.setPosition(0, 540);

	sf::Text playsText;
	playsText.setFont(scoreFont);
	playsText.setPosition(150, 480);

	sf::Text creditsText;
	creditsText.setFont(scoreFont);
	creditsText.setPosition(150, 510);

	sf::Text savedCredText;
	savedCredText.setFont(scoreFont);
	savedCredText.setPosition(150, 540);

	sf::Event event;
	int creditsValue = 0;
	int savedCreditsValue = 0;
	int playsValue = 0;
	int ballShot = 0;
	bool duringAnimation = false;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
				creditsValue = addCredits(creditsValue);

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z)
			{
				savedCreditsValue = saveCredits(creditsValue, savedCreditsValue);
				creditsValue = resetCredits();
			}

			if (event.type == sf::Event::KeyPressed && creditsValue > 0 && event.key.code == sf::Keyboard::S)
			{
				if (duringAnimation)
				{
					duringAnimation = false;
				}
				else
				{
					duringAnimation = true;
					if (event.type == sf::Event::KeyPressed && creditsValue > 0 && event.key.code == sf::Keyboard::S){
						window.waitEvent(event);
					}


				}
				for (int i = 0; i < BALLNUMBER+1; i++)
				{
					if (i % 10 == 0)
					{
						isMultipleTen = 0;
					}
					ballArray[i].setPosition(-cutx - 100, int(i / 10) * 0.369 * cuty);
					isMultipleTen++;
				}
				playsValue = addPlays(playsValue);
				creditsValue = removeCredits(creditsValue);
			}
		}

		creditsText.setString(std::to_string(creditsValue));
		savedCredText.setString(std::to_string(savedCreditsValue));
		playsText.setString(std::to_string(playsValue));

		dt = clock.restart().asSeconds();
		velocity.y = 0.f;
		velocity.x = movementSpeed * dt;
		int isMultipleTen = 0;

		elapsed = TimerClock.getElapsedTime();
		///////// MOVEMENT //////////
		for (int i = 0; i <= ballShot && duringAnimation; i++)
		{
			if (i % 10 == 0)
			{
				// change to next color
				isMultipleTen = 0;
			}

			ballArray[i].move(velocity);
			if (ballArray[i].getPosition().x + ballArray[i].getGlobalBounds().width > 800 - (isMultipleTen * 80))
			{
				ballArray[i].setPosition(800 - (isMultipleTen * 80) - ballArray[i].getGlobalBounds().width, ballArray[i].getPosition().y);
			}

			isMultipleTen++;

			if (ballShotTimer.asMilliseconds() < elapsed.asMilliseconds())
			{
				std::cout << "Shooting ball: " << ballShot << std::endl;
				ballShot++;
				if (ballShot == BALLNUMBER+3)
				{
					ballShot = 0;
					duringAnimation = false;
				}
				TimerClock.restart();
				break;
			}
		}

		window.clear();
		for (int i = 0; i < BALLNUMBER+1; i++)
		{
			window.draw(ballArray[i]);
		}
		window.draw(scoreText);
		window.draw(creditsInText);
		window.draw(creditsOutText);
		window.draw(creditsText);
		window.draw(savedCredText);
		window.draw(playsText);
		window.display();
	}

	return 0;
}