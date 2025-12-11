#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <sstream>
#include <filesystem>

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
sf::Font font;
if (!font.openFromFile("../Oscillator/Assets/Roboto-Light.ttf"))
{
    std::cerr << "Font not loaded!\n";
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
}
sf::Text text(font); 
text.setCharacterSize(36);
text.setFillColor(sf::Color::Red);
text.setStyle(sf::Text::Bold);
text.setPosition(sf::Vector2f(30.f, 30.f));
if(reader->Load_data()){
    std::unique_ptr<IChart> chart = std::make_unique<LineChart>(reader->Get_data(), 600.f, 400.f, sf::Vector2f(100.0, 100.0), -1.0f, 2.5f);
    float last_processed_time = -1.0f; 

    sf::Clock clock;
    float time_since_update = 0.0f;
    while (window.isOpen())
    {
        if(reader->Load_data()){
            chart->Add_data(reader->Get_data());
        }
        window.clear(sf::Color(61, 53, 53));
        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if(1 == mouseWheelScrolled->delta){
                    chart->Set_zoom(chart->Get_zoom()*1.01);
                } else if (-1 == mouseWheelScrolled->delta){
                    chart->Set_zoom(chart->Get_zoom()*0.99);
                }
            }
            // --- CZYTANIE PLIKU (np. co 0.1s) ---
        float dt = clock.restart().asSeconds();
        time_since_update += dt;

        if (time_since_update > 0.1f) {
            // 1. Pobieramy WSZYSTKIE dane z pliku
            // Zakładam, że reader.GetData() zwraca std::vector<Timestamp>
            std::vector<Timestamp> all_data_from_file = reader->Get_data(); 

            // 2. Filtrujemy - szukamy tylko nowych punktów
            std::vector<Timestamp> only_new_points;
            // std::cout << last_processed_time << std::endl;
            for (const auto& point : all_data_from_file) {
                // Jeśli czas punktu jest większy niż czas ostatnio dodanego...
                if (point.Get_time() > last_processed_time) {
                    only_new_points.push_back(point);
                }
            }

            // 3. Jeśli mamy nowe punkty, dodajemy je do wykresu
            if (!only_new_points.empty()) {
                chart->Add_data(only_new_points);
                
                // Aktualizujemy znacznik czasu na najnowszy
                last_processed_time = only_new_points.back().Get_time();

                // Wymuszamy przeliczenie geometrii wykresu
                chart->Update_geometry();
            }

            time_since_update = 0.0f;
        }
        }
        std::ostringstream ss;
        ss << static_cast<float>(chart->Get_zoom());
        text.setString(ss.str());
        // window.draw(*chart);
        chart->Update_geometry();
        chart->Draw(window);
        window.draw(text);
        window.display();

    }
} else
{
    std::cerr << "No data loaded. Exiting!\n";
    return -1;
}


 return 0;
}