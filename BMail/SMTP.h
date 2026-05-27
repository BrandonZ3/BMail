#pragma once
#include <Windows.h>
#include "Strings.h"
#include "StringArray.h"

class SMTPRequest
{
public:
    char* method = NULL;
    SMTPRequest(const SMTPRequest&) = delete;
    SMTPRequest(char* buffer);
    ~SMTPRequest();
};

class SMTPResponse
{
private:
    unsigned short status = 400;
    char* responseString = NULL;

public:
    SMTPResponse(const SMTPResponse&) = delete;
};

enum SMTPProcessState
{
    NOTCONNECTED = 0,
    READY = 1,
    MAIL = 2,
    RCPT = 3,
    DATA = 4,
};
