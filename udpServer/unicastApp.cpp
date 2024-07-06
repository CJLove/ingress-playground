#include "UdpSocket.h"
#include <iostream>
#ifdef _WIN32
    #include "getopt.h"
#else
    #include <unistd.h>
#endif
#include <thread>

class UnicastApp {
public:
    // UDP Multicast
    UnicastApp(const char *remoteAddr, const char *listenAddr, uint16_t localPort, uint16_t port);

    virtual ~UnicastApp() = default;

    void onReceiveData(const char *data, size_t size, std::string &srcAddr, uint16_t srcPort) ;

    void sendMsg(const char *data, size_t len);

private:
    sockets::SocketOpt m_socketOpts;
    sockets::UdpSocket<UnicastApp> m_unicast;
};

UnicastApp::UnicastApp(const char *remoteAddr, const char *listenAddr, uint16_t localPort, uint16_t port) : m_socketOpts({ sockets::TX_BUFFER_SIZE, sockets::RX_BUFFER_SIZE, listenAddr}), m_unicast(*this, &m_socketOpts) {
    sockets::SocketRet ret = m_unicast.startUnicast(remoteAddr, localPort, port);
    if (ret.m_success) {
        std::cout << "Listening on UDP " << listenAddr << ":" << localPort << " sending to " << remoteAddr << ":" << port << std::endl;
    } else {
        std::cout << "Error: " << ret.m_msg << std::endl;
        exit(1); // NOLINT
    }
}

void UnicastApp::sendMsg(const char *data, size_t len) {
    auto ret = m_unicast.sendMsg(data, len);
    if (!ret.m_success) {
        std::cout << "Send Error: " << ret.m_msg << std::endl;
    }
}

void UnicastApp::onReceiveData(const char *data, size_t size, std::string &srcAddr, uint16_t srcPort) {
    std::string str(reinterpret_cast<const char *>(data), size);

    std::cout << "Received: " << str << " from " << srcAddr << ":" << srcPort << std::endl;
    m_unicast.sendMsgTo(data, size, srcAddr, srcPort);
}

void usage() {
    std::cout << "UnicastApp -a <remoteAddr> -l <localPort> -p <port> -L <listenAddr>" << std::endl;
}

int main(int argc, char **argv) {
    int arg = 0;
    const char *addr = nullptr;
    const char *listenAddr = "0.0.0.0";
    uint16_t port = 0;
    uint16_t localPort = 0;
    while ((arg = getopt(argc, argv, "a:l:p:L:?")) != EOF) {    // NOLINT
        switch (arg) {
        case 'a':
            addr = optarg;
            break;
        case 'l':
            localPort = static_cast<uint16_t>(std::stoul(optarg));
            break;
        case 'p':
            port = static_cast<uint16_t>(std::stoul(optarg));
            break;
        case 'L':
            listenAddr = optarg;
            break;
        case '?':
            usage();
            exit(1);    // NOLINT
        }
    }

    auto *app = new UnicastApp(addr, listenAddr, localPort, port); // NOLINT

    while (true) {

        std::this_thread::sleep_for(std::chrono::seconds(30));
    }

    delete app;
}