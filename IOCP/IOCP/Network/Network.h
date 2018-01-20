#pragma once

#include "Client.h"
#include "Room.h"
#include <map>
#include <list>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

//PostQueuedCompletionStatus

class Network {
  private:

  public:
    Network();
    ~Network();

    void StartServer(int _family, int _port, char _ip[]);
    void StartClient(int _family, int _port, char _ip[]);

    /// <summary> �б� </summary>
    void Recv();
    /// <summary> ���� </summary>
    int Send(char* _msg, const int _len);

  private:
    WSADATA WSAData;
    SOCKET netSocket;

    SOCKADDR_IN serverAddr;

    /// <summary> �밴ü </summary>
    list<shared_ptr<Room>> rooms;

    /// <summary> ������ Ŭ���̾�Ʈ map </summary>
    map<SOCKET, shared_ptr<Client>> clients;

    /// <summary> �������� ���� IOCP �ڵ鷯 </summary>
    static HANDLE hIOCP;

    /// <summary> ��Ʈ��ũ���� ���Ǵ� �������� </summary>
    char messageBuffer[MAX_BUFFER];
    /// <summary> ���������� ���� ������ </summary>
    int sizeBuffer = 0;

    void SetServerAddr(int _family, int _port, char _ip[]);

    /// <summary> ���� ���� </summary>
    void OpenNet();
    /// <summary> ���� �ݱ� </summary>
    void CloseNet();

    /// <summary> ���� </summary>
    void Listen();
    /// <summary> ���ε� </summary>
    void Bind();
    /// <summary> ����Ʈ </summary>
    void Accept();

    /// <summary> Ŀ��Ʈ </summary>
    void Connect();
};


