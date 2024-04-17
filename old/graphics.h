#pragma once

#include "main.h"

#include <stdint.h>

#define RGBtouint32_t(r, g, b) ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF) << 0);

using std::vector;

extern std::chrono::high_resolution_clock::time_point last;
extern std::chrono::high_resolution_clock::time_point ab;

float deltaTime();

uint32_t GetColour(float lum, uint32_t baseColor);
//void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t col);
void fillTriangle(HDC hdc, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t col);
void drawLine(HDC hdc, int x0, int y0, int x1, int y1, uint32_t col);
