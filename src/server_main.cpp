#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);  // AF_INET=ipv4, SOCK_STREAM=tcp
    if (server_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // so we can rebind to port right away after restart (otherwise get "address in use")
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // listen on any interface
    address.sin_port = htons(PORT);        // htons = host to network byte order (big endian)

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    std::cout << "listening on " << PORT << "\n";

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    std::cout << client_ip << " connected\n";

    // echo loop: read from client, send back with ECHO: prefix
    char buffer[BUFFER_SIZE];
    while (true) {
        ssize_t n = read(client_fd, buffer, sizeof(buffer) - 1);  // -1 to leave room for '\0'
        if (n <= 0)
            break;

        buffer[n] = '\0';
        if (n > 0 && buffer[n - 1] == '\n')
            buffer[n - 1] = '\0';

        std::string reply = std::string("ECHO: ") + buffer + "\n";
        if (write(client_fd, reply.c_str(), reply.size()) < 0)
            break;
    }

    close(client_fd);
    close(server_fd);
    return EXIT_SUCCESS;
}
