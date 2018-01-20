#include "Network.h"
#include <stdio.h>

HANDLE Network::hIOCP;

void Network::StartServer(int _family, int _port, char _ip[]) {
    SetServerAddr(_family, _port, _ip);
    Bind();
    Listen();
    Accept();
}

void Network::StartClient(int _family, int _port, char _ip[]) {
    SetServerAddr(_family, _port, _ip);
    Connect();
}

Network::Network() {
    Network::OpenNet();
}

Network::~Network() {
    Network::CloseNet();
}

void Network::SetServerAddr(int _family, int _port, char _ip[]) {
    if (_ip == nullptr) {
        serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    } else {
        serverAddr.sin_addr.S_un.S_addr = inet_addr(_ip);
    }
    serverAddr.sin_family = _family;
    serverAddr.sin_port = _port;
}

void Network::OpenNet() {
    int addrLen = sizeof(SOCKADDR_IN);

    memset(&serverAddr, 0, addrLen);

    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        throw "Error - Can not load 'winsock.dll' file\n";
    }

    netSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (netSocket == INVALID_SOCKET) {
        throw "Error - Invalid socket\n";
    }
}

void Network::CloseNet() {
    closesocket(netSocket);
    WSACleanup();
}

void Network::Bind() {
    if (::bind(netSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
        CloseNet();
        throw "Error - Fail to bind\n";
    }
}

void Network::Listen() {
    if (listen(netSocket, 5) == SOCKET_ERROR) {
        CloseNet();
        throw "Error - Fail to listen\n";
    }
}

void Network::Connect() {
    if (connect(netSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        CloseNet();
        throw "Error - Fail to connect\n";
    } else {
        printf("Server Connected\n* Enter Message\n->");
    }
}

void Network::Accept() {

    // ��Ŀ������ ����
    // - CPU * 2��
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    static size_t threadCount = systemInfo.dwNumberOfProcessors * 2;
    unsigned long threadId;

    hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, hIOCP, 0, 1);

    //IOCP ����� �Ϸ� ó�� ������
    for (size_t i = 0; i < threadCount; i++) {
        // - thread ����
        CreateThread(NULL, 0, [](void* data) ->DWORD {
            Network* myThis = (Network*)data;

            //���� ���� ������ �޾ƿ���
            DWORD receiveBytes;
            //��Ʈ ���鶧 ���޵Ȱ� (Ŭ���̾�Ʈ �ڵ��� �� �ִ�.)
            DWORD completionKey;
            //��Ʈ ���鶧 ���޵Ȱ� (��Ĺ ������ �����Ͱ� ���ִ�.)
            struct SOCKETINFO* eventSocket;

            while (true) {
                try {
                    // ����� �Ϸ� ���
                    // ����� IOCP�ڵ�, ���ŵ� ����Ʈ����, CreateIoCompletionPort ���� �Է��Ѱ�, WSASend or WSARecv ���� �ѱ� �ּҰ�
                    if (GetQueuedCompletionStatus(hIOCP, &receiveBytes, (PULONG_PTR)&completionKey, (LPOVERLAPPED *)&eventSocket, INFINITE) == 0) {
                        printf("Error - GetQueuedCompletionStatus Failure\n");
                        myThis->clients.erase(completionKey);
                        continue;
                    }

                    //���� ���� ������ ����ġ�� ����
                    eventSocket->dataBuffer.len = eventSocket->bufferSize = receiveBytes;

                    printf("Thread Playing %d \n",GetCurrentThreadId() );

                    //�Ϸ�� Ÿ�� Ȯ���ϰ� �б⵿���̸� �� Ŭ���̾�Ʈ�� ����
                    if (eventSocket->type == eIOType::IO_RECV) {
                        myThis->clients[completionKey]->ActionRecv(eventSocket);
                    } else {

                    }
                } catch (const std::exception& e) {
                    printf(e.what());
                }
            }
        }, this, 0, &threadId);
    }

    //���� ���Ӵ�� ������
    CreateThread(NULL, 0, [](void* data) ->DWORD {
        Network* myThis = (Network*)data;

        while (true) {

            //Ŭ���̾�Ʈ ���� ���
            int addrLen = sizeof(SOCKADDR_IN);
            SOCKADDR_IN clientAddr;
            memset(&clientAddr, 0, addrLen);
            SOCKET client = accept( myThis->netSocket, (struct sockaddr *)&clientAddr, &addrLen);

            if (client == INVALID_SOCKET) {
                printf("Error - Accept Failure\n");
                continue;
            }

            //Ŭ���̾�Ʈ�� �����ϸ� Ŭ���̾�Ʈ ��ü�� �������ش�.
            auto tCleint = myThis->clients[client] = std::make_shared<Client>(client);

            //Ŭ���̾�Ʈ�� �뿡 ���ӽ�Ų��.
            //myThis->room.AddClinet(tCleint);

            //Ŭ���̾�Ʈ�� IOCP��Ʈ�� �������ش�.
            myThis->hIOCP = CreateIoCompletionPort((HANDLE)client, myThis->hIOCP, (DWORD)client, 1);
        }
    }, this, 0, NULL);
}

void Network::Recv() {
    CreateThread(NULL, 0, [](void* data) ->DWORD {
        Network* myThis = (Network*)data;

        myThis->sizeBuffer = recv(myThis->netSocket, myThis->messageBuffer, MAX_BUFFER, 0);
        if (myThis->sizeBuffer > 0) {
            printf("TRACE - Receive message : %s (%d bytes)\n* Enter Message\n->", myThis->messageBuffer, myThis->sizeBuffer);
        }
        return 0;

    }, this, 0, NULL);
}

int Network::Send(char* _msg, const int _len) {
    int sendBytes = send(netSocket, _msg, _len, 0);
    if (sendBytes > 0) {
        printf("TRACE - Send message : %s (%d bytes)\n", _msg, _len);
    }
    return sendBytes;
}