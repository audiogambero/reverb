#pragma once
#include <memory>
#include <cmath>

template<typename T>
class CircularBuffer
{
public:

    void create(unsigned int length)
    {
        _writeIdx = 0;
        _bufSize = (unsigned int) (pow(2, ceil(log(length) / log(2))));
        _wrapMask = _bufSize - 1;

        _buf.reset(new T[_bufSize]());
        flush();
    }

    void flush()
    {
        for(int i = 0; i < _bufSize; i++) _buf[i] = 0.0;
    }

    void write(T input)
    {
        _buf[_writeIdx++] = input;
        _writeIdx &= _wrapMask;
    }

    T read(unsigned int delaySamples)
    {
        int readIdx = _writeIdx - delaySamples;
        readIdx &= _wrapMask;
        return _buf[readIdx];
    }

    T read(double delayFractional, bool interpolate)
    {
        if (!interpolate) {
            return read((int)delayFractional);
        }

        double fraction = delayFractional - (int)delayFractional;
        int delayInt = (int)delayFractional;

        T y0 = read(delayInt - 1);
        T y1 = read(delayInt);     // P1
        T y2 = read(delayInt + 1); // P2
        T y3 = read(delayInt + 2); // P3

        return 0.5f * (
            (y2 - y0) * fraction +
            (2.0f * y0 - 5.0f * y1 + 4.0f * y2 - y3) * fraction * fraction +
            (-y0 + 3.0f * y1 - 3.0f * y2 + y3) * fraction * fraction * fraction +
            2.0f * y1
        );
    }

private:
    std::unique_ptr<T[]> _buf;
    unsigned int _writeIdx;
    unsigned int _bufSize;
    unsigned int _wrapMask;
};