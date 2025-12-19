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
m_data_max_X{width}, m_data_min_X{0.f}, m_data_min_Y{min_Y}, m_data_max_Y{max_Y}, m_zoom{1.f}, m_padding{10.f}
{
    m_color_of_chart = sf::Color(255, 0, 0);
    m_data_points = MAX_DATA_POINTS/3; //do usunięcia
    if (!m_data.empty()) {
        m_view_min_time = m_data.front().Get_time();
        m_view_max_time = m_data.back().Get_time();
    } else {
        m_view_min_time = 0.0f;
        m_view_max_time = 10.0f;
    }
    m_start = m_view_min_time;
    m_end = m_view_max_time;

    Update_geometry();
}
/**
 * @brief Adds data to the current m_data, and erases the data that is at the end
 * @param new_timestamps new data that will be added to current data.
 */
void LineChart::Add_data(const std::vector<Timestamp>& new_timestamps)
{
    if (new_timestamps.empty()) return;

    for (const Timestamp & ts : new_timestamps)
    {
        m_data.push_back(ts);
    }
    std::sort(m_data.begin(), m_data.end(),
              [](const Timestamp& a, const Timestamp& b){
                  return a.Get_time() < b.Get_time();
              });

    Update_geometry();
}
// void LineChart::Add_data(const std::vector<Timestamp>& new_timestamps)
// {
//     if (new_timestamps.empty()) return;

//     for (const auto& ts : new_timestamps)
//     {
//         m_data.push_back(ts);
//     }

//     if (m_data.size() > MAX_DATA_POINTS)
//         m_data.erase(m_data.begin(), m_data.end() - MAX_DATA_POINTS);

//     std::sort(m_data.begin(), m_data.end(),
//               [](const Timestamp& a, const Timestamp& b){
//                   return a.Get_time() < b.Get_time();
//               });

//     if (m_zoom == 1.f)
//     {
//         float full_min = m_data.front().Get_time();
//         float full_max = m_data.back().Get_time();

//         float span = m_view_max_time - m_view_min_time;
//         m_view_max_time = full_max;
//         m_view_min_time = m_view_max_time - span;
//     }

//     Update_geometry();
// }

/**
 * @brief updates geometry based on the new data that was provided
 */
// void LineChart::Update_geometry()
// {

// }
void LineChart::Update_geometry()
{
    // if(m_data.empty()) return;

    // int visible_point_count = static_cast<int>(m_data_points/m_zoom);
    // if(visible_point_count < 2) visible_point_count = 2;
    // if(visible_point_count > m_data.size()) visible_point_count = m_data.size();
    // // m_scale_X = static_cast<float>(m_width)/static_cast<float>(visible_point_count);
    // m_data_max_X = m_data.back().Get_time();
    // int start_index = static_cast<int>(m_data.size() - visible_point_count);
    // if(0 > start_index) start_index = 0;
    // m_data_min_X = m_data[start_index].Get_time();

    //For dynamic y scaling, 90% it won't be used, letting it stay just in case it was needed
    // m_data_min_Y = std::min_element(m_data.begin() + start_index, m_data.end(), [](const Timestamp& a, const Timestamp& b){
    //     return a.Get_voltage() < b.Get_voltage();
    // })->Get_voltage();
    // m_data_max_Y = std::max_element(m_data.begin() + start_index, m_data.end(), [](const Timestamp& a, const Timestamp& b){
    //     return a.Get_voltage() < b.Get_voltage();
    // })->Get_voltage();
    // float time_span = m_data_max_X - m_data_min_X;



    //////////////////////////////////////////////////////////////
    float time_span = m_view_max_time - m_view_min_time;
    
    if (time_span <= 0.0001f) {
        m_scale_X = 1.0f;
    } else {
        m_scale_X = m_width / time_span;
    }
    // std::cout << m_scale_X << std::endl;

    float voltage_span = m_data_max_Y - m_data_min_Y;
    if (voltage_span <= 0.0001f) {
        m_scale_Y = 1.0f;
    } else {
        m_scale_Y = m_height / voltage_span;
    }
    // std::cout << m_scale_X << " " << m_scale_Y << std::endl;
    std::sort(m_data.begin(), m_data.end(),
          [](const Timestamp& a, const Timestamp& b){ return a.Get_time() < b.Get_time(); });

    auto it_start = std::lower_bound(m_data.begin(), m_data.end(), m_view_min_time, [](const Timestamp& a, float value){
        return a.Get_time() < value;
    });
    if (it_start == m_data.end()) {
        m_start = m_data.size();
    } else {
        m_start = std::distance(m_data.begin(), it_start);
    }
    auto it_end = std::lower_bound(m_data.begin(), m_data.end(), m_view_max_time, [](const Timestamp& a, float value){
        return a.Get_time() < value;
    });
    if (it_end == m_data.end()) {
        m_end = m_data.size();
    } else {
        m_end = std::distance(m_data.begin(), it_end);
        if(m_end < m_data.size()) m_end++;
    }
    ///////////////////////////////////////////////////////////////////

}
/**
 * @brief draws a line chart based on data given to the class.
 * @param target is an object for drawing. Probably window.
 */
void LineChart::Draw(sf::RenderTarget& target) const
{
    // std::cout << m_cursor_position.x << " " << m_cursor_position.y << std::endl;
    Draw_multiple_lines(target, m_color_of_chart);
    Draw_cartesian(target, m_padding);
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
void LineChart::Draw_multiple_lines(sf::RenderTarget& target, sf::Color color_of_lines) const
{
    std::size_t safe_start = (m_start < 1) ? 1 : m_start;
    std::size_t safe_end = (m_end > m_data.size()) ? m_data.size() : m_end;
    if(m_data.size() < 2) return;
    sf::VertexArray segments(sf::PrimitiveType::Lines);
    float window_height = target.getSize().y;
    for(std::size_t i = safe_start; i < safe_end; i++)
    {
        const Timestamp& a = m_data[i-1];
        const Timestamp& b = m_data[i];

        float ax = (a.Get_time() - m_view_min_time) * m_scale_X + m_origin.x;
        float bx = (b.Get_time() - m_view_min_time) * m_scale_X + m_origin.x;
        
        //Scale_Y will be higher than scale_X as voltage is lower than time in most cases
        float ay = window_height - m_origin.y - (a.Get_voltage() - m_data_min_Y) * m_scale_Y;
        float by = window_height - m_origin.y - (b.Get_voltage() - m_data_min_Y) * m_scale_Y;

        if((ax < (m_origin.x + m_padding) && bx < (m_origin.x + m_padding))
        || (ax > (m_origin.x + m_padding + m_width) && bx > (m_origin.x + m_padding + m_width)))
        {
            continue;
        } else 
        if(ax < (m_origin.x + m_padding) && bx > (m_origin.x + m_padding))
        {
            ax = m_origin.x + m_padding;
        } else
        if(ax < (m_origin.x + m_padding + m_width) && bx > (m_origin.x + m_padding + m_width)){
            bx = m_origin.x + m_padding + m_width;
        }
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
    sf::Vector2f(m_origin.x-padding, window_height - m_height - m_origin.y), sf::Color(0, 0, 255), sf::Color(0, 0, 255));
    //Horizontal line
    Draw_line(target, sf::Vector2f(m_origin.x - padding, window_height - m_origin.y + padding), 
    sf::Vector2f(m_origin.x + m_width, window_height - m_origin.y + padding), sf::Color(0, 0, 255), sf::Color(0, 0, 255));
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
bool LineChart::Is_cursor_on_chart(sf::RenderTarget& target) const
{
    float window_height = target.getSize().y;
    if( m_cursor_position.x < m_origin.x || m_cursor_position.x > (m_origin.x+m_width)){
        return false;
    }
    if( m_cursor_position.y < (window_height - m_origin.y - m_height) || m_cursor_position.y > (window_height - m_origin.y)){
        return false;
    }
    return true;
}
[[nodiscard]] sf::Vector2i LineChart::Get_cursor() const
{
    return m_cursor_position;
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
[[nodiscard]] float LineChart::Get_scrolling() const
{
    return m_should_scroll;
}
[[nodiscard]] float LineChart::Get_panning() const
{
    return m_should_pan;
}
void LineChart::Set_cursor(sf::Vector2i position)
{
    m_cursor_position = position;
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
    if (new_zoom < 1.f/3.f || new_zoom > 6.f) return;
    if (m_data.empty()) return;
    if (1.005f > new_zoom && 0.995f < new_zoom) new_zoom = 1.0f;

    float zoom_ratio = new_zoom / m_zoom;

    float cursor_x = static_cast<float>(m_cursor_position.x);
    float alpha = (cursor_x - m_origin.x) / m_width;
    alpha = std::clamp(alpha, 0.0f, 1.0f);
    float current_span = m_view_max_time - m_view_min_time;
    float anchor_time = m_view_min_time + (current_span * alpha);
    float new_span = current_span / zoom_ratio;
    m_view_min_time = anchor_time - (new_span * alpha);
    m_view_max_time = m_view_min_time + new_span;
    m_zoom = new_zoom;
    Update_geometry();
}

void LineChart::Set_origin(sf::Vector2f new_origin)
{
    m_origin = new_origin;
}

void LineChart::Set_scrolling(bool should_scroll)
{
    m_should_scroll = should_scroll;
}
void LineChart::Set_panning(bool should_pan)
{
    m_should_pan = should_pan;
}

void LineChart::Set_color(sf::Color new_color)
{
    m_color_of_chart = new_color;
}