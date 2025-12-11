#include "LineChart.h"
#include "FileReader.h"
#include <algorithm>

/**
 * @brief constructor with parameters
 * @param data is data that will be passed as charts default data
 * @param min_X is min value of X axis.
 * @param max_X is max value of X axis.
 * @param min_Y is min value of Y axis.
 * @param max_Y is max value of Y axis.
 */
LineChart::LineChart(std::vector<Timestamp> data, float width, float height, sf::Vector2f origin, float min_Y, float max_Y) :
m_data{data}, m_width{width}, m_height{height}, 
m_scale_X{1.0}, m_scale_Y{1.0}, m_origin{origin}, 
m_data_max_X{0.f}, m_data_min_X{width}, m_data_min_Y{min_Y}, m_data_max_Y{max_Y}
{
    m_data_points = MAX_DATA_POINTS/3;
}
/**
 * @brief Adds data to the current m_data, and erases the data that is at the end
 * @param new_timestamps new data that will be added to current data.
 */
void LineChart::Add_data(const std::vector<Timestamp>& new_timestamps)
{
    for (const auto& ts : new_timestamps)
    {
        m_data.push_back(ts);
        if (m_data.size() > MAX_DATA_POINTS)
        {
            m_data.erase(m_data.begin());
        }
    }
    m_is_scrolling = true; 
}
/**
 * @brief updates geometry based on the new data that was provided
 */
void LineChart::Update_geometry()
{
    if(m_data.empty()) return;

    int visible_point_count = static_cast<int>(m_data_points/m_zoom);
    if(visible_point_count < 2) visible_point_count = 2;
    if(visible_point_count > m_data.size()) visible_point_count = m_data.size();
    // m_scale_X = static_cast<float>(m_width)/static_cast<float>(visible_point_count);
    m_data_max_X = m_data.back().Get_time();
    int start_index = static_cast<int>(m_data.size() - visible_point_count);
    if(0 > start_index) start_index = 0;
    m_data_min_X = m_data[start_index].Get_time();

    //For dynamic y scaling, 90% it won't be used, letting it stay just in case it was needed
    // m_data_min_Y = std::min_element(m_data.begin() + start_index, m_data.end(), [](const Timestamp& a, const Timestamp& b){
    //     return a.Get_voltage() < b.Get_voltage();
    // })->Get_voltage();
    // m_data_max_Y = std::max_element(m_data.begin() + start_index, m_data.end(), [](const Timestamp& a, const Timestamp& b){
    //     return a.Get_voltage() < b.Get_voltage();
    // })->Get_voltage();
    float time_span = m_data_max_X - m_data_min_X;
    float voltage_span = m_data_max_Y - m_data_min_Y;
    if (time_span <= 0.0001f) {
        m_scale_X = 1.0f;
    } else {
        m_scale_X = m_width / time_span;
    }
    // std::cout << m_scale_X << std::endl;

    
    if (voltage_span <= 0.0001f) {
        m_scale_Y = 1.0f;
    } else {
        m_scale_Y = m_height / voltage_span;
    }
    // std::cout << m_scale_X << " " << m_scale_Y << std::endl;
    m_is_scrolling = false;
}
/**
 * @brief draws a line chart based on data given to the class.
 * @param target is an object for drawing. Probably window.
 */
void LineChart::Draw(sf::RenderTarget& target) const
{
    Draw_multiply_lines(target, sf::Color(255, 0, 0));
    Draw_cartesian(target, 10.0f);
}
/**
 * @brief draws a line.
 * @param begin starting point of the line.
 * @param end ending point of the line.
 * @param color1 color of the begining point.
 * @param color2 color of the end point.
 */
void LineChart::Draw_line(sf::RenderTarget& target, sf::Vector2f begin, sf::Vector2f end, sf::Color color1, sf::Color color2) const
{
    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0].position = begin;
    line[0].color = color1;
    line[1].position = end;
    line[1].color = color2;
    target.draw(line);
}

/**
 * @brief draws multiply lines based on data value.
 * @param target is an object for drawing. Probably window.
 * @param color_of_lines color of the chart.
 */
void LineChart::Draw_multiply_lines(sf::RenderTarget& target, sf::Color color_of_lines) const
{
    
    std::size_t size_of_data = m_data.size();
    if(m_data.size() < 2) return;
    sf::VertexArray segments(sf::PrimitiveType::Lines);
    float window_height = target.getSize().y;
    for(std::size_t i = 1; i < size_of_data; i++)
    {
        const Timestamp& a = m_data[i-1];
        const Timestamp& b = m_data[i];

        if(a.Get_time() < m_data_min_X || a.Get_time() > m_data_max_X 
        || a.Get_voltage() < m_data_min_Y || a.Get_voltage() > m_data_max_Y){
            continue;
        }
        if(b.Get_time() < m_data_min_X || b.Get_time() > m_data_max_X 
        || b.Get_voltage() < m_data_min_Y || b.Get_voltage() > m_data_max_Y){
            continue;
        }
        float ax = (a.Get_time()-m_data_min_X)*m_scale_X+m_origin.x;
        float bx = (b.Get_time()-m_data_min_X)*m_scale_X+m_origin.x;
        
        //Scale_Y will be higher than scale_X as voltage is lower than time in most cases
        float ay = window_height - m_origin.y - (a.Get_voltage() - m_data_min_Y) * m_scale_Y;
        float by = window_height - m_origin.y - (b.Get_voltage() - m_data_min_Y) * m_scale_Y;

        sf::Vertex vertex;
        vertex.position = sf::Vector2f(ax, ay);
        vertex.color = color_of_lines;
        segments.append(vertex);
        vertex.position = sf::Vector2f(bx, by);
        vertex.color = color_of_lines;
        segments.append(vertex);
    }
    if(segments.getVertexCount() > 0){
            target.draw(segments);
    }

}
/**
 * @brief draws X and Y axises, where X is a moment in time, and y value at that time.
 * @param target is an object for drawing. Probably window.
 * @param padding is a hollow space between axis' and a drawing area.
 */
void LineChart::Draw_cartesian(sf::RenderTarget& target, float padding) const
{
    float window_width = target.getSize().x;
    float window_height = target.getSize().y;
    //Vertical line
    Draw_line(target, sf::Vector2f(m_origin.x-padding, window_height - m_origin.y + padding), 
    sf::Vector2f(m_origin.x-padding, window_height - m_height), sf::Color(0, 0, 255), sf::Color(0, 0, 255));
    //Horizontal line
    Draw_line(target, sf::Vector2f(m_origin.x - padding, window_height - m_origin.y + padding), 
    sf::Vector2f(m_origin.x + m_width, window_height - m_origin.y + padding), sf::Color(0, 0, 255), sf::Color(0, 0, 255));
}
    
/**
 * @brief sets given data as the current data
 */
void LineChart::Set_data(std::vector<Timestamp> data)
{
    m_data = data;
}
[[nodiscard]] float LineChart::Get_width() const
{
    return m_width;
}
[[nodiscard]] float LineChart::Get_height() const
{
    return m_height;
}
[[nodiscard]] float LineChart::Get_scale_X() const
{
    return m_scale_X;
}
[[nodiscard]] float LineChart::Get_scale_Y() const
{
    return m_scale_Y;
}
[[nodiscard]] float LineChart::Get_zoom() const
{
    return m_zoom;
}
void LineChart::Set_width(float new_width)
{
    m_width = new_width;
}
void LineChart::Set_height(float new_height)
{
    m_height = new_height;
}
void LineChart::Set_scale_X(float new_scale_X)
{
    m_scale_X = new_scale_X;
}
void LineChart::Set_scale_Y(float new_scale_Y)
{
    m_scale_Y = new_scale_Y;
}
void LineChart::Set_zoom(float new_zoom)
{
    if(new_zoom >= 1.f/3.f && new_zoom <= 6.f){
        m_zoom = new_zoom;    
    }
    return;
}

void LineChart::Set_origin(sf::Vector2f new_origin)
{
    m_origin = new_origin;
}