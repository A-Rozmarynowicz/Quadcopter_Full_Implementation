#include "Buffers.hpp"

#pragma region Buffer
template <typename T, std::size_t N>
T& Buffer<T, N>::operator[](std::size_t i)
{
    return buffer[i];
}

template <typename T, std::size_t N>
constexpr std::size_t Buffer<T, N>::capacity() const
{
    return N;
}

#pragma endregion

#pragma region Queue
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
void Queue<T, N>::flush()
{
    head = tail;
    count = 0;
}

template <typename T, std::size_t N>
std::size_t Queue<T, N>::size() const
{
    return count;
}

template <typename T, std::size_t N>
constexpr std::size_t Queue<T, N>::capacity() const
{
    return N;
}

template <typename T, std::size_t N>
std::size_t Queue<T, N>::get_head_offset_index(std::size_t offset) const
{
    return (head + offset) % N;
}

#pragma endregion

#pragma region Stack

template <typename T, std::size_t N>
bool Stack<T, N>::push(const T& value)
{
    if (full())
    {
        return false;
    }

    buffer[count] = value;
    count++;

    return true;
}

template <typename T, std::size_t N>
bool Stack<T, N>::pop(T& value)
{
    if (empty())
    {
        return false;
    }

    count--;
    value = buffer[count];

    return true;
}

template <typename T, std::size_t N>
T& Stack<T, N>::operator[](std::size_t i)
{
    return buffer[i];
}

template <typename T, std::size_t N>
bool Stack<T, N>::empty() const
{
    return count == 0;
}

template <typename T, std::size_t N>
bool Stack<T, N>::full() const
{
    return count == N;
}

template <typename T, std::size_t N>
void Stack<T, N>::flush()
{
    count = 0;
}

template <typename T, std::size_t N>
std::size_t Stack<T, N>::size() const
{
    return count;
}

template <typename T, std::size_t N>
constexpr std::size_t Stack<T, N>::capacity() const
{
    return N;
}

#pragma endregion