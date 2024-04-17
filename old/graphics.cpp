#include "graphics.h"

std::chrono::high_resolution_clock::time_point last;
std::chrono::high_resolution_clock::time_point ab;

float deltaTime() 
{
	return (float)std::chrono::duration_cast<std::chrono::nanoseconds>(ab - last).count() / 1000000000.0f;
}

void drawLine(HDC hdc, int x0, int y0, int x1, int y1, uint32_t col) 
{
	if (x0 < 0 || x0 > SCRW) return;
	if (y0 < 0 || y0 > SCRH) return;
	if (x1 < 0 || x1 > SCRW) return;
	if (y1 < 0 || y1 > SCRH) return;
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true) {
		//scrbuf[x0 + y0 * SCRW] = col;
		SetPixel(hdc, x0, y0, col);
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

/*void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t col)
{
	drawLine(x0, y0, x1, y1, col);
	drawLine(x0, y0, x2, y2, col);
	drawLine(x1, y1, x2, y2, col);
}*/

void fillTriangle(HDC hdc, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t col)
{
	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }
	if (y0 > y2) { std::swap(y0, y2); std::swap(x0, x2); }
	if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }

	t1x = t2x = x0; y = y0;   // Starting points

	dx1 = (int)(x1 - x0); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y1 - y0);

	dx2 = (int)(x2 - x0); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y2 - y0);

	if (dy1 > dx1) {   // swap values
		std::swap(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		std::swap(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y0 == y1) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawLine(hdc, minx, y, maxx, y, col);    // Draw line from min to max points found on the y
		// Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y1) break;

	}
next:
	// Second half
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);
	t1x = x1;

	if (dy1 > dx1) {   // swap values
		std::swap(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x2) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawLine(hdc, minx, y, maxx, y, col);    // Draw line from min to max points found on the y
		// Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y2) return;
	}
}

uint32_t GetColour(float lum, uint32_t baseColor)
{
	int red = (int)((float)((baseColor >> 16) & 0xFF) * lum) & 0xFF;
	int green = (int)((float)((baseColor >> 8) & 0xFF) * lum) & 0xFF;
	int blue = (int)((float)((baseColor) & 0xFF) * lum) & 0xFF;
	uint32_t c = (red << 16) | (green << 8) | (blue);
	return c;
}
