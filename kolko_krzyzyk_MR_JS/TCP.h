#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include "Connection.h"
#include "Game.h"



void runTcpServer(unsigned short port)
{
  // Create a server socket to accept new connections

  // Listen to the given port for incoming connections
  if (listener.listen(port) != sf::Socket::Done)
    return;
  std::cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;

  // Wait for a connection
  if (listener.accept(socket) != sf::Socket::Done)
    return;
  std::cout << "Client connected: " << socket.getRemoteAddress() << std::endl;

  // Send a message to the connected client
  const char out[] = "Hi, I'm the server";
  if (socket.send(out, sizeof(out)) != sf::Socket::Done)
    return;
  std::cout << "Message sent to the client: \"" << out << "\"" << std::endl;

  // Receive a message back from the client
  if (socket.receive(in_server, sizeof(in_server), received_server) != sf::Socket::Done)
    return;
  std::cout << "Answer received from the client: \"" << in_server << "\"" << std::endl;
}


void runTcpClient(unsigned short port)
{
  // Ask for the server address
  do
  {
    std::cout << "Type the address or name of the server to connect to: ";
    std::cin >> server;
  } while (server == sf::IpAddress::None);

  // Create a socket for communicating with the server

  // Connect to the server
  if (socket.connect(server, port) != sf::Socket::Done)
    return;
  std::cout << "Connected to server " << server << std::endl;

  // Receive a message from the server
  if (socket.receive(in_client, sizeof(in_client), received_client) != sf::Socket::Done)
    return;
  std::cout << "Message received from the server: \"" << in_client << "\"" << std::endl;

  // Send an answer to the server
  const char out[] = "Hi, I'm a client";
  if (socket.send(out, sizeof(out)) != sf::Socket::Done)
    return;
  std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;

}
