#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "protocol.h"
#include "kv_store.h"

const int PORT = 12345;

static std::string toUpper(std::string s) {
    for (auto& c : s) c = std::toupper(static_cast<unsigned char>(c));
    return s;
}

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

    KVStore store;  // one store for the whole session - state persists across commands
    while (true) {
        std::string line = readLine(client_fd);
        if (line.empty())
            break;
        // strip \r in case of Windows line endings
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        std::vector<std::string> tokens = tokenize(line);
        if (tokens.empty())
            continue;

        std::string cmd = toUpper(tokens[0]);
        std::string response;
        if (cmd == "HELP") {
            response = "Available commands: SET, GET, DEL, EXISTS, SIZE, KEYS, CLEAR\n";
        } 
        else if (cmd == "GET" && tokens.size() == 2) 
        {
            auto val = store.get(tokens[1]);
            response = val ? (*val + "\n") : "(nil)\n";
        } 
        else if (cmd == "SET" && tokens.size() == 3)
        {
            store.set(tokens[1], tokens[2]);
            response = "OK\n";
        }
        else if (cmd == "DEL" && tokens.size() == 2) 
        {
            response = store.del(tokens[1]) ? "1\n" : "0\n";
        }
        else if (cmd == "EXISTS" && tokens.size() == 2)
        {
            response = store.exists(tokens[1]) ? "1\n" : "0\n";
        }
        else if (cmd == "SIZE")
        {
            response = std::to_string(store.size()) + "\n";
        }
        else if (cmd == "KEYS")
        {
            std::vector<std::string> keys = store.keys();
            for (size_t i = 0; i < keys.size(); ++i) {
                if (i > 0) response += " ";
                response += keys[i];
            }
            response += "\n";
        }
        else if (cmd == "CLEAR")
        {
            store.clear();
            response = "OK\n";
        }
        else {
            response = "ERR unknown command\n";
        }

        if (write(client_fd, response.c_str(), response.size()) < 0)
            break;
    }

    close(client_fd);
    close(server_fd);
    return EXIT_SUCCESS;
}
