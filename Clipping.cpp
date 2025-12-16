#include "Clipping.h"
#include <algorithm>

using namespace std;
//输入：裁剪矩形clipRect和线段l
//输出：裁剪后线段l
bool liangBarskyClip(Line& l, const RECT& clipRect) {
    double x1 = l.s.x, y1 = l.s.y;
    double x2 = l.e.x, y2 = l.e.y;
    double dx = x2 - x1, dy = y2 - y1;

    double p[4], q[4];
    double t0 = 0.0, t1 = 1.0;

    //定义p为-dx,dx,-dy,dy
    p[0] = -dx;
    q[0] = x1 - clipRect.left;
    p[1] = dx;
    q[1] = clipRect.right - x1;
    p[2] = -dy;
    q[2] = y1 - clipRect.top;
    p[3] = dy;
    q[3] = clipRect.bottom - y1;

    for (int i = 0; i < 4; ++i) {
        if (p[i] == 0.0) {
           //完全在矩形框外
            if (q[i] < 0) {
                return false; // 直接舍弃
            }
        } else {
            double r = q[i] / p[i];
            if (p[i] < 0) {
                t0 = max(t0, r);
            } else {
                t1 = min(t1, r);
            }
        }
    }

    if (t0 > t1) {
        return false; // 完全在矩形框外
    }

    double newX1 = x1 + t0 * dx;
    double newY1 = y1 + t0 * dy;
    double newX2 = x1 + t1 * dx;
    double newY2 = y1 + t1 * dy;

    l.s.x = (LONG)round(newX1);
    l.s.y = (LONG)round(newY1);
    l.e.x = (LONG)round(newX2);
    l.e.y = (LONG)round(newY2);

    l.left = min(l.s.x, l.e.x);
    l.right = max(l.s.x, l.e.x);
    l.button = min(l.s.y, l.e.y);
    l.top = max(l.s.y, l.e.y);

    return true;
}
