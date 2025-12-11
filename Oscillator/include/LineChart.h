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
 * @brief constructor with parameters
 * @param data is data that will be passed as charts default data
 * @param width is the width of the drawing area
 * @param height is the height of the drawing area
 * @param origin is the position of left bottom corner of a chart.
 * @param min_Y is min value of Y axis.
 * @param max_Y is max value of Y axis.
 */
LineChart(std::vector<Timestamp> data, float width, float height, sf::Vector2f origin = sf::Vector2f(0., 0.), float min_Y = -2.5, float max_Y = 2.5);
/**
 * @brief Adds data to the current m_data, and erases the data that is at the end
 * @param new_timestamps new data that will be added to current data.
 */
void Add_data(const std::vector<Timestamp>& new_timestamps) override;
/**
 * @brief updates geometry based on the new data that was provided
 */
void Update_geometry() override;
/**
 * @brief draws a line chart based on data given to the class.
 * @param target is an object for drawing. Probably window.
 */
void Draw(sf::RenderTarget& target) const override;

/**
 * @brief draws a line.
 * @param begin starting point of the line.
 * @param end ending point of the line.
 * @param color1 color of the begining point.
 * @param color2 color of the end point.
 */
void Draw_line(sf::RenderTarget& target, sf::Vector2f begin, sf::Vector2f end, sf::Color color1, sf::Color color2) const;
/**
 * @brief draws multiply lines based on data value.
 * @param target is an object for drawing. Probably window.
 * @param color_of_lines color of the chart.
 */
void Draw_multiply_lines(sf::RenderTarget& target, sf::Color color_of_lines) const;
/**
 * @brief draws X and Y axises, where X is a moment in time, and y value at that time.
 * @param target is an object for drawing. Probably window.
 * @param padding is a hollow space between axis' and a drawing area.
 */
void Draw_cartesian(sf::RenderTarget& target, float padding) const override;
/**
 * @brief sets given data as the current data. Updates m_line_strip variable with new data.
 */
void Set_data(std::vector<Timestamp> data) override;
[[nodiscard]] float Get_width() const override;
[[nodiscard]] float Get_height() const override;
[[nodiscard]] float Get_scale_X() const override;
[[nodiscard]] float Get_scale_Y() const override;
[[nodiscard]] float Get_zoom() const override;
void Set_width(float new_width) override;
void Set_height(float new_height) override;
void Set_scale_X(float new_X) override;
void Set_scale_Y(float new_Y) override;
void Set_zoom(float new_zoom) override;
void Set_origin(sf::Vector2f new_origin) override; 
private:
std::vector<Timestamp> m_data;
float m_width;
float m_height;
sf::Vector2f m_origin;
float m_scale_X;
float m_scale_Y;
float m_data_min_X;
float m_data_max_X;
float m_data_min_Y;
float m_data_max_Y;
/**
 * zoom controls how many data points fit in the fixed drawing area
 * if zoom = 1.0: m_data_points visible
 * if zoom = 2.0: m_data_points/2 visible (zoomed in)
 * if zoom = 0.5: m_data_points*2 visible (zoomed out)
 * min zoom = 1/3, and max zoom = 6.0
 */
float m_zoom = 1.0f;
const int MAX_DATA_POINTS = 900;
int m_data_points;
bool m_is_scrolling = false;
};