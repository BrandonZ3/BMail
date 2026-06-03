#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>

#include "../../Library/Bytes.h"
#include "../../Library/PointerList.h"
#include "../../Library/DBuffer.h"
#include "../../Library/Strings.h"
#include "../../Library/SMTP.h"

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    const char* syshelp = "211 Common help\r\n";
    const char* helpmsg = "214 Command Specific Help\r\n";
    const char* standard = "220 Ready To Receive\r\n";
    const char* closing = "221 thisdomain.com Bye\r\n";
    const char* ok = "250 thisdomain.com OK\r\n";
    const char* usrntlcl = "251 User Not Local\r\n";
    const char* cantvrfy = "252 Cant VRFY User, But Attempting Delivery\r\n";
    const char* startdata = "354 Start Sending Data\r\n";
    const char* servnotav = "421 Service Not Available?\r\n";
    const char* actnot = "450 Action Not Taken\r\n";
    const char* actabrt = "451 Local Error\r\n";
    const char* actstor = "452 Insufficient Storage\r\n";
    const char* cantaccom = "455 Cant Accommodate Parameters\r\n";
    const char* syntaxno = "500 Bad Syntax\r\n";
    const char* syntaxargs = "501 Syntax Of Args\r\n";
    const char* nocommand = "502 Command Not Implemented\r\n";
    const char* badseq = "503 Bad Sequence\r\n";
    const char* paramnoimp = "504 Command Parameter Not Implemented\r\n";
    const char* actnoterr = "550 Error Version of 450\r\n";
    const char* notlcl = "551 User Not Local Try <forward-path>\r\n";
    const char* notstrg = "552 Exceeded Storage Allocation\r\n";
    const char* notmalbna = "553 Mailbox name not allowed\r\n";
    const char* tranfail = "554 Transaction Failed ?No SMTP service here WTF?\r\n";
    const char* paramshuh = "555 MAIL FROM/RCPT To Parameters not recognized or not implemented\r\n";


    WSADATA Dat;
    WSAStartup(0x0202, &Dat);

    SOCKET client = NULL;
    sockaddr_in addr = {};
    int length = sizeof(addr);

    char buffer[1024] = { 0 };

    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server)
    {
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(8081);

        int b = bind(server, (const sockaddr*)&addr, sizeof(addr));

        int r = listen(server, 128);
        while (true)
        {
            char* sender = NULL;
            PointerList* receivers = new PointerList();
            DBuffer* data = new DBuffer(100);
            SMTPProcessState state = SMTPProcessState::NOTCONNECTED;

            client = accept(server, (sockaddr*)&addr, &length);

            state = SMTPProcessState::READY;

            send(client, standard, strlen(standard), 0);

            linger lin = linger();
            int linlength = sizeof(linlength);
            lin.l_onoff = 1;
            lin.l_linger = 5;

            DWORD timeout = 10000;

            int isg = setsockopt(client, SOL_SOCKET, SO_LINGER, (const char*)&lin, linlength);
            isg = setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(DWORD));

            DBuffer* recieveBuffer = NULL;

            if (client)
            {
                while (true)
                {
                    bool tooLarge = false;
                    int len = 1024;

                    if (recieveBuffer == NULL)
                        recieveBuffer = new DBuffer(4096);

                    if (recieveBuffer)
                    {
                        while (len == 1024)
                        {

                            len = recv(client, buffer, 1024, 0);

                            if (len > 0)
                            {
                                recieveBuffer->Add((unsigned char*)buffer, len);

                                if (recieveBuffer->count > 16000000)
                                {
                                    tooLarge = true;
                                    delete recieveBuffer;
                                    recieveBuffer = NULL;
                                    break;
                                }
                            }
                        }

                        recieveBuffer->Add((uint8_t)0);
                        size_t count = Strings::CountOf((const char*)recieveBuffer->DataPointer(0), "\r\n");
                        recieveBuffer->count--;

                        if (count >= 1)
                        {
                            PointerList* messages = PointerList::SplitString((const char*)recieveBuffer->DataPointer(0), "\r\n");
                            bool killConnection = false;

                            for (size_t i = 0; i < messages->count; i++)
                            {
                                PointerList* parts = PointerList::SplitString((const char*)messages->items[i], " ");

                                if (state != SMTPProcessState::DATA)
                                {
                                    if (Strings::CompareCaseInsensitive((const char*)parts->items[0], "HELO"))
                                    {
                                        if (sender)
                                            free(sender);

                                        receivers->FreeEverything();
                                        delete receivers;
                                        receivers = new PointerList();

                                        delete data;
                                        data = new DBuffer(100);

                                        send(client, ok, strlen(ok), 0);
                                        state = SMTPProcessState::READY;


                                        parts->FreeEverything();
                                        delete parts;
                                        continue;
                                    }

                                    if (Strings::CompareCaseInsensitive((const char*)parts->items[0], "NOOP"))
                                    {
                                        send(client, ok, strlen(ok), 0);

                                        parts->FreeEverything();
                                        delete parts;
                                        continue;
                                    }

                                    if (Strings::CompareCaseInsensitive((const char*)parts->items[0], "RSET"))
                                    {
                                        if (sender)
                                            free(sender);

                                        receivers->FreeEverything();
                                        delete receivers;
                                        receivers = new PointerList();

                                        delete data;
                                        data = new DBuffer(100);

                                        send(client, ok, strlen(ok), 0);
                                        state = SMTPProcessState::READY;


                                        parts->FreeEverything();
                                        delete parts;
                                        continue;
                                    }

                                    if (Strings::CompareCaseInsensitive((const char*)parts->items[0], "QUIT"))
                                    {
                                        if (sender)
                                            free(sender);

                                        receivers->FreeEverything();
                                        delete receivers;

                                        delete data;

                                        send(client, closing, strlen(closing), 0);
                                        killConnection = true;

                                        parts->FreeEverything();
                                        delete parts;
                                        break;
                                    }
                                }

                                if (state == SMTPProcessState::READY)
                                {
                                    bool stateStepped = false;

                                    if (Strings::CompareCaseInsensitive((const char*)parts->items[0], "MAIL"))
                                    {
                                        bool countGood = true;
                                        bool syntaxGood = true;

                                        if (parts->count != 2)
                                        {
                                            countGood = false;
                                            send(client, syntaxno, strlen(syntaxno), 0);
                                        }

                                        if (countGood)
                                        {
                                            
                                            if (Bytes::MatchBytes((unsigned char*)parts->items[1], 6, (unsigned char*)"FROM:<") && ((const char*)parts->items[1])[strlen((const char*)parts->items[1]) - 1] == '>' && Strings::CharCount((const char*)parts->items[1], '<') == 1 && Strings::CharCount((const char*)parts->items[1], '>') == 1)
                                            {
                                                size_t indexStart = Strings::IndexOf((const char*)parts->items[1], "<", 1) + 1;
                                                size_t indexClose = Strings::IndexOf((const char*)parts->items[1], ">", 1);

                                                size_t size = (indexClose - indexStart);

                                                sender = (char*)malloc(size + 1);
                                                sender[size] = 0;

                                                size_t pos = 0;

                                                for (size_t i = indexStart; i < indexClose; i++)
                                                {
                                                    sender[pos] = ((const char*)parts->items[1])[i];
                                                    pos++;
                                                }

                                                printf("Sender:%s\r\n", sender);
                                            }
                                            else
                                            {
                                                syntaxGood = false;
                                                send(client, syntaxno, strlen(syntaxno), 0);
                                            }
                                        }

                                        if (countGood && syntaxGood)
                                        {
                                            send(client, ok, strlen(ok), 0);
                                            state = SMTPProcessState::RCPT;
                                        }

                                        parts->FreeEverything();
                                        delete parts;
                                        continue;
                                    }

                                    if (Strings::CompareCaseInsensitive((const char*)parts->items[0], "RCPT"))
                                    {
                                        send(client, badseq, strlen(badseq), 0);
                                        parts->FreeEverything();
                                        delete parts;
                                        continue;
                                    }

                                    if (Strings::CompareCaseInsensitive((const char*)parts->items[0], "DATA"))
                                    {
                                        send(client, badseq, strlen(badseq), 0);
                                        parts->FreeEverything();
                                        delete parts;
                                        continue;
                                    }
                                }

                                if (state == SMTPProcessState::RCPT)
                                {
                                    bool stateStepped = false;

                                    if (Strings::CompareCaseInsensitive((const char*)parts->items[0], "RCPT"))
                                    {
                                        bool countGood = true;
                                        bool syntaxGood = true;

                                        if (parts->count != 2)
                                        {
                                            countGood = false;
                                            send(client, syntaxno, strlen(syntaxno), 0);
                                        }

                                        if (countGood)
                                        {
                                            if (Bytes::MatchBytes((unsigned char*)parts->items[1], 4, (unsigned char*)"TO:<") && ((char*)parts->items[1])[strlen((const char*)parts->items[1]) - 1] == '>' && Strings::CharCount((const char*)parts->items[1], '<') == 1 && Strings::CharCount((const char*)parts->items[1], '>') == 1)
                                            {
                                                size_t indexStart = Strings::IndexOf((const char*)parts->items[1], "<", 1) + 1;
                                                size_t indexClose = Strings::IndexOf((const char*)parts->items[1], ">", 1);

                                                size_t size = (indexClose - indexStart);

                                                char* rec = (char*)malloc(size + 1);
                                                rec[size] = 0;

                                                size_t pos = 0;

                                                for (size_t i = indexStart; i < indexClose; i++)
                                                {
                                                    rec[pos] = ((char*)parts->items[1])[i];
                                                    pos++;
                                                }

                                                receivers->AddPointer(rec);
                                            }
                                            else
                                            {
                                                syntaxGood = false;
                                                send(client, syntaxno, strlen(syntaxno), 0);
                                            }
                                        }

                                        if (countGood && syntaxGood)
                                        {
                                            send(client, ok, strlen(ok), 0);
                                        }

                                        parts->FreeEverything();
                                        delete parts;
                                        continue;
                                    }

                                    if (Strings::CompareCaseInsensitive((const char*)parts->items[0], "MAIL"))
                                    {
                                        send(client, badseq, strlen(badseq), 0);
                                        parts->FreeEverything();
                                        delete parts;
                                        continue;
                                    }

                                    if (Strings::CompareCaseInsensitive((const char*)parts->items[0], "DATA"))
                                    {
                                        state = SMTPProcessState::DATA;
                                        send(client, startdata, strlen(startdata), 0);
                                        parts->FreeEverything();
                                        delete parts;
                                        continue;
                                    }
                                }

                                if (state == SMTPProcessState::DATA)
                                {
                                    recieveBuffer->Add((uint8_t)0);
                                    size_t count = Strings::CountOf((const char*)recieveBuffer->DataPointer(0), "\r\n");
                                    data->Add((unsigned char*)recieveBuffer->DataPointer(0), strlen((const char*)recieveBuffer->DataPointer(0)));
                                    recieveBuffer->count--;

                                    data->Add((uint8_t)0);

                                    if (Strings::Contains((const char*)data->DataPointer(0), "\r\n.\r\n"))
                                    {
                                        printf("Receivers:\r\n");
                                        for (int i = 0; i < receivers->count; i++)
                                        {
                                            printf("%s\r\n", receivers->items[i]);
                                        }
                                        state = SMTPProcessState::READY;
                                        data->count -= 6; //Remove "\r\n.\r\n0"
                                        data->Add((uint8_t)0);
                                        printf("%s\r\n\r\n", data->DataPointer(0));
                                        send(client, ok, strlen(ok), 0);
                                    }

                                    data->count--;

                                    if (parts)
                                    {
                                        parts->FreeEverything();
                                        delete parts;
                                    }
                                    continue;
                                }

                                if (parts)
                                {
                                    parts->FreeEverything();
                                    delete parts;
                                }
                            }

                            messages->FreeEverything();
                            delete messages;
                            messages = NULL;

                            if (killConnection)
                            {
                                delete recieveBuffer;
                                recieveBuffer = NULL;
                                printf("Connection Closing\r\n");
                                closesocket(client);
                                break;
                            }
                        }
                        recieveBuffer->Clear();
                    }
                }
            }
        }
        closesocket(server);
        WSACleanup();
    }
}
