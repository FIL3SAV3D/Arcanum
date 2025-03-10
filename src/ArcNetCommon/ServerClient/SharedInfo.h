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
	ClientUpdate,
};

const uint16_t UpdateFrequency = 60;