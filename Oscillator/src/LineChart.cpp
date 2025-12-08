#include "LineChart.h"
#include "FileReader.h"
#include <algorithm>

/**
 * @brief default constructor for LineChart. 
 * Sets m_min_X = 0., m_max_X = 10., m_min_Y = 0. and m_max_Y = 10.
 */
LineChart::LineChart() : m_min_X{0.}, m_max_X{10.}, m_min_Y{0.}, m_max_Y{10.}, m_scale_X{1.0}, m_scale_Y{1.0}
{}
/**
 * @brief constructor with parameters
 * @param data is data that will be passed as charts default data
 * @param min_X is min value of X axis.
 * @param max_X is max value of X axis.
 * @param min_Y is min value of Y axis.
 * @param max_Y is max value of Y axis.
 */
LineChart::LineChart(std::vector<Timestamp> data, float min_X, float max_X, float min_Y, float max_Y) :
m_data{data}, m_min_X{min_X}, m_max_X{max_X}, m_min_Y{min_Y}, m_max_Y{max_Y}, m_scale_X{1.0}, m_scale_Y{1.0}
{}
/**
 * @brief draws a line chart based on data given to the class.
 */
void LineChart::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    Draw_multiply_lines(target, states, sf::Color(255, 0, 0));
}
/**
 * @brief draws a line.
 * @param begin starting point of the line.
 * @param end ending point of the line.
 */
void LineChart::Draw_line(sf::RenderTarget& target, sf::Vector2f begin, sf::Vector2f end) const
{
    
}

/**
 * @brief draws multiply lines based on data value.
 */
void LineChart::Draw_multiply_lines(sf::RenderTarget& target, sf::RenderStates states, sf::Color color_of_lines) const
{
    std::size_t size_of_data = m_data.size();  
    sf::VertexArray line_strip(sf::PrimitiveType::LineStrip, size_of_data);
    float window_height = target.getSize().y;
    for(std::size_t i = 0; i < size_of_data; i++)
    {
        float pos_x = (m_data[i].Get_time()-m_min_X)*m_scale_X+m_margin;
        float pos_y = window_height - (m_data[i].Get_voltage() - m_min_Y)*m_scale_Y-m_margin;
        line_strip[i].position = sf::Vector2f(pos_x, pos_y);
        line_strip[i].color = color_of_lines;
    }
    target.draw(line_strip, states);
}
/**
 * @brief draws X and Y axises, where X is a moment in time, and y value at that time.
 */
void LineChart::Draw_cartesian(sf::RenderTarget& target, sf::RenderStates states) const
{}

/**
 * @brief increases or decreases the scale of the chart based on current data
 */
void LineChart::Auto_scale(const sf::RenderTarget& target)
{
    if(m_data.empty()) return;
    std::vector<Timestamp>::iterator min_x_it = std::min_element(m_data.begin(), m_data.end(), 
        [](const Timestamp& a, const Timestamp& b){ return a.Get_time() < b.Get_time(); });
    std::vector<Timestamp>::iterator max_x_it = std::max_element(m_data.begin(), m_data.end(), 
        [](const Timestamp& a, const Timestamp& b){ return a.Get_time() < b.Get_time(); });
    
    std::vector<Timestamp>::iterator min_y_it = std::min_element(m_data.begin(), m_data.end(), 
        [](const Timestamp& a, const Timestamp& b){ return a.Get_voltage() < b.Get_voltage(); });
    std::vector<Timestamp>::iterator max_y_it = std::max_element(m_data.begin(), m_data.end(), 
        [](const Timestamp& a, const Timestamp& b){ return a.Get_voltage() < b.Get_voltage(); });

    float data_min_x = min_x_it->Get_time();
    float data_max_x = max_x_it->Get_time();
    float data_min_y = min_y_it->Get_voltage();
    float data_max_y = max_y_it->Get_voltage();

    float screen_width = target.getSize().x - (2 * m_margin);
    float screen_height = target.getSize().y - (2 * m_margin);

    float range_x = data_max_x - data_min_x;
    float range_y = data_max_y - data_min_y;

    m_scale_X = (range_x != 0) ? (screen_width / range_x) : 1.0f;
    m_scale_Y = (range_y != 0) ? (screen_height / range_y) : 1.0f;
    
    // m_min_X = data_min_x;
    // m_min_Y = data_min_y;
}
    
/**
 * @brief sets given data as the current data
 */
void LineChart::Set_data(std::vector<Timestamp> data)
{
    m_data = data;
    // std::size_t size_of_data = m_data.size();  
    // sf::VertexArray line_strip(sf::PrimitiveType::LineStrip, size_of_data);
    // m_line_strip = line_strip;
}
[[nodiscard]] float LineChart::Get_min_X() const
{
    return m_min_X;
}
[[nodiscard]] float LineChart::Get_max_X() const
{
    return m_max_X;
}
[[nodiscard]] float LineChart::Get_min_Y() const
{
    return m_min_Y;
}
[[nodiscard]] float LineChart::Get_max_Y() const
{
    return m_max_Y;
}
[[nodiscard]] float LineChart::Get_scale_X() const
{
    return m_scale_X;
}
[[nodiscard]] float LineChart::Get_scale_Y() const
{
    return m_scale_Y;
}
void LineChart::Set_min_X(float new_X)
{
    m_min_X = new_X;
}
void LineChart::Set_max_X(float new_X)
{
    m_max_X = new_X;
}
void LineChart::Set_min_Y(float new_Y)
{
    m_min_Y = new_Y;
}
void LineChart::Set_max_Y(float new_Y)
{
    m_max_Y = new_Y;
}
void LineChart::Set_scale_X(float new_scale_X)
{
    m_scale_X = new_scale_X;
}
void LineChart::Set_scale_Y(float new_scale_Y)
{
    m_scale_Y = new_scale_Y;
}