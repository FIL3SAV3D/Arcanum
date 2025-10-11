#pragma once
#include <vector>
#include <string>

class Cubemap
{
public:
	unsigned int textureID{ 0 };
public:
	Cubemap(const char* _cubemapPath);
	~Cubemap();

private:
    const std::vector<std::string> faces
    {
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"
    };
};