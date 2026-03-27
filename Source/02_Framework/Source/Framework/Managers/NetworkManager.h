#pragma once

#include <memory>
#include "ArcEngine/Network/Net/"

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
    std::unique_ptr<>
};