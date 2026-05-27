#pragma once
#include "Windows.h"
#include "Strings.h"

class Buffer
{
public:
    unsigned char* buffer = NULL;
    int count = 0;
    int size = 0;

    Buffer(const Buffer&) = delete;

    Buffer(unsigned int initialcount);
    int Add(char character);
    bool Add(char* buffer, int length);
    void Clear();
    ~Buffer();
};

Buffer* BufferFromHexString(const char* string);