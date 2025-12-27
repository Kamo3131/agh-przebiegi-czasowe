#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "IChart.h"
#include "IReader.h"
#include "SettingsPanel.h"

using namespace Panel;

/**
 * @brief SettingsPanel constructor
 * @note Initializes the panel with default settings and links it to the given SFML window
 * @param window Reference to the SFML window 
 */
SettingsPanel::SettingsPanel(sf::RenderWindow& window): display_mode(ModeType::MODE_1), autoscale(true), show_grid(true), time_div(1.0), volt_div(1.0),
     file_name(""), chart_style(ChartStyle::LINE), font_default(nullptr), reload_clicked(false)
     {
    if (!ImGui::SFML::Init(window)) 
    {
    std::cerr << "Failed to initialize!\n";
    }

    std::string fontPath = "../Oscillator/assets/Roboto-Light.ttf";
    ImGuiIO& io = ImGui::GetIO();

    if (std::filesystem::exists(fontPath)) 
    {
        io.Fonts->Clear();
        font_default = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 19.f);

        if(!ImGui::SFML::UpdateFontTexture())
        {
        std::cerr << "Fail to update font texture \n";
        }
        
    } else 
    {
        std::cerr << "Font not loaded: " << fontPath << "\n";
    }
}

/**
 * @brief Update
 *  Udates the settings panel state for the current frame
 * @param window Reference to the SFML window
 * @param dt time since last frame
 */
void SettingsPanel::Update(sf::RenderWindow& window, sf::Time dt) 
{
    ImGui::SFML::Update(window, dt);
    
    ImGui::SetNextWindowPos(ImVec2(900, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, window.getSize().y), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    
    ImGui::Text("File");
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##filename", file_name.data(), sizeof(file_name));
    
    if (ImGui::Button("Reload data", ImVec2(-1, 30))) 
    {
        reload_clicked.store(true); 
    }
    ImGui::SeparatorText("DISPLAY MODE");
    int display_mode_value = static_cast<int>(display_mode);

    ImGui::RadioButton("Mode 1", &display_mode_value, 0);
    ImGui::RadioButton("Mode 2", &display_mode_value, 1);
    
    ImGui::Dummy(ImVec2(0, 10));
    
    ImGui::SeparatorText("CHART STYLE");
    int chart_style_value = static_cast<int>(chart_style);

    ImGui::RadioButton("Line", &chart_style_value, 0);
    ImGui::RadioButton("Bar", &chart_style_value, 1);

    ImGui::Dummy(ImVec2(0, 10));
    
    ImGui::SeparatorText("CHART PARAMETERS");
    ImGui::Checkbox("Autoscale", &autoscale);
    ImGui::Checkbox("Grid", &show_grid);
    
    if (autoscale) 
    {
        ImGui::BeginDisabled();
    }
    
    ImGui::Text("Time [s]");
    ImGui::SliderFloat("##time", &time_div, 0.1f, 5.0f, "%.2f s/div");
    
    ImGui::Text("Volt [V]");
    ImGui::SliderFloat("##volt", &volt_div, 0.1f, 10.0f, "%.2f V/div");
    
    if (autoscale)
    {
        ImGui::EndDisabled();
    }
}


/**
 * @brief Was_reload_clicked
 * @return Checks if the reload button was clicked
 */
bool SettingsPanel::Was_reload_clicked() 
{
    return reload_clicked.exchange(false);
}


SettingsPanel::~SettingsPanel()
{
    ImGui::SFML::Shutdown();
} 

/**
 * @brief Get_display_mode
 * @return 2 kinds of display modes in which oscilloscope can work
 */
ModeType SettingsPanel::Get_display_mode() const 
{
     return display_mode; 
}

/**
 * @brief Get_chart_style
 * @return 2 kinds of chart style (LINE, BAR) in which oscilloscope can work
 */
ChartStyle SettingsPanel::Get_chart_style() const 
{ 
    return chart_style; 
}

/**
 * @brief Is_autoscale_enabled
 * @return true if autoscale is enabled, false otherwise
 */
bool SettingsPanel::Is_autoscale_enabled() const 
{
    return autoscale;
}

/**
 * @brief Is_show_grid_enabled
 * @return true if grid is enabled, false otherwise
 */
 bool SettingsPanel::Is_show_grid_enabled() const 
{
    return show_grid;
}

/**
 * @brief Get_time_div
 * @return The time division in seconds per division [s/div]
 */
float SettingsPanel::Get_time_div() const 
{
    return time_div;
}
    
/**
 * @brief Get_volt_div
 * @return The volt division in volts per division [V/div]
 */
float SettingsPanel::Get_volt_div() const 
{ 
    return volt_div;
}

/**
 * @brief Get_file_name
 * @return File name from which the data will be loaded to oscilloscope
 */
const std::string & SettingsPanel::Get_file_name() const 
{
    return file_name;
}


