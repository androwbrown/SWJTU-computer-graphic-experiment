#pragma once
#include "Line.h"
#include <windows.h>

// Liang-Barsky 法裁剪
bool liangBarskyClip(Line& l, const RECT& clipRect);
