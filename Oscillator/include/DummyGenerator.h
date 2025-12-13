#pragma once
#include <string>
#include <thread>
#include <fstream>
#include <atomic>
#include "FuncIterator.h"

namespace Dummy
{

/**
 * @class Generator
 * @note This class is imitates oscilloscope output, by generating signal provided
 * by FuncIterator, with resolution (how many time points per second). Generator creates
 * temporary file with data and waits until craeted file is removed. When file is removed
 * generator creates new file with data.
 */
class Generator final
{
public:
    Generator(Dummy::FuncIterator & func, int resolution, const std::string_view & tmp_fname = std::string("tmp"));

    /**
     * @brief Start
     * @note starts execution of Generator thread
     */
    void Start();

    /**
     * @brief Stop
     * @note stops execution of Generator thread
     */
    void Stop();

    /**
     * @brief Resume
     * @note resumes execution after Stop() was called
     */
    void Resume();

    /**
     * @brief Destroy
     * @note destroys thread, after this new thread with Start() can be created
     */
    void Destroy();

    ~Generator();
private:
    std::ofstream _ofile;
    std::thread _thread;
    Dummy::FuncIterator _func_iter;
    int _resolution;
    std::string _fname;

    //states
    std::atomic_bool _destroyed;
    std::atomic_bool _destroy_flag;
    std::atomic_bool _stop_flag;
    std::atomic_bool _running;

    /**
     * @brief running_loop
     * @note loop in which Generator creates data
     */
    void running_loop(void);
};

} //namespace Dummy