#pragma once
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "IReader.h"

/**
* @class IChart
* This class provides interface for all implementations of charts.
*/
class IChart : public sf::Drawable
{
public:
/**
 * @brief draws a chart, from the given data 
 */
virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

/**
 * @brief draws X and Y axis, based on the given data.
 */
virtual void Draw_cartesian(sf::RenderTarget& target, sf::RenderStates states) const = 0;

/**
 * @brief sets data for chart to use.
 * @param data is the data that will be set.
 */
virtual void Set_data(std::vector<Timestamp> data) = 0;

virtual ~IChart() {}

};
