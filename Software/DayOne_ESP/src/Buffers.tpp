#include "Buffers.hpp"

template <typename T, std::size_t N>
T& Buffer<T, N>::operator[](std::size_t i)
{
    return buffer[i];
}


template <typename T, std::size_t N>
bool Queue<T, N>::push(const T& value)
{
    if (full())
    {
        return false;
    }

    buffer[tail] = value;
    tail = (tail + 1) % N;
    count++;

    return true;
}

template <typename T, std::size_t N>
bool Queue<T, N>::pop(T& value)
{
    if (empty())
    {
        return false;
    }

    value = buffer[head];
    head = (head + 1) % N;
    count--;

    return true;
}

template <typename T, std::size_t N>
T& Queue<T, N>::operator[](std::size_t i)
{
    return buffer[i];
}

template <typename T, std::size_t N>
bool Queue<T, N>::empty() const
{
    return count == 0;
}

template <typename T, std::size_t N>
bool Queue<T, N>::full() const
{
    return count == N;
}

template <typename T, std::size_t N>
std::size_t Queue<T, N>::size() const
{
    return count;
}