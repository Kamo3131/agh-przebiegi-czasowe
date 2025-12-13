#include "RecordingContainers.h"
#include <iostream>

/**
 * @brief Get_recording_params
 * @return parameters of interval recording
 */
RangeParams RecordingVector::Get_recording_params() const
{
    return _params;
}

/**
 * @brief Set_recording_params
 * @param params parameters of recording to be set
 */
void RecordingVector::Set_recording_params(const RangeParams & params)
{
    _params = params;
}

/**
 * @brief Get_container
 * @return vector with timestamps
 * @note readonly
 */
const RecordingVector::type& RecordingVector::Get_container() const
{
    return _data;
}

/**
 * @brief Get_container
 * @return vector with timestamps
 */
RecordingVector::type& RecordingVector::Get_container()
{
    return _data;
}

/**
 * @overload operator[]
 * @brief operator[]
 * @param i index of timestamp
 * @return indexed timestamp
 */
Timestamp RecordingVector::operator[](unsigned i) const
{
    return _data.at(i);
}

/**
 * @brief Set_vector
 * @param vec vector with timestamp to be set for this container
 * @note move semantic
 */
void RecordingVector::Set_vector(type && vec)
{
    _data = std::move(vec);
}

/**
 * @brief Set_vector
 * @param vec vector with timestamp to be set for this container
 * @note copy semantic
 */
void RecordingVector::Set_vector(const type & vec)
{
    _data = vec;
}

/**
 * @brief Clear
 * @note clears vector
 */
void RecordingVector::Clear()
{
    _data.clear();
}

/********************************** RecordingHistory ********************************/

/**
 * @brief Set_history_time_limit
 * @param limit_in_sec how many intervals will be stored at once
 * @note now 1 interval == 1 second
 */
void RecordingHistory::Set_history_time_limit(int limit_in_sec)
{
    _recordingVectors_limit = limit_in_sec;
}

/**
 * @brief Get_recording_params
 * @return parameters of saved history
 */
RangeParams RecordingHistory::Get_recording_params() const
{
    return _params;
}

/**
 * @brief Set_recording_params
 * @param params parameters of recording to be set
 */
void RecordingHistory::Set_recording_params(const RangeParams & params)
{
    _params = params;
}

/**
 * @overload operator[]
 * @brief operator[]
 * @param i index of timestamp
 * @return indexed timestamp
 * @note this operator provides flatted interface without
 * worries of which list or vector is indexed
 */
Timestamp RecordingHistory::operator[](unsigned int i) const
{
    unsigned int points_count = 0;
    if (!_data.empty())
    {
        type::const_iterator iter = _data.begin();
        while (iter != _data.end())
        {
            unsigned int index_limit = iter->Get_recording_params().Get_max_index();
            if (i < points_count + index_limit)
            {
                return (*iter)[i - points_count];
            }
            points_count += index_limit;
            iter++;
        }
    }
    return Timestamp();
}

/**
 * @brief Get_newest_recordingVector
 * @return newest RecordingVector containing new data read
 */
RecordingVector RecordingHistory::Get_newest_recordingVector() const
{
    return _data.back();
}

/**
 * @brief Get_before_newest_recordingVector
 * @return one before newest RecordingVector containing new data read
 */
RecordingVector RecordingHistory::Get_before_newest_recordingVector() const
{
    type::const_iterator iter = _data.end();
    iter--;
    iter--;

    return *iter;
}

/**
 * @brief Get_container
 * @return list of vectors with timestamps
 * @note readonly
 */
const RecordingHistory::type& RecordingHistory::Get_container() const
{
    return _data;
}

/**
 * @brief Pop_recordingVector
 * @return true if pop finished with success
 */
bool RecordingHistory::Pop_recordingVector()
{
    if (_data.empty())
    {
        return false;
    }
    const RangeParams& last_params = _data.front().Get_recording_params();
    _params._max_index -= last_params.Get_max_index();
    _data.pop_front();
    const RangeParams& new_last_params = _data.front().Get_recording_params();
    _params._time_range.first = new_last_params.Get_min_time();
    return true;
}

/**
 * @brief Push_recordingVector
 * @param vec new RecordingVector to save
 * @return true if push finished with success
 * @note move semantic
 */
bool RecordingHistory::Push_recordingVector(RecordingVector && vec)
{
    if (_data.size() >= _recordingVectors_limit)
        Pop_recordingVector();
    _data.push_back(vec);
    const RangeParams new_params = vec.Get_recording_params();
    _params._max_index += new_params.Get_max_index();

    auto [min_voltage, max_voltage] = _params._voltage_range;
    if (min_voltage > new_params.Get_min_voltage())
    {
        _params._voltage_range.first = new_params.Get_min_voltage();
    }
    if (max_voltage < new_params.Get_max_voltage())
    {
        _params._voltage_range.second = new_params.Get_max_voltage();
    }

    _params._time_range.second = new_params.Get_max_time();

    return true;
}

/**
 * @brief Empty
 * @return true if no timestamps saved
 */
bool RecordingHistory::Empty() const
{
    return _data.empty();
}

/**
 * @brief Clear
 * @note clears list and vectors
*/
void RecordingHistory::Clear()
{
    type::iterator iter = _data.begin();
    while (iter != _data.end())
    {
        iter->Clear();
        iter++;
    }
    _data.clear();
}

/**
 * @brief Begin
 * @return iterator to first timestamp
 */
RecordingHistory::iterator RecordingHistory::Begin() const
{
    return RecordingHistory::iterator(this, 0, 0);
}

/**
 * @brief End
 * @return iterator out of range
 */
RecordingHistory::iterator RecordingHistory::End() const
{
    int last_index = 0;
    if (!_data.empty())
    {
        last_index = _data.back().Get_container().size();
    }
    return RecordingHistory::iterator(this, _data.size(), last_index);
}

/******************************************  RecordingHistory::iterator  ******************************/

/**
 * @brief RecordingHistory::iterator::iterator
 * @note constructor to create interator pointing to desired data.
 * @warning Please use only End() and Begin() to create iterators!
 */
RecordingHistory::iterator::iterator(const RecordingHistory * _recHis_ptr,
    int list_index, int timestamp_index)
{
    _data_ptr = _recHis_ptr;
    if (list_index >= _data_ptr->Get_container().size() || list_index < 0)
        _list_it = _data_ptr->Get_container().end();
    else
    {
        _list_it = _data_ptr->Get_container().begin();
        while (list_index > 0)
        {
            _list_it++;
            list_index--;
        }
    }

    if (_list_it == _data_ptr->Get_container().end())
    {
        if (timestamp_index != 0)
        {
            _vec_it = _data_ptr->Get_container().back().Get_container().end();
        }
    }
    else if (timestamp_index >= _list_it->Get_container().size() || timestamp_index < 0)
    {
        _vec_it = _list_it->Get_container().end();
    }
    else
    {
        _vec_it = _list_it->Get_container().begin() + timestamp_index;
    }
}

/**
 * @overload operator*
 * @brief operator*
 * @return timestamp
 */
RecordingHistory::iterator::value_type RecordingHistory::iterator::operator*() const
{
    return *_vec_it;
}

/**
 * @overload operator++
 * @brief operator++
 * @return reference to operator
 * @note increments index
 */
RecordingHistory::iterator& RecordingHistory::iterator::operator++()
{
    if (_list_it != _data_ptr->Get_container().end())
    {
        ++_vec_it;
        if (_vec_it == _list_it->Get_container().end())
        {
            ++_list_it;
            if (_list_it != _data_ptr->Get_container().end())
            {
                _vec_it = _list_it->Get_container().begin();
            }
        }
    }

    return *this;
}

/**
 * @overload operator++(int)
 * @brief operator++(int)
 * @return reference to operator
 * @note increments index
 */
RecordingHistory::iterator RecordingHistory::iterator::operator++(int)
{
    iterator tmp = *this;
    ++*this;
    return tmp;
}

/**
 * @overload operator==
 * @brief operator==
 * @return true if iterators are equl
 */
bool RecordingHistory::iterator::operator==(const RecordingHistory::iterator & iter) const
{
    if (_list_it == iter._list_it && _vec_it == iter._vec_it)
        return true;
    else
        return false;
}

/**
 * @overload operator!=
 * @brief operator!=
 * @return true if iterators are different
 */
bool RecordingHistory::iterator::operator!=(const RecordingHistory::iterator & iter) const
{
    return !(*this == iter);
}