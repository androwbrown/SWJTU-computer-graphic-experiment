#pragma once
#include <windows.h>

// 裁剪
const RECT clip_button_rect = {710, 500, 780, 530};
const COLORREF clip_button_color = RGB(120, 220, 120); // A green-ish color

// S-H裁剪
const RECT sh_clip_button_rect = {640, 500, 710, 530};
const COLORREF sh_clip_button_color = RGB(100, 180, 220); // A blue-ish color

// W-A裁剪
const RECT wa_clip_button_rect = {780, 500, 850, 530};
const COLORREF wa_clip_button_color = RGB(220, 180, 100); // An orange-ish color

// 旋转
const RECT rotate_button_rect = {500, 500, 570, 530};
const COLORREF rotate_button_color = RGB(220, 100, 100); // A red-ish color

// 手动缩放
const RECT scale_button_rect = {570, 500, 640, 530};
const COLORREF scale_button_color = RGB(200, 100, 200); // A purple-ish color
