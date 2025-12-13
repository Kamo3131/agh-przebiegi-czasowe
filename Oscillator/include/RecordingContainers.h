#pragma once
#include <memory>
#include <list>
#include <vector>

/**
 * @struct Timestamp
 * This struct stores info about voltage and time for one point
 */
struct Timestamp
{
    //stores data as pair
    std::pair<double,double> _data;

    explicit Timestamp(const double& time, const double& voltage)
        : _data(std::make_pair(time, voltage)) {}
    
    Timestamp()
        : Timestamp(0.0f, 0.0f) {}

    /**
     * @brief Get_time
     * @return time of timestamp
     */
    [[nodiscard]] double Get_time() const
    {
        return _data.first;
    }
    /**
     * @brief Get_voltage
     * @return voltage of timestamp
     */
    [[nodiscard]] double Get_voltage() const
    {
        return _data.second;
    }
};

/**
 * @struct RangeParams
 * This struct stores info about recordings parameters like:
 * max & min voltage, max & min time, and how many points there are (index)
 */
struct RangeParams
{
    //stores range data as pairs
    std::pair<double, double> _voltage_range;
    std::pair<double, double> _time_range;
    int _max_index;

    RangeParams(const std::pair<double, double> & voltage_range,
        const std::pair<double, double> time_range, int max_index)
        : _voltage_range(voltage_range), _time_range(time_range), _max_index(max_index)
    {
        //Empty
    }

    RangeParams& operator=(const RangeParams&) = default;

    RangeParams()
        : RangeParams({0.0, 0.0}, {0.0, 0.0}, 0)
    {
        //Empty
    }

    /**
     * @brief Get_max_voltage
     * @return max voltage value of recording
     */
    [[nodiscard]] double Get_max_voltage() const
    {
        return _voltage_range.second;
    }

    /**
     * @brief Get_min_voltage
     * @return min voltage value of recording
     */
    [[nodiscard]] double Get_min_voltage() const
    {
        return _voltage_range.first;
    }

    /**
     * @brief Get_max_time
     * @return max time of recording
     */
    [[nodiscard]] double Get_max_time() const
    {
        return _time_range.second;
    }

    /**
     * @brief Get_min_time
     * @return min time of recording
     */
    [[nodiscard]] double Get_min_time() const
    {
        return _time_range.first;
    }

    /**
     * @brief Get_max_index
     * @return number of elements
     */
    [[nodiscard]] int Get_max_index() const
    {
        return _max_index;
    }
};

/**
 * @class RecordingVector
 * This class implements storage for Timestamps of one interval (now 1 seconds)
 */
class RecordingVector
{
public:
    using iterator = std::vector<Timestamp>::iterator;
    using type = std::vector<Timestamp>;
    using value_type = Timestamp;

    RecordingVector() = default;

    /**
     * @brief Get_recording_params
     * @return parameters of interval recording
     */
    [[nodiscard]] RangeParams Get_recording_params() const;

    /**
     * @brief Set_recording_params
     * @param params parameters of recording to be set
     */
    void Set_recording_params(const RangeParams & params);

    /**
     * @brief Get_container
     * @return vector with timestamps
     * @note readonly
     */
    [[nodiscard]] const type& Get_container() const;

    /**
     * @brief Get_container
     * @return vector with timestamps
     */
    [[nodiscard]] type& Get_container();

    /**
     * @overload operator[]
     * @brief operator[]
     * @param i index of timestamp
     * @return indexed timestamp
     */
    [[nodiscard]] Timestamp operator[](unsigned i) const;

    /**
     * @brief Set_vector
     * @param vec vector with timestamp to be set for this container
     * @note move semantic
     */
    void Set_vector(type && vec);

    /**
     * @brief Set_vector
     * @param vec vector with timestamp to be set for this container
     * @note copy semantic
     */
    void Set_vector(const type & vec);

    /**
     * @brief Clear
     * @note clears vector
     */
    void Clear();

private:
    type _data;
    RangeParams _params;
};

/**
 * @class RecordingHistory
 * This class implements storage for intervals containing Timestamps. Limit can be
 * set to manage how many intervals (now 1 interval == 1 second) will be saved. After
 * limit is reach oldest interval is removed and newest saved.
 */
class RecordingHistory
{
public:
    using value_type = RecordingVector;
    using type = std::list<RecordingVector>;

    RecordingHistory() = default;

    /**
     * @brief Set_history_time_limit
     * @param limit_in_sec how many intervals will be stored at once
     * @note now 1 interval == 1 second
     */
    void Set_history_time_limit(int limit_in_sec);

    /**
     * @brief Get_recording_params
     * @return parameters of saved history
     */
    [[nodiscard]] RangeParams Get_recording_params() const;

    /**
     * @brief Set_recording_params
     * @param params parameters of recording to be set
     */
    void Set_recording_params(const RangeParams & params);

    /**
     * @overload operator[]
     * @brief operator[]
     * @param i index of timestamp
     * @return indexed timestamp
     * @note this operator provides flatted interface without
     * worries of which list or vector is indexed
     */
    [[nodiscard]] Timestamp operator[](unsigned int i) const;

    /**
     * @brief Get_newest_recordingVector
     * @return newest RecordingVector containing new data read
     */
    [[nodiscard]] RecordingVector Get_newest_recordingVector() const;

    /**
     * @brief Get_before_newest_recordingVector
     * @return one before newest RecordingVector containing new data read
     */
    [[nodiscard]] RecordingVector Get_before_newest_recordingVector() const;

    /**
     * @brief Get_container
     * @return list of vectors with timestamps
     * @note readonly
     */
    [[nodiscard]] const type& Get_container() const;

    /**
     * @brief Pop_recordingVector
     * @return true if pop finished with success
     */
    bool Pop_recordingVector();

    /**
     * @brief Push_recordingVector
     * @param vec new RecordingVector to save
     * @return true if push finished with success
     * @note move semantic
     */
    bool Push_recordingVector(RecordingVector && vec);

    /**
     * @brief Empty
     * @return true if no timestamps saved
     */
    [[nodiscard]] bool Empty() const;

    /**
     * @brief Clear
     * @note clears list and vectors
    */
    void Clear();

    /**
     * @class iterator
     * This is forward iterator that provides flatted interface without
     * worries of which list or vector is indexed using overload method like
     * operator++, operator* and operator==, operator!=.
     */
    class iterator
    {
    public:
        using list_iter = type::const_iterator;
        using vec_iter = type::value_type::type::const_iterator;
        using value_type = Timestamp;
        iterator(const RecordingHistory * _recHis_ptr, int list_index, int timestamp_index);
        iterator(const iterator &) = default;
        iterator& operator=(const iterator&) = default;

        /**
         * @overload operator*
         * @brief operator*
         * @return timestamp
         */
        [[nodiscard]] value_type operator*() const;

        /**
         * @overload operator++
         * @brief operator++
         * @return reference to operator
         * @note increments index
         */
        iterator& operator++();

        /**
         * @overload operator++(int)
         * @brief operator++(int)
         * @return reference to operator
         * @note increments index
         */
        iterator operator++(int);

        /**
         * @overload operator==
         * @brief operator==
         * @return true if iterators are equl
         */
        [[nodiscard]] bool operator==(const iterator &) const;

        /**
         * @overload operator!=
         * @brief operator!=
         * @return true if iterators are different
         */
        [[nodiscard]] bool operator!=(const iterator &) const;
    private:
        const RecordingHistory * _data_ptr;
        list_iter _list_it;
        vec_iter _vec_it;
    };

    /**
     * @brief Begin
     * @return iterator to first timestamp
     */
    [[nodiscard]] iterator Begin() const;

    /**
     * @brief End
     * @return iterator out of range
     */
    [[nodiscard]] iterator End() const;
private:
    type _data;
    RangeParams _params;
    int _recordingVectors_limit {30};  //One recording == 1 second of history
};
