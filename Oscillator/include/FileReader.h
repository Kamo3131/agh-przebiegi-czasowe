#pragma once
#include "IReader.h"

/**
 * @class Reader
 * This class implements IReader interface that reads from files
 */
class FileReader final : public IReader
{
public:
    FileReader(const std::string_view & fname, float start_time = 0.0f)
        : _file(fname.data()), _start_time(start_time) {}
   
    /**
     * @brief Get_data
     * @return vector with timestamps
     * @throws runtime_error if timestamps not loaded before
     */
    [[nodiscard]] std::vector<Timestamp> Get_data() const override;

    /**
     * @brief Set_file
     * @param fname name of file to open
     * @return true if file successfully open
     */
    [[nodiscard]] bool Set_file(const std::string_view & fname) override;

    /**
     * @brief Load_data
     * @return true if timestamps loaded
     */
    [[nodiscard]] bool Load_data() override;

    /**
     * @brief Set_time
     * @param time that will be added to new time values
     * @note use this method if data time value should start 
     * from given 'time' 
     */
    void Set_time(float time) override;

    /**
     * @brief Get_time_read
     * @return time value of last loaded timestamp
     */

    [[nodiscard]] float Get_time_read() const override;
private:
    float _start_time;
    std::ifstream _file;
    std::vector<Timestamp> _timestamps;
};