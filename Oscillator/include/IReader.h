#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "RecordingContainers.h"

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
     * @brief Set_file
     * @param fname name of file to open
     * @return true if file successfully open
     */
    [[nodiscard]] virtual bool Set_file(const std::string_view & fname) = 0;

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