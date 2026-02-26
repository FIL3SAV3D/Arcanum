#pragma once

#include <zpp_bits.h>

#include <filesystem>

class Serializer
{
public:
    template<typename T>
    static bool Serialize(const std::filesystem::path& _Path, const char* _Name, T _Object);

    template<typename T>
    static bool Serialize(const std::filesystem::path& _Path, T _Object);

    template<typename T>
    static bool Deserialize(const std::filesystem::path& _Path, const char* _Name, T& _Object);

    template<typename T>
    static bool Deserialize(const std::filesystem::path& _Path, T& _Object);

    static bool HasSerializedExtension(const std::filesystem::path& _Path);

};