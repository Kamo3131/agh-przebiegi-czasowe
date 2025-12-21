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
#include "DummyGenerator.h"

int main()
{
sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "IUiBG", sf::Style::Titlebar | sf::Style::Close);
Dummy::FuncIterator func = Dummy::Create_Func(Dummy::FuncType::SIN, 1000, 5, 2);
Dummy::Generator gen(func, 1000);

std::unique_ptr<IReader> reader = std::make_unique<FileReader>();
gen.Start();
reader->Start();

std::this_thread::sleep_for(std::chrono::seconds(3));

std::ofstream file("output");
RecordingHistory test = reader->Get_data();
RecordingHistory::iterator iter = test.Begin();

sf::Font font;
if (!font.openFromFile("../Oscillator/assets/Roboto-Light.ttf"))
{
    std::cerr << "Font not loaded!\n";
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
}
sf::Text text(font); 
text.setCharacterSize(36);
text.setFillColor(sf::Color::Red);
text.setStyle(sf::Text::Bold);
text.setPosition(sf::Vector2f(30.f, 30.f));

    std::unique_ptr<IChart> chart = std::make_unique<LineChart>(reader->Get_data().Get_newest_recordingVector().Get_container(), 600.f, 400.f, sf::Vector2f(100.0, 100.0), -2.5f, 2.5f);
    chart->Set_color(sf::Color(255, 128, 0));
    float panning_speed = 0.05f;
    while (window.isOpen())
    {
        window.clear(sf::Color(61, 53, 53));
        if( (ReaderState::STOPPED != reader->Get_state()) && reader->Check_if_new_data_loaded()){
            // std::cout << "New data loaded!\n";
            chart->Add_data(reader->Get_data().Get_newest_recordingVector().Get_container());
        }
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if(1 == mouseWheelScrolled->delta){
                    chart->Set_zoom(chart->Get_zoom()*1.02);
                } else if (-1 == mouseWheelScrolled->delta){
                    chart->Set_zoom(chart->Get_zoom()*0.98);
                }
            }
            if (const auto * keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if(keyPressed->scancode == sf::Keyboard::Scancode::Space){
                    if(ReaderState::STOPPED == reader->Get_state()){
                        chart->Reset_data();
                        reader->Resume();
                        chart->Set_panning(false);
                        chart->Set_scrolling(true);
                    } else {
                        reader->Stop();
                        chart->Set_panning(true);
                        chart->Set_scrolling(false);
                    }
                }
                if(chart->Get_panning() && (keyPressed->scancode == sf::Keyboard::Scancode::A || keyPressed->scancode == sf::Keyboard::Scancode::Left)){
                    chart->Set_pan(chart->Get_time_span()*(-panning_speed));
                }
                if(chart->Get_panning() && (keyPressed->scancode == sf::Keyboard::Scancode::D || keyPressed->scancode == sf::Keyboard::Scancode::Right)){
                    chart->Set_pan(chart->Get_time_span()*panning_speed);
                }
            }
            if (const auto * mouseMoved = event->getIf<sf::Event::MouseMoved>()){
                chart->Set_cursor(mouseMoved->position);
            }
            if (!chart->Get_scrolling()){
                reader->Stop();
            }
        }
        std::ostringstream ss;
        ss << static_cast<float>(chart->Get_zoom());
        text.setString(ss.str());
        chart->Draw(window);
        window.draw(text);
        window.display();

    }

 gen.Destroy();
 return 0;
}
