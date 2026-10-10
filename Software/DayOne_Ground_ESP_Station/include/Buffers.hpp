#ifndef BUFFERS_HPP
#define BUFFERS_HPP

#include <cstddef>
#include <array>

template <typename T, std::size_t N>
class Buffer
{
public:
    T& operator[](std::size_t i);
    constexpr std::size_t capacity() const;
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
    void flush();
    std::size_t size() const;
    constexpr std::size_t capacity() const;
    std::size_t get_head_offset_index(std::size_t offset) const;
private:
    Buffer<T, N> buffer;
    std::size_t head = 0;
    std::size_t tail = 0;
    std::size_t count = 0;
};


template <typename T, std::size_t N>
class Stack
{
public:
    bool push(const T& value);
    bool pop(T& value);

    T& operator[](std::size_t i);

    bool empty() const;
    bool full() const;

    void flush();

    std::size_t size() const;
    constexpr std::size_t capacity() const;

private:
    T buffer[N];
    std::size_t count = 0;
};

#include "Buffers.tpp"

#endif