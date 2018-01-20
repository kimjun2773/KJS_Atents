#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>
#include <memory>
#include <functional>
#include <atomic>

using namespace std;

//클라이언트에 룸클래스를 프랜드 선언해주기 위해 전방선언해준다.
class Room;

static const int MAX_BUFFER = 1024;

enum class eIOType {
    IO_SEND,
    IO_RECV
};

struct SOCKETINFO {
    /// <summary> IOCP에서 사용되는 overlapped정보구조체 </summary>
    WSAOVERLAPPED overlapped;

    /// <summary> IOCP에서 사용되는 버퍼 정보 구조체 </summary>
    WSABUF dataBuffer;

    /// <summary> 사용된 버퍼 </summary>
    char messageBuffer[MAX_BUFFER];

    /// <summary> 사용된 싸이즈를 기록 </summary>
    DWORD bufferSize;

    eIOType type;
    void init(eIOType type) {
        this->type = type;
        this->bufferSize = 0;

        memset(this->messageBuffer, 0x00, MAX_BUFFER);
        ZeroMemory(&this->overlapped, sizeof(this->overlapped));

        this->dataBuffer.len = MAX_BUFFER;
        this->dataBuffer.buf = this->messageBuffer;
    }
};

class Client : public std::enable_shared_from_this<Client> {
  public:
    int ID;
    string name;

    inline Client(SOCKET socket) {
        this->socket = socket;

        socketinfoRecv.init(eIOType::IO_RECV);
        socketinfoSend.init(eIOType::IO_SEND);

        funcRecv = std::bind(&Client::Recv, this, std::placeholders::_1);
        funcSend = std::bind(&Client::Send, this, std::placeholders::_1);

        PostRecv();
    }

    ~Client() {
        printf("%d 삭제\n", socket);
        closesocket(socket);
    }

    /// <summary> 읽기 쓰기 다른 객체로 넘기기 </summary>
    inline void SetFunc(std::function< void(SOCKETINFO*)> recv, std::function< void(WSABUF&)> send) {
        funcRecv = recv;
        funcSend = send;
    }

    /// <summary> 읽기 쓰기 처리 다시 가져오기 </summary>
    inline void ReturnFunc() {
        funcRecv = std::bind(&Client::Recv, this, std::placeholders::_1);
        funcSend = std::bind(&Client::Send, this, std::placeholders::_1);
    }

    /// <summary> 외부 호출용  Recv </summary>
    inline void ActionRecv(SOCKETINFO* socketinfoRecv) {
        funcRecv(socketinfoRecv);
        PostRecv();
        return;
    }

    /// <summary> 외부 호출용 Send </summary>
    inline void ActionSend(WSABUF& sendbuffer) {
        funcSend(sendbuffer);
        return;
    }

  protected :
    //프랜드 선언
    friend Room;

    //허락된 클래스 이외에서 사용할수 없도록 한다.

    /// <summary> 내부 사용 Send 로직</summary>
    inline virtual void Send(WSABUF& sendbuffer) {
        socketinfoSend.dataBuffer = sendbuffer;
        socketinfoSend.bufferSize = sendbuffer.len;
        ZeroMemory(&socketinfoSend.overlapped, sizeof(socketinfoSend.overlapped));
        PostSend();
    }

    /// <summary> 내부 사용 Recv 로직</summary>
    inline virtual void Recv(SOCKETINFO* socketinfoRecv) {
        if (socketinfoRecv->bufferSize == 0) {
            closesocket(socket);
        } else {
            printf("TRACE - Receive message : %s (%d bytes)\n", socketinfoRecv->dataBuffer.buf, socketinfoRecv->dataBuffer.len);
            PostSend();
        }
    }

  private:

    SOCKETINFO socketinfoRecv;
    SOCKETINFO socketinfoSend;

    DWORD flags = 0;
    SOCKET socket;

    /// <summary> 데이터 읽기관리 객체 이동을 위한 함수 포인터 </summary>
    std::function< void(SOCKETINFO*)> funcRecv;

    /// <summary> 데이터 쓰기관리 객체 이동을 위한 함수 포인터 </summary>
    std::function< void(WSABUF&)> funcSend;

  private:

    /// <summary> 실제 Recv </summary>
    inline void PostRecv() {
        socketinfoRecv.dataBuffer.len = MAX_BUFFER;
        socketinfoRecv.bufferSize = 0;
        if (WSARecv(socket, &(socketinfoRecv.dataBuffer), 1, &socketinfoRecv.bufferSize, &flags, &socketinfoRecv.overlapped, NULL) == SOCKET_ERROR) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
            }
        }
    }

    /// <summary> 실제 Send </summary>
    inline void PostSend() {
        if (WSASend(socket, &(socketinfoSend.dataBuffer), 1, &socketinfoSend.bufferSize, 0, &socketinfoSend.overlapped, NULL) == SOCKET_ERROR) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
            }
        }
    }
};