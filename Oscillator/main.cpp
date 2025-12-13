#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

//Projects includes
#include "IReader.h"
#include "FileReader.h"
#include "DummyGenerator.h"

int main()
{
 sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "IUiBG", sf::Style::Titlebar | sf::Style::Close);
//  Simple tests (working)
//  std::unique_ptr<IReader> reader = std::make_unique<FileReader>("test");
//  std::cout << (reader->Set_file("hello") ? "true" : "false") << std::endl;
//  std::cout << reader->Get_time_read() << std::endl;
//  reader->Set_time(100);
//  std::cout << reader->Get_time_read() << std::endl;
Dummy::FuncIterator func = Dummy::Create_Func(Dummy::FuncType::SIN, 1000, 5, 2);
Dummy::Generator gen(func, 1000);


std::unique_ptr<IReader> reader = std::make_unique<FileReader>();
gen.Start();
reader->Start();

std::this_thread::sleep_for(std::chrono::seconds(3));
gen.Destroy();

std::ofstream file("output");
RecordingHistory test = reader->Get_data();
RecordingHistory::iterator iter = test.Begin();
while (iter != test.End())
{
    Timestamp tmp = *iter;
    file << tmp.Get_time() << " " << tmp.Get_voltage() << "\n";
    iter++;
}

std::cout << test.Get_recording_params().Get_min_voltage() << " , voltage max: " << test.Get_recording_params().Get_max_voltage();
file.close();

 while (window.isOpen())
  {
   window.clear(sf::Color::White);
   
   while (const std::optional event = window.pollEvent())
    {
     if (event->is<sf::Event::Closed>()) window.close();
    }
  
   window.display();
  }

  gen.Destroy();
 return 0;
}