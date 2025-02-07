
#pragma region Server Without Simulation

#include <iostream>
#include <winsock2.h>
#include <unordered_map>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

#define PORT 54000
#define BUFFER_SIZE 1024

#pragma region Server With Simulation
#include "Server.h"
int main() 
{
	Server server = Server();
	server.Run();
}
#pragma endregion
