#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main()
{
 sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "IUiBG", sf::Style::Titlebar | sf::Style::Close);

 while (window.isOpen())
  {
   window.clear(sf::Color::White);
   
   while (const std::optional event = window.pollEvent())
    {
     if (event->is<sf::Event::Closed>()) window.close();
    }
  
   window.display();
  }
 return 0;
}