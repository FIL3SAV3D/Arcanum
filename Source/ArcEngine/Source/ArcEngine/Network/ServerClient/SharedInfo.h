#pragma once
#include <cstdint>

const uint16_t UpdateFrequency = 60;

enum class ServerClientMsg : std::uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	ServerMessageClient,
	ServerMessageAll,
	ServerMessageAllExceptSender,
	ServerParseUserCommand,
	ServerparseServerCommand,
	ServerparseClientCommand,
};

enum class UserCommand : std::uint32_t
{
	SpawnEntity,
	MoveEntity,
	DestroyEntity
};

enum class ServerCommand : std::uint32_t
{
	ServerTODO
};

enum class ClientCommand : std::uint32_t
{
	ClientDisconnect
};