#include "Buffer.h"

Buffer::Buffer(unsigned int initialcount)
{
    this->size = initialcount;
    this->buffer = (unsigned char*)malloc(this->size * sizeof(char));
}

int Buffer::Add(char character)
{
    if (this->count + 1 == this->size)
    {
        this->size *= 2;

        unsigned char* test = (unsigned char*)realloc(this->buffer, this->size * sizeof(char));

        if (test)
        {
            this->buffer = test;
            this->buffer[this->count] = character;
            this->count++;
            return this->count - 2;
        }
        else
            return -1;
    }
    else
    {
        this->buffer[this->count] = character;
        this->count++;
    }
}

bool Buffer::Add(char* buffer, int length)
{
    if (this->count + length >= this->size)
    {
        this->size = (this->size * 2) + length;

        unsigned char* test = (unsigned char*)realloc(this->buffer, this->size * sizeof(char));

        if (test)
        {
            this->buffer = test;

            for (int i = 0; i < length; i++)
            {
                this->buffer[this->count] = buffer[i];
                this->count++;
            }
            return true;
        }
        else
            return false;
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            this->buffer[this->count] = buffer[i];
            this->count++;
        }
        return true;
    }
}

void Buffer::Clear()
{
    this->count = 0;
}

Buffer::~Buffer()
{
    free(this->buffer);
}

Buffer* BufferFromHexString(const char* string)
{
    Buffer* buf = new Buffer(4096);

    size_t stringLen = strlen(string);

    if (stringLen % 2 == 0 && IsHexString(string, stringLen))
    {
        for (size_t i = 0; i < stringLen; i += 2)
        {
            unsigned char val = 0;
            if ((string[i] >= 0x30 && string[i] <= 0x39))
            {
                val += (string[i] - 0x30) * 16;
            }
            else if (string[i] >= 0x41 && string[i] <= 0x46)
            {
                val += ((string[i] - 0x41) + 10) * 16;
            }
            else if (string[i] >= 0x61 && string[i] <= 0x66)
            {
                val += ((string[i] - 0x61) + 10) * 16;
            }

            if ((string[i + 1] >= 0x30 && string[i + 1] <= 0x39))
            {
                val += (string[i + 1] - 0x30);
            }
            else if (string[i + 1] >= 0x41 && string[i + 1] <= 0x46)
            {
                val += (string[i + 1] - 0x41) + 10;
            }
            else if (string[i + 1] >= 0x61 && string[i + 1] <= 0x66)
            {
                val += (string[i + 1] - 0x61) + 10;
            }

            buf->Add(val);
        }
    }

    return buf;
}