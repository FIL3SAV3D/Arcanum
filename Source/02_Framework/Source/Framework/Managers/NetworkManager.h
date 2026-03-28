#pragma once

#include <memory>

class NetworkManager
{
public:
     NetworkManager() = default;
    ~NetworkManager() = default;

public:
    void Create();
    void Destroy();

public:
    void StartServer();
    void StopServer();

    void StartClient();
    void StopClient();

private:
   //std::unique_ptr<>
};