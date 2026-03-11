#include "Serializer.h"

#include <fmt/core.h>

#include <fstream>
#include <iostream>

#include <string>

template<typename T>
bool Serializer::Serialize(const std::filesystem::path& _Path, const char* _Name, T _Object)
{
    std::string path = _Path.string();

    auto [data, out] = zpp::bits::data_out();

    auto result = out(_Object);

    if (failure(result)) {
        fmt::println("Failed to Serialize {}", typeid(_Object).name());
        return false;
    }

    std::ofstream outfile(path.append(_Name), std::ios::out | std::ios::binary);
    outfile.write(reinterpret_cast<char*>(data.data()), data.size());

    return true;
}

template<typename T>
bool Serializer::Serialize(const std::filesystem::path& _Path, T _Object)
{
    std::string path = _Path.string();

    auto [data, out] = zpp::bits::data_out();

    auto result = out(_Object);

    if (failure(result)) {
        fmt::println("Failed to Serialize {}", typeid(_Object).name());
        return false;
    }

    std::ofstream outfile(path, std::ios::out | std::ios::binary);
    outfile.write(reinterpret_cast<char*>(data.data()), data.size());

    return true;
}

template<typename T>
bool Serializer::Deserialize(const std::filesystem::path& _Path, const char* _Name, T& _Object)
{
    std::string path = _Path.string();

    auto [data, in] = zpp::bits::data_in();

    std::string deserializedFile;
    std::ifstream serializedFile;
    serializedFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        serializedFile.open(path.append(_Name));

        std::stringstream ShaderStream;
        ShaderStream << serializedFile.rdbuf();
        serializedFile.close();

        deserializedFile = ShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        fmt::println("{}", e.what());
    }

    data.resize(deserializedFile.size());
    memcpy(&data[0], &deserializedFile[0], deserializedFile.size());

    auto result = in(_Object);

    if (failure(result)) {
        fmt::println("Failed to Deserialize {}", typeid(_Object).name());
        return false;
    }

    return true;
}

template<typename T>
bool Serializer::Deserialize(const std::filesystem::path& _Path, T& _Object)
{
    std::string path = _Path.string();

    auto [data, in] = zpp::bits::data_in();

    std::string deserializedFile;
    std::ifstream serializedFile;
    serializedFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        serializedFile.open(path);

        std::stringstream ShaderStream;
        ShaderStream << serializedFile.rdbuf();
        serializedFile.close();

        deserializedFile = ShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        fmt::println("{}", e.what());
    }

    data.resize(deserializedFile.size());
    memcpy(&data[0], &deserializedFile[0], deserializedFile.size());

    auto result = in(_Object);

    if (failure(result)) {
        fmt::println("Failed to Deserialize {}", typeid(_Object).name());
        return false;
    }

    return true;
}

bool Serializer::HasSerializedExtension(const std::filesystem::path& _Path)
{
    return _Path.extension() == ".Scroll";
}
