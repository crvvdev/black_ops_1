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

#include "misc\tools.hpp"

// 2d drawing
#include "draw\drawing.hpp"
using namespace Drawing;

// game engine
#include "sdk\pointers.hpp"
#include "sdk\classes.hpp"
#include "sdk\math.hpp"
#include "sdk\engine.hpp"

// gui
#include "misc\options.hpp"
#include "menu\gui.hpp"
#include "menu\controls.hpp"
#include "menu\menu.hpp"

// cheats & others
#include "hooks.hpp"
#include "misc\globals.hpp"
#include "cheats\misc.hpp"
#include "cheats\esp.hpp"
#include "cheats\aimbot.hpp"
