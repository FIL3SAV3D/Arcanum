#pragma once

#include "ArcEngine/Util/Platform/PlatformDetection.h"

#if defined(ARC_PLATFORM_WINDOWS)
#include "ArcEngine/Platform/Windows/Window.h"
#elif defined(ARC_PLATFORM_ANDROID)
#error "Not Implemented Yet!"
#elif defined(ARC_PLATFORM_LINUX)
#error "Not Implemented Yet!"
#endif 