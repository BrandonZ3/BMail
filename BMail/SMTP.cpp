#include "SMTP.h"

SMTPRequest::SMTPRequest(char* buffer)
{
    StringArray* myArray = Splitter(buffer, "\r\n");
    StringArray* command = Splitter(myArray->items[0], " ");
    if (
        Contains(command->items[0], "HELO") ||
        Contains(command->items[0], "MAIL") ||
        Contains(command->items[0], "RCPT") ||
        Contains(command->items[0], "DATA") ||
        Contains(command->items[0], "SEND") ||
        Contains(command->items[0], "SOML") ||
        Contains(command->items[0], "SAML") ||
        Contains(command->items[0], "RSET") ||
        Contains(command->items[0], "VRFY") ||
        Contains(command->items[0], "EXPN") ||
        Contains(command->items[0], "HELP") ||
        Contains(command->items[0], "NOOP") ||
        Contains(command->items[0], "QUIT") ||
        Contains(command->items[0], "TURN"))
    {
        this->method = command->items[0];
    }
    myArray->FreeEverything();
    delete myArray;
    command->FreeItemArray();
    delete command;
}

SMTPRequest::~SMTPRequest()
{
    free(this->method);
}