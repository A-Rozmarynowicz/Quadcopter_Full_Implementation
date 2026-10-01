#pragma once

#include "Configuration.hpp"
#include <array>

template <typename T, std::size_t N>
class Buffer
{
public:
    T& operator[](std::size_t i);

private:
    std::array<T, N> buffer;
};


template <typename T, std::size_t N>
class Queue
{
public:
    bool push(const T& value);
    bool pop(T& value);
    T& operator[](std::size_t i);
    bool empty() const;
    bool full() const;
    std::size_t size() const;
    constexpr std::size_t capacity() const;
private:
    Buffer<T, N> buffer;
    std::size_t head = 0;
    std::size_t tail = 0;
    std::size_t count = 0;
};

template <typename T1, typename T2, std::size_t N>
class Double_Queue
{
public:
    bool pop(T1& v1, T2& v2);
    bool push(const T1& v1, const T2& v2);
    bool empty() const;
    bool full() const;
    bool validate() const;
    std::size_t size() const;
    constexpr std::size_t capacity() const;
private:
    Queue<T1, N> queue_1;
    Queue<T2, N> queue_2;
};

#include "Buffers.tpp"