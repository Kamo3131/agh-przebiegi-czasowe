#pragma once
#include <string>
#include <atomic>
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "IChart.h"
#include "IReader.h"


namespace Panel
{

/**
 * @enum ChartStyle
 * @note defines which chart style to use
 */
enum class ChartStyle
{
    LINE,
    BAR,
};


/**
 * @enum ModeType
 * @note defines which mode type to use
 */
enum class ModeType
{
    MODE_1,
    MODE_2,
};

    
/**
 *  @class SettingsPanel
 *  @note This class is used to specify and to update UI 
 *  Represents the panel where user can change settings for the UI of oscilloscope
 */
class SettingsPanel
{
public:
    /**
     * @brief SettingsPanel constructor
     * @note Initializes the panel with default settings and links it to the given SFML window
     * @param window Reference to the SFML window 
     */
    SettingsPanel(sf::RenderWindow& window);

    /**
     * @brief Update
     *  Udates the settings panel state for the current frame
     * @param window Reference to the SFML window
     * @param dt time since last frame
     */
    void Update(sf::RenderWindow& window, sf::Time dt);
    
    /**
     * @brief Was_reload_clicked
     * @return Checks if the reload button was clicked
     */
    bool Was_reload_clicked();
    
    /**
     * @brief Get_display_mode
     * @return 2 kinds of display modes in which oscilloscope can work
     */
    [[nodiscard]] ModeType Get_display_mode() const;

    /**
     * @brief Get_chart_style
     * @return 2 kinds of chart style (LINE, BAR) in which oscilloscope can work
     */
    [[nodiscard]] ChartStyle Get_chart_style() const;

    /**
     * @brief Is_autoscale_enabled
     * @return true if autoscale is enabled, false otherwise
     */
    [[nodiscard]] bool Is_autoscale_enabled() const;

    /**
     * @brief Is_show_grid_enabled
     * @return true if grid is enabled, false otherwise
     */
    [[nodiscard]] bool Is_show_grid_enabled() const;

    /**
     * @brief Get_time_div
     * @return The time division in seconds per division [s/div]
     */
    [[nodiscard]] float Get_time_div() const;
    
    /**
     * @brief Get_volt_div
     * @return The volt division in volts per division [V/div]
     */
    [[nodiscard]] float Get_volt_div() const;

    /**
     * @brief Get_file_name
     * @return File name from which the data will be loaded to oscilloscope
     */
    [[nodiscard]] const std::string & Get_file_name() const;
    
    ~SettingsPanel();
private:
    Panel::ModeType display_mode;
    float time_div;
    float volt_div;
    std::string file_name;
    Panel::ChartStyle chart_style;
    ImFont* font_default;
    std::atomic_bool  reload_clicked;
    bool show_grid;
    bool autoscale;
};
} //namespace Panel


