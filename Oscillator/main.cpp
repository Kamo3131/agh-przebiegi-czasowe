#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
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
#include "SettingsPanel.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1200, 600 }), "IUiBG", sf::Style::Titlebar | sf::Style::Close);
    Panel::SettingsPanel panel_settings(window);
    // clock for ImGui
    sf::Clock dt;

    Dummy::FuncIterator func = Dummy::Create_Func(Dummy::FuncType::SIN, 1000, 5, 2);
    Dummy::Generator gen(func, 1000);

    std::unique_ptr<IReader> reader = std::make_unique<FileReader>();
    gen.Start();
    reader->Start();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::ofstream file("output");
    RecordingHistory test = reader->Get_data();
    RecordingHistory::iterator iter = test.Begin();



    std::unique_ptr<IChart> chart = std::make_unique<LineChart>(reader->Get_data().Get_newest_recordingVector().Get_container(), 850., 560., sf::Vector2f(10.0, 20.0), -2.5f, 2.5f);
    chart->Set_color(sf::Color(255, 128, 0));
    float panning_speed = 0.05f;
    while (window.isOpen())
    {
        if( (ReaderState::STOPPED != reader->Get_state()) && reader->Check_if_new_data_loaded())
        {
            // std::cout << "New data loaded!\n";
            chart->Add_data(reader->Get_data().Get_newest_recordingVector().Get_container());
        }

        while (const std::optional event = window.pollEvent())
        {
        ImGui::SFML::ProcessEvent(window, *event);
        if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) 
        {
            continue;
        }

            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if(1 == mouseWheelScrolled->delta){
                    chart->Set_zoom(chart->Get_zoom()*1.02);
                } else if (-1 == mouseWheelScrolled->delta){
                    chart->Set_zoom(chart->Get_zoom()*0.98);
                }
            }
            if (const auto * keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if(keyPressed->scancode == sf::Keyboard::Scancode::Space)
                {
                    if(ReaderState::STOPPED == reader->Get_state())
                    {
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
                if(chart->Get_panning() && (keyPressed->scancode == sf::Keyboard::Scancode::A || keyPressed->scancode == sf::Keyboard::Scancode::Left))
                {
                    chart->Set_pan(chart->Get_time_span()*(-panning_speed));
                }
                if(chart->Get_panning() && (keyPressed->scancode == sf::Keyboard::Scancode::D || keyPressed->scancode == sf::Keyboard::Scancode::Right))
                {
                    chart->Set_pan(chart->Get_time_span()*panning_speed);
                }
            }
            if (const auto * mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                chart->Set_cursor(mouseMoved->position);
            }
            if (!chart->Get_scrolling())
            {
                reader->Stop();
            }
        }

        panel_settings.Update(window, dt.restart());
        
        if (panel_settings.Was_reload_clicked()) {
            reader->Stop();
            reader->Set_file(panel_settings.Get_file_name());
            chart->Reset_data();
            reader->Resume();
            chart->Set_scrolling(true);
        }

        ImGui::Separator();         
        ImGui::Text("Zoom: %.3f", chart->Get_zoom());
        ImGui::Text("Panning: %s", chart->Get_panning() ? "ON" : "OFF");
        ImGui::Separator();
        ImGui::End();
        window.clear(sf::Color(61, 53, 53));


        // std::ostringstream ss;
        // ss << static_cast<float>(chart->Get_zoom());
        // text.setString(ss.str());
        chart->Draw(window);
        // window.draw(text);
        ImGui::SFML::Render(window);
        window.display();
    }
     gen.Destroy();
 return 0;
}

