#pragma once
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "IReader.h"

/**
* @class IChart
* This class provides interface for all implementations of charts.
*/
class IChart
{
public:
/**
 * @brief draws a chart, from the given data
 * @param target is an object for drawing. Probably window. 
 */
virtual void Draw(sf::RenderTarget& target) = 0;

/**
 * @brief sets data for chart to use.
 * @param data is the data that will be set.
 */
virtual void Set_data(std::vector<Timestamp> data) = 0;
/**
 * @brief adds data to the current data, and erases data which exceed max possible points.
 * @param new_timestamps data that will be added to the current data.
 */
virtual void Add_data(const std::vector<Timestamp>& new_timestamps) = 0;
/**
 * @brief Checks if given cursor position is within the bonds of chart's drawing space.
 * @param target is an object for drawing. Probably window. 
 */
virtual bool Is_cursor_on_chart(sf::RenderTarget& target) const = 0;
/**
 * @brief clears current data and sets min_X to 0 and max_X based on current time span.
 */
virtual void Reset_data() = 0;
[[nodiscard]] virtual sf::Vector2i Get_cursor() const = 0;
[[nodiscard]] virtual float Get_width() const = 0;
[[nodiscard]] virtual float Get_height() const = 0;
[[nodiscard]] virtual float Get_zoom() const = 0;
[[nodiscard]] virtual float Get_scrolling() const = 0;
[[nodiscard]] virtual float Get_panning() const = 0;
[[nodiscard]] virtual float Get_time_span() const = 0;
virtual void Set_cursor(sf::Vector2i postion) = 0;
virtual void Set_width(float new_width) = 0;
virtual void Set_height(float new_height) = 0;
virtual void Set_zoom(float new_zoom) = 0; 
virtual void Set_origin(sf::Vector2f new_origin) = 0;
virtual void Set_scrolling(bool should_scroll) = 0;
virtual void Set_panning(bool should_pan) = 0;  
virtual void Set_color(sf::Color new_color) = 0;
virtual void Set_pan(float pan_value) = 0;
virtual ~IChart() {}

protected:
std::vector<Timestamp> m_data;
float m_width;
float m_height;
sf::Vector2f m_origin;
float m_padding;
sf::Vector2i m_cursor_position;
bool m_should_scroll = false;
bool m_should_pan = false;
sf::Color m_color_of_chart;
};