#pragma once

#include <filesystem>
#include <cassert>

#include <fmt/core.h>

class Filepath
{
public:
    static std::string FindAssetFolder()
    {
        std::string directory{};

        directory = std::filesystem::current_path().string();

        bool foundBuildPath = false;
        bool foundAssetPath = false;

        while (!foundAssetPath)
        {
            foundBuildPath = std::filesystem::exists((directory + "\\Build"));
            foundAssetPath = std::filesystem::exists((directory + "\\Data"));

            if (!foundAssetPath && !foundBuildPath)
            {
                const size_t last_slash_idx = directory.rfind('\\');

                assert(std::string::npos != last_slash_idx && "Couldn't find Asset Folder");

                directory = directory.substr(0, last_slash_idx);
            }

            if (foundBuildPath)
            {
                directory = directory + "\\Build\\Data\\Assets";
                foundAssetPath = true;
            }
            else if (foundAssetPath)
            {
                directory = directory + "\\Data\\Assets";
            }
        }

        fmt::print("{}", directory);

        return directory;
    }

public:
    static std::string assetFolderDirectory;
};