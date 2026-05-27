#include "StringArray.h"



StringArray::StringArray(unsigned int initialcount)
{
    this->size = initialcount;
    this->items = (char**)malloc(this->size * sizeof(char*));
}

int StringArray::Add(char* string)
{
    if (this->count + 1 == this->size)
    {
        this->size *= 2;

        char** test = (char**)realloc(this->items, this->size * sizeof(char*));

        if (test)
        {
            this->items = test;
            this->items[this->count] = string;
            this->count += 1;
            return this->count - 2;
        }
        else
            return -1;
    }
    else
    {
        this->items[this->count] = string;
        this->count += 1;
    }
}

void StringArray::FreeEverything()
{
    FreeStrings();
    FreeItemArray();
}

void StringArray::FreeStrings()
{
    for (int i = 0; i < this->count; i++)
        free(this->items[i]);
}

void StringArray::FreeItemArray()
{
    free(items);
    items = NULL;
}

void StringArray::Remove(int index)
{
    if (index <= this->count - 1 && index > -1)
    {
        for (int i = 1; i + index < this->count; i++)
        {
            this->items[index + (i - 1)] = this->items[index + i];
        }
        this->count--;
    }
}

void StringArray::RemoveAndFree(int index)
{
    if (index <= this->count - 1 && index > -1)
    {
        free(this->items[index]);
        for (int i = 1; i + index < this->count; i++)
        {
            this->items[index + (i - 1)] = this->items[index + i];
        }
        this->count--;
    }
}

StringArray* StringArray::CloneMe(int startIndex, int count)
{
    if (startIndex + count <= this->count)
    {
        StringArray* clone = new StringArray(count * 2);
        clone->count = count;

        int pos = 0;

        for (int i = startIndex; i < startIndex + count; i++)
        {
            clone->items[pos] = CloneString(this->items[i]);
            pos++;
        }
        return clone;
    }
    else
        return NULL;
}

StringArray* Splitter(const char* string, const char* seperator)
{
    StringArray* stringArray = new StringArray(10);

    int stringLength = strlen(string);
    int seperatorLength = strlen(seperator);

    int start = -1;
    int end = -1;

    for (int i = 0; i < stringLength; i++)
    {
        bool found = false;

        for (int x = 0; x < seperatorLength; x++)
        {
            if (string[i + x] == seperator[x])
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

        if (found && end == -1)
        {
            if (start == -1)
            {
                i += seperatorLength - 1;
                continue;
            }
            else
            {
                end = i;

                int length = end - start;

                char* newstring = (char*)malloc(length + 1);
                newstring[length] = 0;

                int pos = 0;

                for (int n = start; n < end; n++)
                {
                    newstring[pos] = string[n];
                    pos++;
                }

                stringArray->Add(newstring);

                i += seperatorLength - 1;
                start = -1;
                end = -1;
            }
        }

        if (start == -1 && !found)
            start = i;

        if (start != -1 && end == -1 && i == stringLength - 1)
        {
            end = i + 1;

            int length = end - start;

            char* newstring = (char*)malloc(length + 1);
            newstring[length] = 0;

            int pos = 0;

            for (int n = start; n < end; n++)
            {
                newstring[pos] = string[n];
                pos++;
            }

            stringArray->Add(newstring);

            i += seperatorLength - 1;
            start = -1;
            end = -1;
        }
    }

    return stringArray;
}
