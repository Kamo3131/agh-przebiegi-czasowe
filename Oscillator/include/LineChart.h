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
 * @brief draws a line chart based on data given to the class.
 * @param target is an object for drawing. Probably window.
 */
void Draw(sf::RenderTarget& target) override;
/**
 * @brief sets data for chart to use.
 * @param data is the data that will be set.
 */
void Set_data(std::vector<Timestamp> data) override;
/**
 * @brief Checks if given cursor position is within the bonds of chart's drawing space.
 * @param target is an object for drawing. Probably window. 
 */
bool Is_cursor_on_chart(sf::RenderTarget& target) const override;
/**
 * @brief clears current data and sets min_X to 0 and max_X based on current time span.
 */
void Reset_data() override;
[[nodiscard]] sf::Vector2i Get_cursor() const override;
[[nodiscard]] float Get_width() const override;
[[nodiscard]] float Get_height() const override;
[[nodiscard]] float Get_zoom() const override;
[[nodiscard]] float Get_scrolling() const override;
[[nodiscard]] float Get_panning() const override;
[[nodiscard]] float Get_time_span() const override;
void Set_cursor(sf::Vector2i postion) override;
void Set_width(float new_width) override;
void Set_height(float new_height) override;
void Set_zoom(float new_zoom) override;
void Set_origin(sf::Vector2f new_origin) override;
void Set_scrolling(bool should_scroll) override;
void Set_panning(bool should_pan) override;
/**
 * @brief Sets color of the chart.
 * @param new_color a color to which chart will be set.
 */
void Set_color(sf::Color new_color) override;

/**
 * @brief Increases or decreases charts m_view_min_X and m_view_max_X by pan_value, if both of them can be changed.
 * @param pan_value value that will change m_view~ values.
 */
void Set_pan(float pan_value) override;
private:
/**
 * @brief updates geometry based on the data that was provided
 */
void Update_geometry();
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
void Draw_multiple_lines(sf::RenderTarget& target, sf::Color color_of_lines) const;
/**
 * @brief draws a frame around drawing area, based on the given data.
 * @param target is an object for drawing. Probably window. 
 * @param padding value added between the chart and the outer frame.
 */
void Draw_frame(sf::RenderTarget& target) const;
[[nodiscard]] float Get_scale_X() const;
[[nodiscard]] float Get_scale_Y() const;
[[nodiscard]] float Get_min_X() const;
[[nodiscard]] float Get_max_X() const;
void Set_scale_X(float new_X);
void Set_scale_Y(float new_Y);
void Set_min_X(float new_min_X);
void Set_max_X(float new_max_X);
/**
 * Scale of how much X-axis should one point take
 */
float m_scale_X;
/**
 * Scale of how much Y-axis should voltage take
 */
float m_scale_Y;
/**
 * Min value of voltage.
 */
float m_data_min_Y;
/**
 * Max value of voltage.
 */
float m_data_max_Y;
/**
 * Span of how much time should be seen on the chart.
 */
float m_time_span;
float m_view_min_time;
float m_view_max_time;
/**
 * Index of the first point that can be seen.
 */
int m_start;
/**
 * Index of the first point that is beyond current chart's view.
 */
int m_end;
/**
 * zoom controls how many data points fit in the fixed drawing area
 * if zoom = 1.0: m_time_span visible
 * if zoom = 2.0: m_time_span/2 visible (zoomed in)
 * if zoom = 0.5: m_time_span*2 visible (zoomed out)
 * min zoom = 1/3, and max zoom = 16.0
 */
float m_zoom;
};