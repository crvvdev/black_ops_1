#pragma once

#include <Windows.h>
#include <intrin.h>
#include <functional>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <memory>
#include <mutex>
#include <regex>

#include <DirectXMath.h>

// static auto D3DCOLOR_TO_QUAKE(int r, int g, int b, int a)
//{
//	return D3DXvec4_t{
//			static_cast<float>(r) / 255.f,
//			static_cast<float>(g) / 255.f,
//			static_cast<float>(b) / 255.f,
//			static_cast<float>(a) / 255.f
//	};
// };

#include "tools.hpp"

// 2d drawing
#include "drawing.hpp"
using namespace Drawing;

// game engine
#include "pointers.hpp"
#include "classes.hpp"
#include "math.hpp"
#include "engine.hpp"

// gui
#include "options.hpp"
#include "gui.hpp"
#include "controls.hpp"
#include "menu.hpp"

// cheats & others
#include "hooks.hpp"
#include "globals.hpp"
#include "misc.hpp"
#include "esp.hpp"
#include "aimbot.hpp"
