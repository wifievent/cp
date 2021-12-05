#include "sslserver.h"

class EchoSsl : public SslServer
{
public:
    EchoSsl();
    ~EchoSsl() {}

protected:
    void handleClnt(SslClientSocket* clntsock) override;
};

EchoSsl::EchoSsl() {
}

void EchoSsl::handleClnt(SslClientSocket* clntsock) {
    char buffer[BUFSIZE];
    char message[] = "HI, this is server";
    int len = 0;
    int count = 0;
    while((len = clntsock->recv(buffer, BUFSIZE)) != -1) {
        if(len == 0) {
            spdlog::info("clntsock is shutdown");
            return;
        }
        spdlog::info(count++);
        spdlog::info("recv data from client");
        spdlog::info(buffer);
        clntsock->send(message, strlen(message) + 1);
    }
    return;
}

int main()
{
    EchoSsl server;
    server.start(9090, "/home/kali/echossl-test/certkey-test/server.crt", "/home/kali/echossl-test/certkey-test/server.key");
    server.stop();
    return 0;
}
