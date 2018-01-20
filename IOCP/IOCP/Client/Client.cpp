
#include "./../Network/Network.h"
#include <iostream>

int main() {
    try {
        Network net;
        net.StartClient(PF_INET, 8888, "127.0.0.1");

        while (true) {
            char messageBuffer[1024];

            int i = -1;
            while (i++ < 1024) {
                messageBuffer[i] = getchar();
                if (messageBuffer[i] == '\n') {
                    messageBuffer[i++] = '\0';
                    break;
                }
            }

            net.Send(messageBuffer, i);
            net.Recv();
        }
    } catch (char* e) {
        std::cout << e << std::endl;
    }

    return 0;
}

