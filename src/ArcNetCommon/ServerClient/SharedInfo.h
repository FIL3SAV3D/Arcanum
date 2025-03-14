#pragma once
#include <cstdint>

enum class ServerClientMsg : std::uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,

	SpawnEntity,
	NewUser,
	UserSync,
	ServerSync,

	RelayUserDisconnect,
	UserDisconnect,
};