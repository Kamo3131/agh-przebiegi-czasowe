#include "FileReader.h"

/**
 * @brief Get_data
 * @return vector with timestamps
 * @throws runtime_error if timestamps not loaded before
 */
[[nodiscard]] std::vector<Timestamp> FileReader::Get_data() const
{
    if (_timestamps.empty())
    {
        throw std::runtime_error("Empty data");
    }
    return _timestamps;
}

/**
 * @brief Set_file
 * @param fname name of file to open
 * @return true if file successfully open
 */
[[nodiscard]] bool FileReader::Set_file(const std::string_view & fname)
{
    if (_file.is_open())
    {
        _file.close();
    }

    _file.open(fname.data());
    return _file.is_open();
}

/**
 * @brief Load_data
 * @return true if timestamps loaded
 */
[[nodiscard]] bool FileReader::Load_data()
{
    if (!_file.is_open())
    {
        return false;
    }

    //Clear previous timestamps and load new timestamps
    _timestamps.clear();
    while (!_file.eof())
    {
        float voltage = 0.0f;
        _file >> voltage;
        float time = 0.0f;
        _file >> time;
        time += _start_time;
        _timestamps.emplace_back(voltage, time);
        _start_time = time;
    }

    //Rewind and clear state flags
    _file.clear();
    _file.seekg(0);

    return true;
}

/**
 * @brief Set_time
 * @param time that will be added to new time values
 * @note use this method if data time value should start 
 * from given 'time' 
 */
void FileReader::Set_time(float time)
{
    _start_time = time;
}

/**
 * @brief Get_time_read
 * @return time value of last loaded timestamp
 */
[[nodiscard]] float FileReader::Get_time_read() const
{
    return _start_time;
}