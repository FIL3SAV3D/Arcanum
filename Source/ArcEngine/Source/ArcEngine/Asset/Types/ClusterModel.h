#pragma once

#include "ArcEngine/Asset/Interface/IAsset.h"

struct ClusterModel : public IAsset
{
    unsigned int meshletCount = 0;

    unsigned int VBO  = 0;
    unsigned int MBO  = 0;
    unsigned int MVBO = 0;
    unsigned int MTBO = 0;
};