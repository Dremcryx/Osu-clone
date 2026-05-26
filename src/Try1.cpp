#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <cmath>

enum class GameState
{
	MainMenu,
	GameMenu,
	Playing,
	Pause

};

class Button{
private:
	sf::CircleShape mmcircle;
	sf::Text mmtext;
	
public:
	Button(const sf::Font& font): mmcircle( 180.f ) , mmtext(font)
	{
		//Main menu circle
		
		mmcircle.setFillColor( sf::Color::Blue );
		mmcircle.setOutlineColor( sf::Color(211, 211, 211));
		mmcircle.setOutlineThickness (20.f);
		mmcircle.setOrigin({180.f, 180.f});
		mmcircle.setPosition({640.f,360.f});
			
		//Main menu text
		mmtext.setString ("TEST");
		mmtext.setCharacterSize (100.f);
		mmtext.setFillColor(sf::Color::White);
		mmtext.setStyle(sf::Text::Bold );
		
		//Center text
		sf::FloatRect textRect = mmtext.getGlobalBounds();
		mmtext.setOrigin({textRect.size.x / 2.0f, (textRect.size.y / 2.0f) + 25.f});
		mmtext.setPosition({640.f,360.f});

	}
	
	bool updateAndCheckClick(sf::Vector2f worldPos) {
        sf::Vector2f delta = worldPos - mmcircle.getPosition();
        float distanceSquared = (delta.x * delta.x) + (delta.y * delta.y);
        float radiusSquared = mmcircle.getRadius() * mmcircle.getRadius();

        if (distanceSquared <= radiusSquared) {
            mmcircle.setFillColor(sf::Color(0, 0, 200));
			mmcircle.setRadius(200.f);
			mmcircle.setOrigin({200.f, 200.f});
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                return true;
            }
        } 
		else {
            mmcircle.setFillColor(sf::Color::Blue);
			mmcircle.setRadius(180.f);
			mmcircle.setOrigin({180.f, 180.f});
        }
        return false;
	}	
	

	void draw(sf::RenderWindow& window) {
		window.draw( mmcircle );
		window.draw( mmtext );
	}
	
};	
class MainMenu{
private:
	sf::Font font;
	std::unique_ptr<Button> m_testButton;

public:
	MainMenu(){
		//Create text for main menu
		if (!font.openFromFile("arial.ttf"))
		{
			// error...
		}
		m_testButton = std::make_unique<Button>(font); 

	}
	void update(sf::Vector2f worldPos, sf::RenderWindow& window) {
        if (m_testButton->updateAndCheckClick(worldPos)) {
            
        }
    }	

	void draw(sf::RenderWindow& window) {
        m_testButton->draw(window);
	}


};
class GameMenu{
private:

public:
	GameMenu(){




		
	}




};

int main()
{
	//Render the main window
	sf::RenderWindow window( sf::VideoMode( { 1280, 720 } ), "SFML works!" , sf::Style::Titlebar | sf::Style::Close );
    //hide mouse cursor
	window.setMouseCursorVisible(false);

	//Load main menu
    MainMenu mainMenu;
	
	sf::CircleShape cursor(20.f);
	cursor.setOrigin({20.f,20.f});

	//Keep window open
	while ( window.isOpen() )
	{
		// Get mouse position and convert
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		//Get mouse position relative to window
		
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		int mouseX = mousePos.x;
        int mouseY = mousePos.y;


		//draw custom cursor at mouse position
		mainMenu.update(worldPos, window);
		cursor.setPosition(static_cast<sf::Vector2f>(mousePos));
		
	
		window.clear(sf::Color::Black);
		
		mainMenu.draw(window);
		window.draw(cursor);

		window.display();
	}
}