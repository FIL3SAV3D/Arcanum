#pragma once
#include <vector>
#include <string>

class HDRCubemap
{
public:
    unsigned int envCubemap;
    unsigned int irradianceMap;
    unsigned int prefilterMap;

public:
    HDRCubemap(const char* _cubemapPath, const unsigned int& _sizex, const unsigned int& _sizey);
	~HDRCubemap();

private:
    unsigned int hdrTexture;
    unsigned int captureFBO, captureRBO;

    unsigned int envCubemapSize = 1024;
    unsigned int irradianceMapSize = 64;


    void LoadHDRTexture(const char* _cubemapPath, const unsigned int& _format);
    void ConvertEquirectangularToCubemap(const unsigned int& _format);
    void ConvertCubemapToIrradiance(const unsigned int& _format);
    void PrefilterHDRMap(const unsigned int& _format);
};