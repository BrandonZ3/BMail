#pragma once
#include <Windows.h>
#include "Strings.h"

class StringArray
{
public:
    char** items = NULL;
    int count = 0;
    int size = 0;

    StringArray(const StringArray&) = delete;
    StringArray(unsigned int initialcount);
    int Add(char* string);
    void FreeEverything();
    void FreeStrings();
    void FreeItemArray();
    void Remove(int index);
    void RemoveAndFree(int index);
    StringArray* CloneMe(int startIndex, int count);
};

StringArray* Splitter(const char* string, const char* seperator);
