#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

//Projects includes
#include "IReader.h"
#include "FileReader.h"
#include "IChart.h"
#include "LineChart.h"

int main()
{
 sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "IUiBG", sf::Style::Titlebar | sf::Style::Close);
//  Simple tests (working)
//  std::unique_ptr<IReader> reader = std::make_unique<FileReader>("test");
//  std::cout << (reader->Set_file("hello") ? "true" : "false") << std::endl;
//  std::cout << reader->Get_time_read() << std::endl;
//  reader->Set_time(100);
//  std::cout << reader->Get_time_read() << std::endl;

 std::unique_ptr<IReader> reader = std::make_unique<FileReader>("data.txt"); 
if(reader->Load_data()){

    // std::unique_ptr<IChart> chart = std::make_unique<LineChart>(reader->Get_data());
    // lineChart.Set_zoom_value(10.);
    LineChart lineChart(reader->Get_data());
    lineChart.Auto_scale(window);
    while (window.isOpen())

        {
        window.clear(sf::Color::White);
        
        while (const std::optional event = window.pollEvent())
            {
            if (event->is<sf::Event::Closed>()) window.close();
            }
        // window.draw(*chart);

        window.draw(lineChart);
        window.display();
        }
} else
{
    std::cerr << "No data loaded. Exiting!\n";
    return -1;
}


 return 0;
}