#pragma once
#include <windows.h>
#include <vector>
#include "Point.h"
#include <vector>

extern std::vector<class Line> lines;
extern std::vector<class Circle> circles;
extern std::vector<class Rect> rects;
extern std::vector<class Triangle> triangles;
extern std::vector<class Parallelogram> parallelograms;
extern std::vector<class Rhombus> rhombuses;
extern std::vector<class Bezier> beziers;

struct Circle;

bool isInsideCircle(const POINT& p, const Circle& c);
bool isInsidePolygon(const POINT& p, const std::vector<POINT>& polygon);

const COLORREF FILL_COLOR = RGB(211, 211, 211); 
extern RECT scanline_button_rect;
extern COLORREF scanline_button_color;
extern RECT seed_button_rect;
extern COLORREF seed_button_color;

struct FilledShape {
    int type;
    int index;
    bool use_scanline;
    POINT seed_point; 
};


extern std::vector<FilledShape> filled_shapes;

void scanlineFill(HDC hdc, const std::vector<POINT>& vertices, COLORREF fillColor);
void seedFill(HDC hdc, int x, int y, COLORREF fillColor, COLORREF boundaryColor);
void fillShape(HDC hdc, int shape_type, int shape_index, bool use_scanline, const POINT& click_pos);
void redrawFill(HDC hdc, const FilledShape& shape);