#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include <cmath>
#include <vector>
#include <cstdlib>

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
	//Constructor
	Button(const sf::Font& font): mmcircle( 180.f ) , mmtext(font)
	{
		//Main menu circle
		
		mmcircle.setFillColor( sf::Color::Blue );
		mmcircle.setOutlineColor( sf::Color(211, 211, 211));
		mmcircle.setOutlineThickness (20.f);
		mmcircle.setOrigin({180.f, 180.f});
		mmcircle.setPosition({640.f,360.f});
			
		//Main menu text
		mmtext.setString ("OSU!");
		mmtext.setCharacterSize (100.f);
		mmtext.setFillColor(sf::Color(255, 192, 203));
		mmtext.setStyle(sf::Text::Bold );
		
		//Center text
		sf::FloatRect textRect = mmtext.getGlobalBounds();
		mmtext.setOrigin({textRect.size.x / 2.0f, (textRect.size.y / 2.0f) + 25.f});
		mmtext.setPosition({640.f,360.f});

	}
	
	void updateHoverState(sf::Vector2f worldPos) {
        sf::Vector2f delta = worldPos - mmcircle.getPosition();
        float distanceSquared = (delta.x * delta.x) + (delta.y * delta.y);
        float radiusSquared = mmcircle.getRadius() * mmcircle.getRadius();


		//If mouse is over the button, Circle changes color and size
        if (distanceSquared <= radiusSquared) {
            mmcircle.setFillColor(sf::Color(0, 0, 200));
			mmcircle.setRadius(200.f);
			mmcircle.setOrigin({200.f, 200.f});
        } 
		else {
            mmcircle.setFillColor(sf::Color::Blue);
			mmcircle.setRadius(180.f);
			mmcircle.setOrigin({180.f, 180.f});
        }  
	}

	bool isClicked(sf::Vector2f worldPos) {
        sf::Vector2f delta = worldPos - mmcircle.getPosition();
        float distanceSquared = (delta.x * delta.x) + (delta.y * delta.y);
        float radiusSquared = mmcircle.getRadius() * mmcircle.getRadius();

        return (distanceSquared <= radiusSquared);

	}
	
	void draw(sf::RenderWindow& window) {
        window.draw(mmcircle);
        window.draw(mmtext);
	}	
	
};	
class MainMenu{
private:
	
	std::unique_ptr<Button> m_testButton;

public:
	MainMenu(const sf::Font& globalfont){
		m_testButton = std::make_unique<Button>(globalfont); 
	}
	void update(sf::Vector2f worldPos) {
        m_testButton->updateHoverState(worldPos);			
    }
		   	
	bool handleMouseButtonPressed(sf::Vector2f worldPos) {
        return m_testButton->isClicked(worldPos);
    }
	
	void draw(sf::RenderWindow& window) {
        m_testButton->draw(window);
	}


};
class GameMenu{
private:
	sf::RectangleShape LevelSelect;
	sf::Text Maptext;

public:
	
	GameMenu(const sf::Font& globalfont, sf::Vector2f position, std::string MapName): LevelSelect({600.f,100.f}), Maptext(globalfont)
	{
		LevelSelect.setFillColor(sf::Color(0, 102, 255));
		LevelSelect.setOutlineColor(sf::Color::White);
		LevelSelect.setOutlineThickness(-10.f);
		LevelSelect.setOrigin({300.f,50.f});
		LevelSelect.setPosition(position);

		Maptext.setString (MapName);
		Maptext.setCharacterSize (45.f);
		Maptext.setFillColor(sf::Color::Black);
		Maptext.setOrigin({300.f,50.f});
		Maptext.setPosition({position.x + 250.f, position.y + 20.f});
		
	}


	//Check if click happens in LevelSelect
	bool LevelClicked(sf::Vector2f worldPos) {
        return LevelSelect.getGlobalBounds().contains(worldPos);
	}

	void draw(sf::RenderWindow& window) {
        window.draw(LevelSelect);
        window.draw(Maptext);
	}	
};
class Map{
private:

	std::vector<sf::CircleShape> circlesMap;
	sf::Clock clock;

	//Create timer for spawning circles
	float timeSinceLastSpawn;
	float spawnInterval;
	float circlesize;


	sf::RectangleShape PlayZone;

	//Fixed Play zone
	float boxWidth = 650.f;
    float boxHeight = 650.f;
	//Create Offset var
	float boxleft;
	float boxtop;

	bool isFirstFrame;

public:
	//Load config
	Map(float difficulty_speed, float difficulty_size, sf::RenderWindow& window) :
		timeSinceLastSpawn(-1.f), 
		spawnInterval(difficulty_speed),
		circlesize(difficulty_size),
		isFirstFrame(true)
	{
		clock.restart();

		//Create offset
		boxleft = ((float(window.getSize().x)) - boxWidth) / 2.f;
		boxtop = ((float(window.getSize().y)) - boxHeight) / 2.f;
		
		PlayZone.setSize({boxWidth,boxHeight});
		PlayZone.setPosition({boxleft,boxtop});
		PlayZone.setFillColor(sf::Color::Transparent);
		PlayZone.setOutlineColor(sf::Color(255, 255, 255, 100));
		PlayZone.setOutlineThickness(3.f);

	}

	void update(sf::RenderWindow& window)
	{
		
		if (isFirstFrame) {
            clock.restart();
            isFirstFrame = false;
            return;
		}
		
		float dt = clock.restart().asSeconds();
		timeSinceLastSpawn += dt;

		//Circle spawner
		if (timeSinceLastSpawn >= spawnInterval)
		{
			//Create circle based on difficulty
			sf::CircleShape hitcircle(circlesize);
			hitcircle.setFillColor(sf::Color::Blue);
			hitcircle.setOutlineThickness(-5.f);

			//Make sure circle stays inside box
			int maxrangeX = boxWidth - (circlesize * 2.f);
        	int maxrangeY = boxHeight - (circlesize * 2.f);

			//Randomly generate position of circle
			float posX = boxleft + float(rand() % maxrangeX);
			float posY = boxtop + float(rand() % maxrangeY);

			hitcircle.setPosition({posX, posY});

			//Store inside vector
			circlesMap.push_back(hitcircle);

			//Reset timer
			timeSinceLastSpawn -= spawnInterval;

		}	
	}
	
	//Check for clicks (Later)
	std::vector<sf::CircleShape>& getCircles() {
		return circlesMap;
	}	

	//Check for hitcircle clicks
	bool checkcirclehit(sf::Vector2f worldPos){
		for(int i = static_cast<int>(circlesMap.size()) - 1; i >= 0; --i)
		{
			if(circlesMap[i].getGlobalBounds().contains(worldPos)){
				circlesMap.erase (circlesMap.begin() + i);
				return true;
			}
		}
		return false;
	}

	void draw(sf::RenderWindow& window) {
		window.draw(PlayZone);
		for (const auto& circle : circlesMap) {
            window.draw(circle);
		}
        
	}

};

int main()
{	
	//Load Sound
	sf::SoundBuffer buffer_click("poptapclick.mp3");
	sf::Sound sound_click(buffer_click);

	//Render the main window
	sf::RenderWindow window( sf::VideoMode( { 1280, 720 } ), "SFML works!" , sf::Style::Titlebar | sf::Style::Close );
    window.setFramerateLimit(60);

	//Hide mouse cursor
	window.setMouseCursorVisible(false);

	//Cursor size and color
	sf::CircleShape cursor(20.f);
	cursor.setOrigin({20.f,20.f});
	cursor.setFillColor(sf::Color::Yellow);

	//Load Font
	sf::Font globalfont;
	if (!globalfont.openFromFile("arial.ttf"))
		{
			//error
		}

	//Gamestate
	GameState currentState = GameState::MainMenu;
	
	//Load main menu
    MainMenu mainMenu(globalfont);
	//Load game menu
	GameMenu map1(globalfont,{640.f,100.f}, "Easy");
	GameMenu map2(globalfont,{640.f,250.f}, "Medium");
	GameMenu map3(globalfont,{640.f,400.f}, "Hard");

	//Level var
	int selectedlevel = 0;
	std::unique_ptr<Map> activeLevel = nullptr;

	
	//Score
	sf::String playerScore = "";

	//Keep window open
	while ( window.isOpen() )
	{
		// Get mouse position and convert
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		
		while ( const std::optional event = window.pollEvent() )
		{
			//Window Closing
			if ( event->is<sf::Event::Closed>() )
				window.close();


			//-------------------Mouse Press Inputs/ For playing loop
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mousePressed ->button == sf::Mouse::Button::Left)
				{
					if(currentState == GameState::Playing)
					{
						if (activeLevel != nullptr)
						{
							sf::Vector2f eventWindowPos = window.mapPixelToCoords(mousePressed->position);

							if (activeLevel ->checkcirclehit(eventWindowPos))
							{
								std::cout << "Hit! \n";
								sound_click.play();
							}
						}
					}
				}
			}
			//-------------------Mouse Release Inputs
			if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) 
			{	
				//Check for left click
				if (mouseReleased->button ==(sf::Mouse::Button::Left)) 
				{
					//Check if click is in MAIN MENU
					if (currentState == GameState::MainMenu) 
					{
						//Check if click is in the Button(mmcircle)
						if (mainMenu.handleMouseButtonPressed(worldPos)) 
							{
								sound_click.play();
								//Switch state to GameMenu
								currentState = GameState::GameMenu;
							}

					}
					//Check if click is in Game Menu
					else if (currentState == GameState::GameMenu)
					{
						//Level 1
						if(map1.LevelClicked(worldPos)){

							sound_click.play();

							selectedlevel = 1; 
							activeLevel = std::make_unique<Map>(0.8f, 60.f, window);	
							currentState = GameState::Playing;
						}
						//Level 2
						else if(map2.LevelClicked(worldPos)){

							sound_click.play();

							selectedlevel = 2;
							activeLevel = std::make_unique<Map>(0.5f, 50.f, window);
							currentState = GameState::Playing;
						}
						//Level 3
						else if(map3.LevelClicked(worldPos)){
							
							sound_click.play();

							selectedlevel = 3;
							activeLevel = std::make_unique<Map>(0.3f, 45.f, window);
							currentState = GameState::Playing;
						}
					}

				}
			}	
			//-------------------Escape to previous screen
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				//Return from GameMenu to MainMenu
				if (currentState == GameState::GameMenu)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
					{
						currentState = GameState::MainMenu;
					}
				}
				else if (currentState == GameState::Playing)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
					{
						currentState = GameState::GameMenu;
					}
				}
			}
			
		}
		//Get mouse position relative to window		
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		//draw custom cursor at mouse position
		mainMenu.update(worldPos);
		cursor.setPosition(static_cast<sf::Vector2f>(mousePos));

		//Keep cursor updating
		cursor.setPosition(worldPos);
		
		//Update
		if (currentState == GameState::MainMenu)
		{
			mainMenu.update(worldPos);
		}
		else if (currentState == GameState::GameMenu) 
		{

		}
		else if (currentState == GameState::Playing && activeLevel != nullptr)
		{
			activeLevel->update(window);
		}

		//Keep cursor updating
		cursor.setPosition(worldPos);

		//Final Render
		window.clear(sf::Color::Black);

		if (currentState == GameState::MainMenu) {

            mainMenu.draw(window);
        }
		else if (currentState == GameState::GameMenu){
			
			map1.draw(window);
			map2.draw(window);
			map3.draw(window);
		}
		else if (currentState == GameState::Playing){

			if (selectedlevel == 1) window.clear(sf::Color(80, 50, 20));
			else if (selectedlevel == 2) window.clear(sf::Color(20, 80, 20));
			else if (selectedlevel == 3) window.clear(sf::Color(50, 50, 50));
			
			if (activeLevel != nullptr) {
            	activeLevel->draw(window);
        	}
		}	
			
		window.draw(cursor);
		window.display();
		
		
	}
}