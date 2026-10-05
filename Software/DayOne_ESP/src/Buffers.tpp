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

#pragma region Double_Queue

template <typename T1, typename T2, std::size_t N>
bool Double_Queue<T1, T2, N>::pop(T1& v1, T2& v2)
{
    if (!validate() || empty())
    {
        return false;
    }
    bool b1 = queue_1.pop(v1);
    bool b2 = queue_2.pop(v2);
    return b1 && b2;
}

template <typename T1, typename T2, std::size_t N>
bool Double_Queue<T1, T2, N>::push(const T1& v1, const T2& v2)
{
    if (!validate() || full())
    {
        return false;
    }
    bool b1 = queue_1.push(v1);
    bool b2 = queue_2.push(v2);
    return b1 && b2;
}

template <typename T1, typename T2, std::size_t N>
bool Double_Queue<T1, T2, N>::empty() const
{
    bool e1 = queue_1.empty();
    bool e2 = queue_2.empty();
    return e1 && e2;
}

template <typename T1, typename T2, std::size_t N>
bool Double_Queue<T1, T2, N>::full() const
{
    bool f1 = queue_1.full();
    bool f2 = queue_2.full();
    return f1 && f2;
}

template <typename T1, typename T2, std::size_t N>
bool Double_Queue<T1, T2, N>::validate() const
{
    return queue_1.size() == queue_2.size();
}

template <typename T1, typename T2, std::size_t N>
std::size_t Double_Queue<T1, T2, N>::size() const
{
    std::size_t n1 = queue_1.size();
    std::size_t n2 = queue_2.size();
    if (n1 == n2)
    {
        return n1;
    }
    else
    {
        return N;
    }
}

template <typename T1, typename T2, std::size_t N>
constexpr std::size_t Double_Queue<T1, T2, N>::capacity() const
{
    return N;
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