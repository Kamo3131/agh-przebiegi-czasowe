#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "RecordingContainers.h"

/**
 * @enum ReaderState
 * @note defines in which state Reader is
 */
enum class ReaderState
{
    CREATED,
    WAITING,
    READING,
    STOPPED,
    DESTROYED
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
     * @return RecordingHistory with all timestamps collected
     */
    [[nodiscard]] virtual  const RecordingHistory& Get_data() const = 0;
    
    /**
     * @brief Get_state
     * @return state of the reader
     */
    [[nodiscard]] virtual ReaderState Get_state() const = 0;

    /**
     * @brief Check_if_new_data_loaded
     * @return true if new data after last call to this function was loaded
     */
    [[nodiscard]] virtual bool Check_if_new_data_loaded() = 0;

    /**
     * @brief Set_file
     * @param fname name of file to open
     * @return true if file successfully open
     */
    [[nodiscard]] virtual bool Set_file(const std::string_view & fname) = 0;

    /**
     * @brief Set_history_time_limit
     * @param limit_in_sec limit of seconds of input history stored
     */
    virtual void Set_history_time_limit(int limit_in_sec) = 0;

    /**
     * @brief Start
     * @note starts execution of Reader thread
     */
    virtual void Start() = 0;

    /**
     * @brief Stop
     * @note stops execution of Reader thread
     */
    virtual void Stop()  = 0;

    /**
     * @brief Resume
     * @note resumes execution after Stop() was called
     */
    virtual void Resume()  = 0;

    /**
     * @brief Destroy
     * @note destroys thread, after this new thread with Start() can be created
     */
    virtual void Destroy()  = 0;

    virtual ~IReader() {}
};