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

    /// <summary> 읽기 </summary>
    void Recv();
    /// <summary> 쓰기 </summary>
    int Send(char* _msg, const int _len);

  private:
    WSADATA WSAData;
    SOCKET netSocket;

    SOCKADDR_IN serverAddr;

    /// <summary> 룸객체 </summary>
    list<shared_ptr<Room>> rooms;

    /// <summary> 접속한 클라이언트 map </summary>
    map<SOCKET, shared_ptr<Client>> clients;

    /// <summary> 전역으로 사용될 IOCP 핸들러 </summary>
    static HANDLE hIOCP;

    /// <summary> 네트워크에서 사용되는 전역버퍼 </summary>
    char messageBuffer[MAX_BUFFER];
    /// <summary> 전역버퍼의 사용된 사이즈 </summary>
    int sizeBuffer = 0;

    void SetServerAddr(int _family, int _port, char _ip[]);

    /// <summary> 소켓 연결 </summary>
    void OpenNet();
    /// <summary> 소켓 닫기 </summary>
    void CloseNet();

    /// <summary> 리슨 </summary>
    void Listen();
    /// <summary> 바인드 </summary>
    void Bind();
    /// <summary> 엑셉트 </summary>
    void Accept();

    /// <summary> 커텍트 </summary>
    void Connect();
};


