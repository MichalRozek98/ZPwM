#pragma once
#include <SFML/Network.hpp>
#include <iostream>

const unsigned short port = 50001;
char who;

sf::TcpSocket socket;
sf::TcpListener listener;
sf::IpAddress server;

char in_server[128];
std::size_t received_server;

char in_client[128];
std::size_t received_client;


