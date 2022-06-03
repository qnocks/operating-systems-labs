// Copyright 2022 qnocks

#include <WS2tcpip.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

const char *DIRECTORY =
    "C:/Users/romao/Documents/study/os/lab6/os-task6-qnocks/";

void initWinsock() {
  WSADATA data;
  int winSockCode = WSAStartup(MAKEWORD(2, 2), &data);
  if (winSockCode != 0) {
    std::cout << "Error occurred during init WinSock " << winSockCode;
    exit(EXIT_FAILURE);
  }
}

void initServer(sockaddr_in *server, const char *ip, int port) {
  server->sin_family = AF_INET;
  server->sin_port = htons(port);
  inet_pton(AF_INET, ip, &server->sin_addr);
}

void send(SOCKET socket, const sockaddr_in &server,
          const std::string &message) {
  char *buffer = new char[message.length() + 1];
  std::snprintf(buffer, message.length() + 1, message.c_str());
  buffer[message.length() + 1] = '\0';

  int responseCode =
      sendto(socket, buffer, message.length() + 1, 0,
             reinterpret_cast<const sockaddr *>(&server), sizeof(server));
  if (responseCode == SOCKET_ERROR) {
    std::cout << "Error while sending " << WSAGetLastError() << std::endl;
  }

  delete[] buffer;
}

std::string receive(SOCKET socket) {
  char buffer[512] = "";
  recvfrom(socket, buffer, 512, 0, nullptr, nullptr);
  std::string bufferString(buffer);
  return bufferString;
}

std::vector<std::string> split(const std::string &unparsedString,
                               char delimiter) {
  std::vector<std::string> splittedString;
  std::stringstream stream(unparsedString);
  std::string item;

  while (std::getline(stream, item, delimiter)) {
    splittedString.push_back(item);
  }

  return splittedString;
}

std::string sendCommand(SOCKET socket, const sockaddr_in &server) {
  std::string command;
  std::string fileName;
  getline(std::cin, command);
  if (command.rfind("get", 0) == 0) {
    fileName = split(command, ' ')[1];
  }

  send(socket, server, command);
  return fileName;
}

void retrieveFile(SOCKET socket, int fileBytes, const std::string &fileName) {
  std::ofstream file(DIRECTORY + fileName, std::ios::binary);
  if (!file.is_open()) {
    std::cout << "Couldn't open file to retrieve data" << std::endl;
  }

  char *buffer = new char[fileBytes];
  recvfrom(socket, buffer, fileBytes, 0, nullptr, nullptr);
  file.write(buffer, fileBytes);
  file.close();

  delete[] buffer;
}

int processResponse(SOCKET socket, const std::string &fileName) {
  std::string response = receive(socket);
  std::cout << response << std::endl;

  if (fileName.length() && response == "0") {
    std::string fileBytes = receive(socket);
    std::cout << fileBytes << std::endl;
    retrieveFile(socket, std::stoi(fileBytes), fileName);
  } else if (response == "0") {
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "Few arguments were passed" << std::endl;
    exit(1);
  }

  const char *ip = argv[1];
  int port = std::atoi(argv[2]);

  initWinsock();
  SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);
  sockaddr_in server{};

  initServer(&server, ip, port);
  while (true) {
    std::string fileName = sendCommand(out, server);
    if (processResponse(out, fileName) == 1) {
      break;
    }
  }

  return 0;
}
