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
m_scale_X{1.0}, m_scale_Y{1.0}, m_data_min_Y{min_Y}, m_data_max_Y{max_Y}, m_zoom{1.f}, m_time_span{1.f}
{
    m_data = data;
    m_width = width;
    m_height = height;
    m_origin = origin;
    m_padding = 10.f;
    m_color_of_chart = sf::Color(255, 0, 0);
    if (!m_data.empty()) {
        m_view_min_time = m_data.front().Get_time();
        m_view_max_time = m_data.back().Get_time();
    } else {
        m_view_min_time = 0.0f;
        m_view_max_time = 1.0f;
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
    constexpr size_t max_data_provided = 40000;
    if (m_data.size() < max_data_provided){
            m_data.insert(m_data.end(), new_timestamps.begin(), new_timestamps.end());
            std::sort(m_data.begin(), m_data.end(), [](const Timestamp& a, const Timestamp& b){
                  return a.Get_time() < b.Get_time();
              });
    }
    else {
        auto it_erase = std::lower_bound(m_data.begin(), m_data.end(), m_view_min_time - 10.f, [](const Timestamp& a, float value){
        return a.Get_time() < value;
        });
        if( it_erase != m_data.end()){
            m_data.erase(m_data.begin(), it_erase);
        }
    }
    m_should_scroll = true;
    // std::cout <<  m_view_max_time << " " << m_data.size() << std::endl;
    Update_geometry();
}
/**
 * @brief clears current data and sets min_X to 0 and max_X based on current time span.
 */
void LineChart::Reset_data(){
    m_data.clear();
    m_view_min_time = 0.f;
    m_view_max_time = m_view_min_time + m_time_span;
}

/**
 * @brief sets current X and Y scales, and calculates first and last index for visible elements.
 */
void LineChart::Update_geometry()
{
    constexpr float division_by_zero_safeguard_value = 0.0001f;
    if (m_time_span <= division_by_zero_safeguard_value) {
        m_scale_X = 1.0f;
    } else {
        m_scale_X = m_width / m_time_span;
    }
    float voltage_span = m_data_max_Y - m_data_min_Y;
    if (voltage_span <= division_by_zero_safeguard_value) {
        m_scale_Y = 1.0f;
    } else {
        m_scale_Y = m_height / voltage_span;
    }
    constexpr float scrolling_speed_value = 0.0005f;
    if( m_should_scroll ){
        m_view_min_time+=scrolling_speed_value * m_time_span;
        m_view_max_time+=scrolling_speed_value * m_time_span;
    }
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
}
/**
 * @brief draws a line chart based on data given to the class.
 * @param target is an object for drawing. Probably window.
 */
void LineChart::Draw(sf::RenderTarget& target)
{
    Update_geometry();
    Draw_frame(target);
    Draw_multiple_lines(target, m_color_of_chart);
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

        float ax = (a.Get_time() - m_view_min_time) * m_scale_X + m_origin.x + m_padding;
        float bx = (b.Get_time() - m_view_min_time) * m_scale_X + m_origin.x + m_padding;
        
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
        if(ax < (m_origin.x + m_width) && bx > (m_origin.x + m_width)){
            bx = m_origin.x + m_width;
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
void LineChart::Draw_frame(sf::RenderTarget& target) const
{
    float window_width = target.getSize().x;
    float window_height = target.getSize().y;
    constexpr sf::Color inner_frame_background = sf::Color(10, 10, 10);
    sf::RectangleShape rect({m_width, m_height});
    rect.setPosition(sf::Vector2f(m_origin.x + m_padding, m_origin.y));
    rect.setFillColor(inner_frame_background);
    target.draw(rect);
    constexpr sf::Color outer_frame_color = sf::Color(0, 0, 255);
    constexpr sf::Color inner_frame_color = sf::Color(0, 0, 100);
    //Vertical left line, outer frame
    Draw_line(target, sf::Vector2f(m_origin.x, window_height - m_origin.y + m_padding), 
    sf::Vector2f(m_origin.x, window_height - m_height - m_origin.y - m_padding), outer_frame_color, outer_frame_color);
    //Vertical right line, outer frame
    Draw_line(target, sf::Vector2f(m_origin.x + m_width + m_padding + m_padding, window_height - m_origin.y + m_padding), 
    sf::Vector2f(m_origin.x + m_width + m_padding + m_padding, window_height - m_height - m_origin.y - m_padding), outer_frame_color, outer_frame_color);
    //Horizontal bottom line, outer frame
    Draw_line(target, sf::Vector2f(m_origin.x, window_height - m_origin.y + m_padding), 
    sf::Vector2f(m_origin.x + m_width + m_padding + m_padding, window_height - m_origin.y + m_padding), outer_frame_color, outer_frame_color);
    //Horizontal top line, outer frame
    Draw_line(target, sf::Vector2f(m_origin.x, window_height - m_height - m_origin.y - m_padding), 
    sf::Vector2f(m_origin.x + m_width + m_padding + m_padding, window_height - m_height - m_origin.y - m_padding), outer_frame_color, outer_frame_color);
    //Vertical left line, inner frame
    Draw_line(target, sf::Vector2f(m_origin.x + m_padding, window_height - m_origin.y), 
    sf::Vector2f(m_origin.x + m_padding, window_height - m_height - m_origin.y), inner_frame_color, inner_frame_color);
    //Vertical right line, inner frame
    Draw_line(target, sf::Vector2f(m_origin.x + m_width + m_padding, window_height - m_origin.y), 
    sf::Vector2f(m_origin.x + m_width + m_padding, window_height - m_height - m_origin.y), inner_frame_color, inner_frame_color);
    //Horizontal bottom line, inner frame
    Draw_line(target, sf::Vector2f(m_origin.x + m_padding, window_height - m_origin.y), 
    sf::Vector2f(m_origin.x + m_width + m_padding, window_height - m_origin.y), inner_frame_color, inner_frame_color);
    //Horizontal top line, inner frame
    Draw_line(target, sf::Vector2f(m_origin.x + m_padding, window_height - m_height - m_origin.y), 
    sf::Vector2f(m_origin.x + m_width + m_padding, window_height - m_height - m_origin.y), inner_frame_color, inner_frame_color);
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
[[nodiscard]] float LineChart::Get_time_span() const
{
    return m_time_span;
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
    constexpr float min_zoom = 1.f;
    constexpr float max_zoom = 16.f;
    if (new_zoom < min_zoom || new_zoom > max_zoom) return;
    if (m_data.empty()) return;
    constexpr float higher_than_one_zoom_round_border = 1.010f;
    constexpr float lower_than_one_zoom_round_border = 0.990f;
    if (higher_than_one_zoom_round_border > new_zoom && lower_than_one_zoom_round_border < new_zoom) new_zoom = 1.0f;

    float zoom_ratio = new_zoom / m_zoom;

    float cursor_x = static_cast<float>(m_cursor_position.x);
    float from_min_to_cursor_time = (cursor_x - m_origin.x - m_padding) / m_scale_X;
    float from_cursor_to_max_time = m_time_span - from_min_to_cursor_time;
    float new_time_from_min = from_min_to_cursor_time / zoom_ratio;
    float new_time_from_cursor = from_cursor_to_max_time / zoom_ratio;
    float new_time_span = new_time_from_min + new_time_from_cursor;
    if ( new_time_from_min > from_min_to_cursor_time){
        m_view_min_time -= new_time_from_min - from_min_to_cursor_time; 
    } else {
        m_view_min_time += from_min_to_cursor_time - new_time_from_min;
    }
    if ( new_time_from_cursor > from_cursor_to_max_time){
        m_view_max_time += new_time_from_cursor - from_cursor_to_max_time;
    } else {
        m_view_max_time -= from_cursor_to_max_time - new_time_from_cursor;
    }
    m_time_span = new_time_span;
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

void LineChart::Set_min_X(float new_min_X)
{
    m_view_min_time = new_min_X;
}
void LineChart::Set_max_X(float new_max_X)
{
    m_view_max_time = new_max_X;
}
/**
 * @brief Increases or decreases charts m_view_min_X and m_view_max_X by pan_value, if both of them can be changed.
 * @param pan_value value that will change m_view~ values.
 */
void LineChart::Set_pan(float pan_value)
{
    if((m_view_min_time < m_data.front().Get_time() && pan_value > 0) 
    || (m_view_max_time > m_data.back().Get_time() && pan_value < 0) 
    || (m_view_min_time > m_data.front().Get_time() && m_view_max_time < m_data.back().Get_time())){
        m_view_min_time+=pan_value;
        m_view_max_time+=pan_value;
    }

}

[[nodiscard]] float LineChart::Get_min_X() const
{
    return m_view_min_time;
}
[[nodiscard]] float LineChart::Get_max_X() const
{
    return m_view_max_time;
}