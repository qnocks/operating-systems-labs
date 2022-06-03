// Copyright 2022 qnocks

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

std::string receive(int socket, const sockaddr_in &server, int serverSize) {
  char buffer[512];
  recvfrom(socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&server,
           reinterpret_cast<socklen_t *>(&serverSize));
  std::string bufferString(buffer);
  return bufferString;
}

void sendCommand(const std::string &command, int socket,
                 const sockaddr_in &server, int serverSize) {
  std::cout << "Sending to server: " << command << std::endl;
  sendto(socket, command.c_str(), command.size() + 1, 0,
         reinterpret_cast<const sockaddr *>(&server), sizeof(server));
  std::cout << "Server response: " << receive(socket, server, serverSize)
            << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "There's should be 1 command line argument - port"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  int port = std::atoi(argv[1]);
  struct sockaddr_in server;
  int serverSize;
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  std::string dirCommand = "dir";
  std::string quitCommand = "quit";

  if (sock < 0) {
    perror("Error occurred during socket initialization");
    exit(EXIT_FAILURE);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");

  sendCommand(dirCommand, sock, server, serverSize);
  sendCommand(quitCommand, sock, server, serverSize);

  close(sock);
  return 0;
}
