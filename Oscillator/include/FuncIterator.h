#pragma
#include <functional>

namespace Dummy
{

/**
 * @enum FuncType
 * @note defines which function type to implement
 */
enum class FuncType
{
    SIN,
    SQUARE,
    RANDOM
};

/**
 * @class FuncIterator
 * @note Iterator with implemented function that overload of incrementation operator
 * that return next value.
 */
class FuncIterator
{
public:
    /**
     * @brief FuncIterator constructor
     * @param func function that return value depending of value passed to this function
     * @param step value of one step
     * @param steps_per_cycle number of steps until cycle repeats
     * @param _max_value output value amplifier
     */
    FuncIterator(std::function<double(double)> func, float step, int steps_per_cycle,
        int _max_value);
    
    /**
     * @overload operator++()
     * @brief operator++()
     * @return next value of function
     */
    double operator++();
private:
    const int _steps_per_cycle;
    const float _max_value;
    const float _step;
    std::function<double(double)> _func;
    double _step_cnt;
};

/**
 * @brief Create_Func
 * @param type type of function to create
 * @param point_per_sec number of points per second
 * @param repeats_per_sec number of cycle repeats per second
 * @return Function iterator with desired function
 */
FuncIterator Create_Func(const FuncType type, int point_per_sec, int repeats_per_sec,
    float max_value);


} //namespace Dummy