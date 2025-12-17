#pragma once
#include <atomic>
#include <thread>

#include "IReader.h"

/**
 * @class Reader
 * This class implements IReader interface that reads from files in it's own thread.
 * If file with data exists Reader read data and removes file. Then waits unit new file
 * with data is created and repeats process.
 */
class FileReader final : public IReader
{
public:
    FileReader(const std::string_view & fname = "tmp", double start_time = 0.0f);
   
    /**
     * @brief Get_data
     * @return RecordingHistory with all timestamps collected
     */
    [[nodiscard]] const RecordingHistory& Get_data() const override;

    /**
     * @brief Get_state
     * @return state of the reader
     */
    [[nodiscard]] ReaderState Get_state() const override;

    /**
     * @brief Set_file
     * @param fname name of file to open
     * @return true if file successfully open
     */
    [[nodiscard]] bool Set_file(const std::string_view & fname) override;

    /**
     * @brief Set_history_time_limit
     * @param limit_in_sec limit of seconds of input history stored
     */
    void Set_history_time_limit(int limit_in_sec) override;

    /**
     * @brief Start
     * @note starts execution of Reader thread
     */
    void Start() override;

    /**
     * @brief Stop
     * @note stops execution of Reader thread
     */
    void Stop() override;

    /**
     * @brief Resume
     * @note resumes execution after Stop() was called
     */
    void Resume() override;

    /**
     * @brief Destroy
     * @note destroys thread, after this new thread with Start() can be created
     */
    void Destroy() override;

    ~FileReader();
private:
    double _start_time;
    std::ifstream _file;
    std::string _fname;
    RecordingHistory _data;
    std::thread _thread;
    ReaderState _state;

    //states
    std::atomic_bool _destroyed;
    std::atomic_bool _destroy_flag;
    std::atomic_bool _stop_flag;
    std::atomic_bool _running;

    /**
     * @brief running_loop
     * @note loop in which Reader reads data
     */
    void running_loop(void);
};