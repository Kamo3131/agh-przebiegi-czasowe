#include "DummyGenerator.h"
#include <chrono>
#include <filesystem>
#include <memory>
#include <iostream>

using namespace Dummy;

Generator::Generator(Dummy::FuncIterator & func, int resolution, const std::string_view & tmp_fname)
    : _func_iter(func), _resolution(resolution), _fname(tmp_fname), _thread()
{
    _destroyed  = false;
    _destroy_flag = false;
    _running = false;
    _stop_flag = false; 
}

/**
 * @brief running_loop
 * @note loop in which Generator creates data
 */
void Generator::running_loop(void)
{
    if (!_running)
    {
        if (std::filesystem::exists(_fname))
        {
            std::filesystem::remove(_fname);
        }
        const std::chrono::duration sleep_time_running = std::chrono::seconds(1);
        const std::chrono::duration sleep_time_stopped = std::chrono::milliseconds(100);
        _running = true;
        _destroyed = false;
        while (!_destroy_flag)
        {
            if (!std::filesystem::exists(_fname))
            {
                if (_stop_flag)
                {
                    std::this_thread::sleep_for(sleep_time_stopped);
                }
                else
                {
                    _ofile.open(_fname);
                    if (_ofile.is_open())
                    {
                        const double interval_sec = 1.0; // 1 second
                        const double step = interval_sec / _resolution;
                        for (int i = 0; i < _resolution; i++)
                        {
                            if (i == _resolution - 1)
                            {
                                _ofile << step * i << " " << ++_func_iter;
                            }
                            else
                            {
                                _ofile << step * i << " " << ++_func_iter << '\n';
                            }
                        }
                        _ofile.close();
                    }
                    std::this_thread::sleep_for(sleep_time_running);
                }
            }
        }
        _running = false;
        _destroyed = true;
    }
}

/**
 * @brief Start
 * @note starts execution of Generator thread
 */
void Generator::Start()
{
    _thread = std::thread(&Generator::running_loop, this);
}

/**
 * @brief Stop
 * @note stops execution of Generator thread
 */
void Generator::Stop()
{
    _stop_flag = true;
}

/**
 * @brief Resume
 * @note resumes execution after Stop() was called
 */
void Generator::Resume()
{
    _stop_flag = false;
}

/**
 * @brief Destroy
 * @note destroys thread, after this new thread with Start() can be created
 */
void Generator::Destroy()
{
    _destroy_flag = true;
    if (_thread.joinable())
        _thread.join();
}

Generator::~Generator()
{
    if (!_destroyed)
    {
        Destroy();
    }
}