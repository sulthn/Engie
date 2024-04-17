#pragma once

#include <windows.h>
#include <d2d1.h>

#include <stdint.h>
#include <chrono>
#include <algorithm>
#include <list>

using std::list;

#include "matrix.h"

extern std::chrono::high_resolution_clock::time_point last;
extern std::chrono::high_resolution_clock::time_point ab;

extern ID2D1Factory* pD2DFactory;

extern double deltaTime;

extern int SCRW;
extern int SCRH;

extern mat4x4 matProj;
extern vec3d vCamera;
extern vec3d vLookDir;

extern float fYaw;

uint32_t GetColour(float lum, uint32_t baseColor);

ID2D1PathGeometry* GenTriangleGeometry(D2D1_POINT_2F pt1, D2D1_POINT_2F pt2, D2D1_POINT_2F pt3);

D2D1_POINT_2F D2PF(int x, int y);


void initialize();
void render(ID2D1HwndRenderTarget* pRT, uint32_t d);
void resize(float w, float h);
