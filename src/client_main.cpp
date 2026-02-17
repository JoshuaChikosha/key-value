#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <ip> <port>\n";
        return EXIT_FAILURE;
    }

    const char* ip = argv[1];
    int port = std::atoi(argv[2]);
    if (port <= 0 || port > 65535) {
        std::cerr << "bad port\n";
        return EXIT_FAILURE;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {  // convert "127.0.0.1" string to binary
        std::cerr << "bad ip\n";
        close(sock);
        return EXIT_FAILURE;
    }

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return EXIT_FAILURE;
    }

    // read line from user, send to server, print response
    std::string line;
    char buf[1024];

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
            break;
        if (line == "quit" || line == "exit")
            break;

        line += "\n";
        write(sock, line.c_str(), line.size());

        ssize_t n = read(sock, buf, sizeof(buf) - 1);  // leave space for null terminator
        if (n <= 0)
            break;
        buf[n] = '\0';
        std::cout << buf;
    }

    close(sock);
    return EXIT_SUCCESS;
}
