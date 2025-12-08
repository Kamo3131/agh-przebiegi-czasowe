#pragma once
#include "IChart.h"

/**
 * @class LineChart
 * @brief Implementation of class IChart. It draws a line chart.
 */
class LineChart final: public IChart
{
public:
/**
 * @brief default constructor for LineChart. 
 * Sets m_min_X = 0., m_max_X = 10., m_min_Y = 0. and m_max_Y = 10.
 */
LineChart();
/**
 * @brief constructor with parameters
 * @param data is data that will be passed as charts default data
 * @param min_X is min value of X axis.
 * @param max_X is max value of X axis.
 * @param min_Y is min value of Y axis.
 * @param max_Y is max value of Y axis.
 */
LineChart(std::vector<Timestamp> data, float min_X=0., float max_X=10., float min_Y = 0., float max_Y=10.);
/**
 * @brief draws a line chart based on data given to the class.
 */
void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
/**
 * @brief draws X and Y axises, where X is a moment in time, and y value at that time.
 */
void Draw_line(sf::RenderTarget& target, sf::Vector2f begin, sf::Vector2f end) const;
/**
 * @brief draws multiply lines based on data value.
 */
void Draw_multiply_lines(sf::RenderTarget& target, sf::RenderStates states, sf::Color color_of_lines) const;
/**
 * @brief draws X and Y axis, based on the given data.
 */
void Draw_cartesian(sf::RenderTarget& target, sf::RenderStates states) const override;
/**
 * @brief increases or decreases the scale of the chart based on current data
 */
void Auto_scale(const sf::RenderTarget& target);
/**
 * @brief sets given data as the current data. Updates m_line_strip variable with new data.
 */
void Set_data(std::vector<Timestamp> data) override;
[[nodiscard]] float Get_min_X() const;
[[nodiscard]] float Get_max_X() const;
[[nodiscard]] float Get_min_Y() const;
[[nodiscard]] float Get_max_Y() const;
[[nodiscard]] float Get_scale_X() const;
[[nodiscard]] float Get_scale_Y() const;
void Set_min_X(float new_X);
void Set_max_X(float new_X);
void Set_min_Y(float new_Y);
void Set_max_Y(float new_Y);
void Set_scale_X(float new_X);
void Set_scale_Y(float new_Y);
private:
std::vector<Timestamp> m_data;
float m_min_X, m_max_X; // X axis starting point, and X axis finish point
float m_min_Y, m_max_Y; // Y axis starting point, and Y axis finish point
float m_scale_X;
float m_scale_Y;
float m_margin = 100.;
// sf::VertexArray m_line_strip;
};