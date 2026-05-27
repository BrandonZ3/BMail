#include "Strings.h"


Escapers CountEscaped(const char* string)
{
    Escapers esc = { 0 };

    size_t stringLength = strlen(string);

    for (size_t i = 0; i < stringLength; i++)
    {
        bool found = false;

        if (
            (string[i] == '\\' && string[i + 1] == '\\') ||
            (string[i] == '\\' && string[i + 1] == 'r') ||
            (string[i] == '\\' && string[i + 1] == 'n') ||
            (string[i] == '\\' && string[i + 1] == 't') ||
            (string[i] == '\\' && string[i + 1] == '\'') ||
            (string[i] == '\\' && string[i + 1] == '\"')
            )
        {
            esc.ended += 1;
            esc.started += 1;
        }
        else if (string[i] == '\\')
        {
            esc.started += 1;
        }
    }

    return esc;
}

int FirstIndexOf(const char* string, const char* item)
{
    int stringLength = strlen(string);
    int seperatorLength = strlen(item);

    int start = -1;
    int end = -1;

    for (int i = 0; i < stringLength; i++)
    {
        for (int x = 0; x < seperatorLength; x++)
        {
            if (string[i + x] == item[x])
            {
                if (x == seperatorLength - 1)
                {
                    return i;
                    break;
                }
                else
                    continue;
            }
            else
            {
                break;
            }
        }
    }
    return -1;
}

bool Contains(const char* string, const char* item)
{
    int stringLength = strlen(string);
    int seperatorLength = strlen(item);

    for (int i = 0; i < stringLength; i++)
    {
        bool found = false;

        for (int x = 0; x < seperatorLength; x++)
        {
            if (string[i + x] == item[x])
            {
                if (x == seperatorLength - 1)
                {
                    found = true;
                    break;
                }
                else
                    continue;
            }
            else
            {
                break;
            }
        }

        if (found)
        {
            return true;
        }
    }
    return false;
}

size_t CharCount(const char* string, char character)
{
    size_t output = 0;
    size_t stringLen = strlen(string);

    for (size_t i = 0; i < stringLen; i++)
    {
        if (string[i] == character)
            output++;
    }
    return output;
}

size_t StringCount(const char* string, const char* chars)
{
    size_t output = 0;

    size_t stringLength = strlen(string);
    size_t seperatorLength = strlen(chars);

    for (size_t i = 0; i < stringLength; i++)
    {
        bool found = false;

        for (int x = 0; x < seperatorLength; x++)
        {
            if (string[i + x] == chars[x])
            {
                if (x == seperatorLength - 1)
                {
                    found = true;
                    break;
                }
                else
                    continue;
            }
            else
            {
                break;
            }
        }

        if (found)
        {
            i += seperatorLength - 1;
            output++;
        }
    }

    return output;
}

char* CloneString(char* string)
{
    int stringLength = strlen(string);

    char* newString = (char*)malloc(stringLength + 1);

    newString[stringLength] = 0;

    for (int i = 0; i < stringLength; i++)
        newString[i] = string[i];

    return newString;
}

char* Trim(char* string)
{
    char* result = NULL;
    int stringLength = strlen(string);

    int start = -1;
    int end = -1;

    for (int i = 0; i < stringLength; i++)
    {
        if (start == -1 && string[i] != 0x20)
        {
            start = i;
            break;
        }
    }

    for (int i = stringLength - 1; i >= 0; i--)
    {
        if (end == -1 && string[i] != 0x20)
        {
            end = i;
            break;
        }
    }

    if (start != -1 && end != -1)
    {
        int length = end - start;
        result = (char*)malloc(length + 2);

        int pos = 0;

        for (int i = start; i <= end; i++)
        {
            result[pos] = string[i];
            pos++;
        }

        result[length + 1] = 0;
        return result;
    }
    else if (start == end && start == -1)
    {
        result = (char*)malloc(1);
        result[0] = 0;
        return result;
    }
}

char* Concat(const char* string1, const char* string2)
{
    int l1 = strlen(string1);
    int l2 = strlen(string2);

    char* newString = (char*)malloc(l1 + l2 + 1);

    newString[l1 + l2] = 0;

    int pos = 0;

    for (int i = 0; i < l1; i++)
    {
        newString[pos] = string1[i];
        pos++;
    }

    for (int i = 0; i < l2; i++)
    {
        newString[pos] = string2[i];
        pos++;
    }

    return newString;
}

char* ToLowerString(char* string)
{
    int stringLength = strlen(string);

    char* newString = (char*)malloc(stringLength + 1);

    newString[stringLength] = 0;

    for (int i = 0; i < stringLength; i++)
    {
        if (string[i] > 0x40 && string[i] < 0x5B)
            newString[i] = string[i] + 0x20;
        else
            newString[i] = string[i];
    }

    return newString;
}

char* ReplaceChar(char* string, char oldchar, char newchar)
{
    int stringLength = strlen(string);

    char* newString = (char*)malloc(stringLength + 1);

    newString[stringLength] = 0;

    for (int i = 0; i < stringLength; i++)
    {
        if (string[i] == oldchar)
            newString[i] = newchar;
        else
            newString[i] = string[i];
    }

    return newString;
}

bool StringCompare(const char* string1, const char* string2)
{
    int strLen1 = strlen(string1);
    int strLen2 = strlen(string2);

    if (strLen1 != strLen2)
        return false;

    for (int i = 0; i < strLen1; i++)
        if (string1[i] != string2[i])
            return false;

    return true;
}

bool StringCompareCaseInsensitive(const char* string1, const char* string2)
{
    int strLen1 = strlen(string1);
    int strLen2 = strlen(string2);

    if (strLen1 != strLen2)
        return false;

    for (int i = 0; i < strLen1; i++)
    {
        unsigned char char1 = string1[i];
        unsigned char char2 = string2[i];

        if (char1 > 0x40 && char1 < 0x5B)
        {
            char1 += 0x20;
        }

        if (char2 > 0x40 && char2 < 0x5B)
        {
            char2 += 0x20;
        }

        if (char1 != char2)
        {
            return false;
        }
    }

    return true;
}

char* UnsignedToString(unsigned long long value)
{
    unsigned long long temp = value;
    int size = 0;
    while (temp != 0)
    {
        if (temp != 0)
        {
            size++;
        }
        temp /= 10;
    }

    char* output = (char*)malloc(size + 1);
    output[size] = 0;
    int pos = size - 1;


    while (value != 0)
    {
        if (value != 0)
        {
            output[pos] = value % 10 + 0x30;
            pos--;
        }
        value /= 10;
    }

    return output;
}

bool IsNumeric(const char* string)
{
    size_t stringLength = strlen(string);

    for (size_t i = 0; i < stringLength; i++)
    {
        if (string[i] > 0x39 || string[i] < 0x30)
            return false;
    }

    return true;
}

unsigned char StringToUnsignedChar(const char* string)
{
    unsigned char output = 0;
    size_t stringLength = strlen(string);

    if (!IsNumeric(string))
    {
        return output;
    }

    int size = 0;

    for (size_t i = 0; i < stringLength; i++)
    {
        if (size == 3)
            break;
        output += string[i] - 0x30;
        if (i == stringLength - 1)
            break;
        output *= 10;
        size++;
    }

    return output;
}

unsigned short StringToUnsignedWord(const char* string)
{
    unsigned short output = 0;
    size_t stringLength = strlen(string);

    if (!IsNumeric(string))
    {
        return output;
    }

    int size = 0;

    for (size_t i = 0; i < stringLength; i++)
    {
        if (size == 5)
            break;
        output += string[i] - 0x30;
        if (i == stringLength - 1)
            break;
        output *= 10;
        size++;
    }

    return output;
}

unsigned long StringToUnsignedDword(const char* string)
{
    unsigned long output = 0;
    size_t stringLength = strlen(string);

    if (!IsNumeric(string))
    {
        return output;
    }

    int size = 0;

    for (size_t i = 0; i < stringLength; i++)
    {
        if (size == 11)
            break;
        output += string[i] - 0x30;
        if (i == stringLength - 1)
            break;
        output *= 10;
        size++;
    }

    return output;
}

char* ConvertBareText(const char* string)
{
    size_t stringLen = strlen(string);
    Escapers esc = CountEscaped(string);

    if (esc.started == esc.ended)
    {
        size_t newLen = (stringLen - (esc.ended));

        char* output = (char*)malloc(newLen + 1);
        output[newLen] = 0;
        size_t pos = 0;

        for (size_t i = 0; i < stringLen; i++)
        {
            if (string[i] == '\\' && string[i + 1] == '\\')
            {
                output[pos] = '\\';
                pos++;
                i++;
                continue;
            }

            if (string[i] == '\\' && string[i + 1] == 't')
            {
                output[pos] = '\t';
                pos++;
                i++;
                continue;
            }

            if (string[i] == '\\' && string[i + 1] == '\'')
            {
                output[pos] = '\'';
                pos++;
                i++;
                continue;
            }

            if (string[i] == '\\' && string[i + 1] == '\"')
            {
                output[pos] = '\"';
                pos++;
                i++;
                continue;
            }

            if (string[i] == '\\' && string[i + 1] == 'r')
            {
                output[pos] = '\r';
                pos++;
                i++;
                continue;
            }


            if (string[i] == '\\' && string[i + 1] == 'n')
            {
                output[pos] = '\n';
                pos++;
                i++;
                continue;
            }

            output[pos] = string[i];
            pos++;
        }

        return output;
    }
    return NULL;
}

bool IsHexString(const char* string, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        if (!((string[i] >= 0x30 && string[i] <= 0x39) || (string[i] >= 0x41 && string[i] <= 0x46) || (string[i] >= 0x61 && string[i] <= 0x66)))
        {
            return false;
        }
    }
    return true;
}

char* StringToHexString(const char* string)
{
    char* output = NULL;

    size_t stringLen = strlen(string);
    size_t pos = 0;

    output = (char*)malloc((stringLen * 2) + 1);
    output[stringLen * 2] = 0;

    for (size_t i = 0; i < stringLen; i++)
    {
        unsigned char val = string[i];

        unsigned char low = val % 16;
        unsigned char high = (val / 16) % 16;

        if (low < 10)
        {
            low += 0x30;
        }
        else
        {
            low += (0x61 - 10);
        }

        if (high < 10)
        {
            high += 0x30;
        }
        else
        {
            high += (0x61 - 10);
        }

        output[pos] = high;
        output[pos + 1] = low;
        pos += 2;
    }

    return output;
}

bool BufferToStringCompareCaseInsensitive(const char* buffer, const char* string)
{
    int strLen = strlen(string);

    for (int i = 0; i < strLen; i++)
    {
        unsigned char char1 = buffer[i];
        unsigned char char2 = string[i];

        if (char1 > 0x40 && char1 < 0x5B)
        {
            char1 += 0x20;
        }

        if (char2 > 0x40 && char2 < 0x5B)
        {
            char2 += 0x20;
        }

        if (char1 != char2)
        {
            return false;
        }
    }

    return true;
}
