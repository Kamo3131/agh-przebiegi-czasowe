#include "FuncIterator.h"
#include <cmath>
#include <random>
#include <iostream>

using namespace Dummy;

const static double pi = std::acos(-1);

/**
 * @brief square
 * @param step step
 * @param transition_point value of step after which will transition from low to high
 */
double square(double step, double transition_point)
{
    if (step < transition_point)
        return 0.0;
    else
        return 1.0;
}

/**
 * @brief Create_Func
 * @param type type of function to create
 * @param point_per_sec number of points per second
 * @param repeats_per_sec number of cycle repeats per second
 * @return Function iterator with desired function
 */
FuncIterator Dummy::Create_Func(const FuncType type, int point_per_sec, int repeats_per_sec,
    float max_value)
{
    int points_per_repeat = point_per_sec / repeats_per_sec;
    std::function<double(double)> func;
    float step = 0.f;
    const double interval_time = 1.0; // 1 seconds
    
    switch (type)
    {
        case FuncType::SIN:
                            func =  (double(*)(double))&sin;
                            step = (2.0 * pi) / points_per_repeat;
                            break;
        case FuncType::SQUARE:
                            {
                            float transition_point =(points_per_repeat / 2.0);
                            func = std::bind(square, std::placeholders::_1, transition_point);
                            step = 1.0;
                            break;
                            }
        case FuncType::RANDOM:
                            {
                            std::random_device r_device;
                            std::seed_seq my_seed{r_device(), r_device(), r_device()};
                            std::mt19937 gen(my_seed);

                            func = [gen](double step) mutable -> double {
                                constexpr double max_unif_val = 1.0;
                                constexpr double min_unif_val = -1.0;
                                std::uniform_real_distribution<double> dist(min_unif_val, max_unif_val);
                                return dist(gen);
                            };
                            break;
                            }
        default:
                //error
                break;
    }

    return FuncIterator(func, step, points_per_repeat, max_value);
}

/**
 * @brief FuncIterator constructor
 * @param func function that return value depending of value passed to this function
 * @param step value of one step
 * @param steps_per_cycle number of steps until cycle repeats
 * @param _max_value output value amplifier
 */
FuncIterator::FuncIterator(std::function<double(double)> func, float step, int steps_per_cycle,
         int max_value) : _steps_per_cycle(steps_per_cycle), _max_value(max_value),
         _step(step), _func(func)
{
    _step_cnt = 0.0;
}

/**
 * @overload operator++()
 * @brief operator++()
 * @return next value of function
 */
double FuncIterator::operator++()
{
    double val = _func(_step_cnt);
    _step_cnt += _step;
    if (_step_cnt >= _steps_per_cycle)
    {
        _step_cnt = 0.0;
    }
    return _max_value * val;
}