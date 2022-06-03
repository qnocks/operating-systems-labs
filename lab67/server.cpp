// Copyright 2022 qnocks

#include <arpa/inet.h>
#include <dirent.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

const char *DIRECTORY = ".";

int initServer(int socket, sockaddr_in *server, int port) {
  server->sin_family = AF_INET;
  server->sin_port = htons(port);
  server->sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(socket, reinterpret_cast<sockaddr *>(server), sizeof(*server)) < 0) {
    perror("Error occurred during binding socket");
    return 1;
  }

  return 0;
}

std::string receive(int socket, const sockaddr_in &server, int serverSize) {
  char buffer[512];
  recvfrom(socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&server,
           reinterpret_cast<socklen_t *>(&serverSize));
  std::string bufferString(buffer);
  return bufferString;
}

void send(int socket, const sockaddr_in &server, const std::string &message) {
  int responseCode =
      sendto(socket, message.c_str(), message.size() + 1, 0,
             reinterpret_cast<const sockaddr *>(&server), sizeof(server));

  if (responseCode < 0) {
    std::cout << "Error occurred during sending." << std::endl;
  }
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

std::string getDir() {
  std::string files;
  struct dirent *entry;
  DIR *dir = opendir(DIRECTORY);
  if (dir == nullptr) {
    std::cout << "Failed to open dir" << std::endl;
    return files;
  }

  while ((entry = readdir(dir)) != nullptr) {
    files += entry->d_name;
    files += "\n";
  }

  closedir(dir);
  return files;
}

void transferFile(int socket, const sockaddr_in &server,
                  const std::string &fileName, int_least64_t fileSize) {
  char *buffer = new char[fileSize];

  std::ifstream file;
  file.open(fileName, std::ios::binary);
  file.read(buffer, fileSize);

  sendto(socket, buffer, fileSize, 0,
         reinterpret_cast<const sockaddr *>(&server), sizeof(server));

  file.close();
  delete[] buffer;
}

int processGetCommand(int socket, const sockaddr_in &server,
                      const std::string &request) {
  std::string files = getDir();
  std::vector<std::string> filesNames = split(files, '\n');
  std::string fileName = split(request, ' ')[1];

  if (std::find(filesNames.begin(), filesNames.end(), fileName) !=
      filesNames.end()) {
    std::string response = "0";
    send(socket, server, response);

    std::ifstream file;
    file.open(fileName, std::ios::binary);
    file.ignore(std::numeric_limits<std::streamsize>::max());
    file.close();
    std::streamsize fileSize = file.gcount();
    response = std::to_string(fileSize);
    send(socket, server, response);

    transferFile(socket, server, fileName, fileSize);
    return 0;
  } else {
    std::string response = "1";
    send(socket, server, response);
    return 0;
  }
}

int processRequest(const std::string &request, int socket,
                   const sockaddr_in &server) {
  std::cout << request << std::endl;
  if (request == "dir") {
    std::string files = getDir();
    send(socket, server, files);
    return 0;
  } else if (request.rfind("get", 0) == 0) {
    processGetCommand(socket, server, request);
  } else if (request == "quit") {
    std::string response = "0";
    send(socket, server, response);
    return 1;
  }

  return 0;
}

bool isValidPort(int port) {
  return port >= 1024 && port <= 65535;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "There's should be 1 command line argument - port"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  int port = std::atoi(argv[1]);
  if (!isValidPort(port)) {
    std::cout << "Port should be integer between 1024 and 65355" << std::endl;
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server;
  int serverSize;
  int out = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (out < 0) {
    perror("Error occurred while socket initialization");
    exit(EXIT_FAILURE);
  }

  if (initServer(out, &server, port) == 1) {
    perror("Error occurred while server initialization");
    exit(EXIT_FAILURE);
  }

  while (true) {
    std::string request = receive(out, server, serverSize);
    if (processRequest(request, out, server) == 1) {
      break;
    }
  }

  close(out);
  return 0;
}
