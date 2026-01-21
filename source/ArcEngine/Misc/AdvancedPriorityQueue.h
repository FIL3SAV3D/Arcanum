#pragma once

#include <queue>
#include <functional>

template <class _T, class _Container = std::vector<_T>, class _Pr = std::less<typename _Container::value_type>>
class AdvancedPriorityQueue : public std::priority_queue<_T, _Container, _Pr>
{
public:
    bool contains(const _T& value)
    {
        auto it = std::find(this->c.begin(), this->c.end(), value);
        return it != this->c.end();
    }

    bool remove(const _T& value) 
    {
        auto it = std::find(this->c.begin(), this->c.end(), value);

        if (it == this->c.end()) {
            return false;
        }
        if (it == this->c.begin()) {
            // deque the top element
            this->pop();
        }
        else {
            // remove element and re-heap
            this->c.erase(it);
            std::make_heap(this->c.begin(), this->c.end(), this->comp);
        }
        return true;
    }

    _T& popback()
    {
        auto it = this->c.end();
        this->c.erase(it);
        return (*it);
    }

    _Container::iterator begin() { return this->c.begin(); }
    _Container::iterator end() { return this->c.end(); }
    _Container::reverse_iterator rbegin() { return this->c.rbegin(); }
    _Container::reverse_iterator rend() { return this->c.rend(); }

    _Container::const_iterator begin() const { return this->c.begin(); }
    _Container::const_iterator end()	const { return this->c.end(); }
    _Container::const_reverse_iterator rbegin() const { return this->c.rbegin(); }
    _Container::const_reverse_iterator rend() const { return this->c.rend(); }
};