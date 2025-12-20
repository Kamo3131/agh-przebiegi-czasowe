#include "FileReader.h"
#include <chrono>
#include <filesystem>
#include <memory>
#include <iostream>

FileReader::FileReader(const std::string_view & fname, double start_time)
    : _fname(fname.data()), _start_time(start_time), _thread()
{
    _destroyed  = false;
    _destroy_flag = false;
    _running = false;
    _stop_flag = false;
    _state = ReaderState::CREATED;
    _new_data_loaded = false;
}

/**
 * @brief Get_data
 * @return RecordingHistory with timestamps
 * @throws runtime_error if timestamps not loaded before
 */
[[nodiscard]] const RecordingHistory& FileReader::Get_data() const
{
    if (_data.Empty())
    {
        throw std::runtime_error("Empty data");
    }
    return _data;
}

/**
 * @brief Get_state
 * @return state of the reader
 */
[[nodiscard]] ReaderState FileReader::Get_state() const
{
    return _state;
}

/**
 * @brief Check_if_new_data_loaded
 * @return true if new data after last call to this function was loaded
 */
[[nodiscard]] bool FileReader::Check_if_new_data_loaded()
{
    bool state_val = _new_data_loaded;
    _new_data_loaded = false;
    return state_val;
}

/**
 * @brief Set_file
 * @param fname name of file to open
 * @return true if file successfully open
 */
[[nodiscard]] bool FileReader::Set_file(const std::string_view & fname)
{
    _fname = fname;
    return true;
}

/**
 * @brief Set_history_time_limit
 * @param limit_in_sec limit of seconds of input history stored
 */
void FileReader::Set_history_time_limit(int limit_in_sec)
{
    _data.Set_history_time_limit(limit_in_sec);
}

/**
 * @brief running_loop
 * @note loop in which Reader reads data
 */
void FileReader::running_loop(void)
{
    if (!_running)
    {
        const std::chrono::duration sleep_time_running = std::chrono::seconds(1);
        const std::chrono::duration sleep_time_stopped = std::chrono::milliseconds(100);
        _running = true;
        _destroyed = false;
        while (!_destroy_flag)
        {
            if (std::filesystem::exists(_fname))
            {
                if (_stop_flag)
                {
                    std::this_thread::sleep_for(sleep_time_stopped);
                }
                else
                {
                    
                    _file.open(_fname);
                    if (_file.is_open())
                    {
                        _state = ReaderState::READING;
                        //if file exist read data
                        RecordingVector vec;
                        double max_voltage = 0.0;
                        double min_voltage = 0.0;
                        double min_time = _start_time;
                        double max_time = 0.0;
                        int index = 0;
                        while (!_file.eof())
                        {
                            double time = 0.0f;
                            _file >> time;
                            double voltage = 0.0f;
                            _file >> voltage;
                            if (voltage > max_voltage)
                            {
                                max_voltage = voltage;
                            }
                            time += _start_time;
                            
                            vec.Get_container().emplace_back(time, voltage);
                            index++;
                            max_time = time;
                        }
                        _file.close();
                        //try to remove file
                        std::error_code ec;
                        std::filesystem::remove(_fname, ec);
                        /*if can't be removed it's written by Generator, so
                        try again later */
                        if (!ec)
                        {
                            RangeParams params({min_voltage, max_voltage},
                            {min_time, max_time}, index);
                            vec.Set_recording_params(params);
                            _data.Push_recordingVector(std::move(vec));
                            _start_time = max_time;
                            _new_data_loaded = true;
                        }
                        else
                        {
                            continue;
                        }
                        _state = ReaderState::WAITING;
                        std::this_thread::sleep_for(sleep_time_running);
                    }
                    else
                    {
                        _state = ReaderState::WAITING;
                        std::this_thread::sleep_for(sleep_time_stopped);
                    }
                }
                
            }
        }
        _running = false;
        _destroyed = true;
    }
}

/**
 * @brief Start
 * @note starts execution of Reader thread
 */
void FileReader::Start()
{
    _thread = std::thread(&FileReader::running_loop, this);
}

/**
 * @brief Stop
 * @note stops execution of Reader thread
 */
void FileReader::Stop()
{
    _stop_flag = true;
    _start_time = 0;
    _data.Clear();
    _state = ReaderState::STOPPED;
}

/**
 * @brief Resume
 * @note resumes execution after Stop() was called
 */
void FileReader::Resume()
{
    _stop_flag = false;
    _state = ReaderState::WAITING;
}

/**
 * @brief Destroy
 * @note destroys thread, after this new thread with Start() can be created
 */
void FileReader::Destroy()
{
    _destroy_flag = true;
    if (_thread.joinable())
        _thread.join();
    _state = ReaderState::DESTROYED;
}

FileReader::~FileReader()
{
    if (!_destroyed)
    {
        Destroy();
    }
    _data.Clear();
}