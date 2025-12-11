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
 */
virtual void Draw(sf::RenderTarget& target) const = 0;

/**
 * @brief draws X and Y axis, based on the given data.
 */
virtual void Draw_cartesian(sf::RenderTarget& target, float padding) const = 0;

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
 * @brief updates geometry based on the new data that was provided
 */
virtual void Update_geometry() = 0;
[[nodiscard]] virtual float Get_width() const = 0;
[[nodiscard]] virtual float Get_height() const = 0;
[[nodiscard]] virtual float Get_scale_X() const = 0;
[[nodiscard]] virtual float Get_scale_Y() const = 0;
[[nodiscard]] virtual float Get_zoom() const = 0;
virtual void Set_width(float new_width) = 0;
virtual void Set_height(float new_height) = 0;
virtual void Set_scale_X(float new_X) = 0;
virtual void Set_scale_Y(float new_Y) = 0;
virtual void Set_zoom(float new_zoom) = 0; 
virtual void Set_origin(sf::Vector2f new_origin) = 0; 
virtual ~IChart() {}

};
