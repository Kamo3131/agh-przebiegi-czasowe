#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

//Projects includes
#include "IReader.h"
#include "FileReader.h"

int main()
{
 sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "IUiBG", sf::Style::Titlebar | sf::Style::Close);
//  Simple tests (working)
//  std::unique_ptr<IReader> reader = std::make_unique<FileReader>("test");
//  std::cout << (reader->Set_file("hello") ? "true" : "false") << std::endl;
//  std::cout << reader->Get_time_read() << std::endl;
//  reader->Set_time(100);
//  std::cout << reader->Get_time_read() << std::endl;

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