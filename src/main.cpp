#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

/*
int main()
{
	

	sf::RenderWindow window(sf::VideoMode({640, 480}), "Game" ,sf::Style::Titlebar | sf::Style::Close  );
	window.setFramerateLimit(30);

	// Force an immediate initial clear to black before the loop starts
    window.clear(sf::Color::Black);
    window.display();

	//Game loop
	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
			}
        }       
	}

	//Render 

	window.clear(); //clear old frame
	window.display(); //draw new frame

return 0;
}
*/