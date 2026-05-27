#pragma once
#include <Windows.h>


struct Escapers
{
    size_t started;
    size_t ended;
};
Escapers CountEscaped(const char* string);
int FirstIndexOf(const char* string, const char* item);
bool Contains(const char* string, const char* item);
size_t CharCount(const char* string, char character);
size_t StringCount(const char* string, const char* chars);
char* CloneString(char* string);
char* Trim(char* string);
char* Concat(const char* string1, const char* string2);
char* ToLowerString(char* string);
char* ReplaceChar(char* string, char oldchar, char newchar);
bool StringCompare(const char* string1, const char* string2);
bool StringCompareCaseInsensitive(const char* string1, const char* string2);
char* UnsignedToString(unsigned long long value);
bool IsNumeric(const char* string);
unsigned char StringToUnsignedChar(const char* string);
unsigned short StringToUnsignedWord(const char* string);
unsigned long StringToUnsignedDword(const char* string);
char* ConvertBareText(const char* string);
bool IsHexString(const char* string, size_t length);
char* StringToHexString(const char* string);
bool BufferToStringCompareCaseInsensitive(const char* buffer, const char* string);
