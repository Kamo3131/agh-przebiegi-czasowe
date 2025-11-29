#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

/**
 * @struct Timestamp
 * This struct stores info about voltage and time for one point
 */
struct Timestamp
{
    float _voltage;
    float _time;

    explicit Timestamp(float voltage, float time)
        : _voltage(voltage), _time(time) {}
    
    Timestamp()
        : Timestamp(0.0f, 0.0f) {}

    [[nodiscard]] float Get_time() const
    {
        return _time;
    }

    [[nodiscard]] float Get_voltage() const
    {
        return _voltage;
    }
};

/**
 * @class IReader
 * This class provides common interface for all implementations
 * that give access to data
 */
class IReader
{
public:
    
    /**
     * @brief Get_data
     * @return vector with timestamps
     * @throws runtime_error if timestamps not loaded before
     */
    [[nodiscard]] virtual  std::vector<Timestamp> Get_data() const = 0;

    /**
     * @brief Set_file
     * @param fname name of file to open
     * @return true if file successfully open
     */
    [[nodiscard]] virtual bool Set_file(const std::string_view & fname) = 0;

    /**
     * @brief Load_data
     * @return true if timestamps loaded
     */
    [[nodiscard]] virtual bool Load_data() = 0;

    /**
     * @brief Set_time
     * @param time that will be added to new time values
     * @note use this method if data time value should start 
     * from given 'time' 
     */
    virtual void Set_time(float time) = 0;

    /**
     * @brief Get_time_read
     * @return time value of last loaded timestamp
     */
    [[nodiscard]] virtual float Get_time_read() const = 0;

    virtual ~IReader() {}
};