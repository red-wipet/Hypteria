#include <windows.h>
#include <tchar.h>
//#include <ctime>
//#include <iostream>
//#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
//#include <time.h>
//#include "bootrec.h"
#include <cmath>
#include <time.h>
typedef struct
{
	float x;
	float y;
	float z;
} VERTEX;

typedef struct
{
	int vtx0;
	int vtx1;
} EDGE;


namespace _3D //credits to ArTicZera/JhoPro for this stuff, but I modified it to use cursor instead of icon
{
	VOID RotateX(VERTEX* vtx, float angle)
	{
		vtx->y = cos(angle) * vtx->y - sin(angle) * vtx->z;
		vtx->z = sin(angle) * vtx->y + cos(angle) * vtx->z;
	}

	VOID RotateY(VERTEX* vtx, float angle)
	{
		vtx->x = cos(angle) * vtx->x + sin(angle) * vtx->z;
		vtx->z = -sin(angle) * vtx->x + cos(angle) * vtx->z;
	}

	VOID RotateZ(VERTEX* vtx, float angle)
	{
		vtx->x = cos(angle) * vtx->x - sin(angle) * vtx->y;
		vtx->y = sin(angle) * vtx->x + cos(angle) * vtx->y;
	}

	void DrawEdge(HDC dc, LPCWSTR icon, int x0, int y0, int x1, int y1, int r)
	{
		int dx = abs(x1 - x0);
		int dy = -abs(y1 - y0);

		int sx = (x0 < x1) ? 1 : -1;
		int sy = (y0 < y1) ? 1 : -1;

		int error = dx + dy;

		int i = 0;

		while (true)
		{
			if (i == 0)
			{
				DrawIcon(dc, x0, y0, LoadCursor(NULL, icon));
				i = 10;
			}
			else
			{
				i--;
			}

			if (x0 == x1 && y0 == y1)
			{
				break;
			}

			int e2 = 2 * error;

			if (e2 >= dy)
			{
				if (x0 == x1)
				{
					break;
				}

				error += dy;
				x0 += sx;
			}

			if (e2 <= dx)
			{
				if (y0 == y1)
				{
					break;
				}

				error += dx;
				y0 += sy;
			}
		}
	}
}
#define M_PI   3.14159265358979323846264338327950288
//#define PI   3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //Credits to Void_/GetMBR
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}
/*COLORREF RndRGB() {
	int clr = rand() % 5;
	if (clr == 0) return RGB(255, 0, 0); if (clr == 1) return RGB(0, 255, 0); if (clr == 2) return RGB(0, 0, 255); if (clr == 3) return RGB(255, 0, 255); if (clr == 4) return RGB(255, 255, 0);
}*/

DWORD WINAPI payload1(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;


	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;
				int Xii = x * x + i;
				int Yii = y * y + i;
				int fx = (int)((i * 4) + (i * 4) * cbrt(Xii + i - Yii + i));

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI payload2(LPVOID lpParam) {
	HDC hdc;
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1), xSize = sw / 4, ySize = 8;
	while (1) {
		hdc = GetDC(0); HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, screenshot);
		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < sw + 90; i++) {
			int wave = cbrt(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCCOPY);
			BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);
		Sleep(1);
		ReleaseDC(0, hdc);
		DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
	}
}
DWORD WINAPI payload3(LPVOID lpParam) {
	HDC hdc;
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1), xSize = sw / 90, ySize = 90;
	while (1) {
		hdc = GetDC(0); HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, screenshot);
		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < sw + 10; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCCOPY);
			BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);
		Sleep(1);
		ReleaseDC(0, hdc);
		DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
	}
}
DWORD WINAPI payload4(LPVOID lpParam) {
	HDC hdc;
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1), xSize = sw / 9, ySize = 90;
	while (1) {
		hdc = GetDC(0); HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, screenshot);
		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < sw + 10; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCCOPY);
			BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);
		Sleep(1);
		ReleaseDC(0, hdc);
		DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
	}
}
DWORD WINAPI payload5(LPVOID lpParam) { //that shader is by me and pankoza
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			int average = round((float)(rgbScreen[i].b + rgbScreen[i].r + rgbScreen[i].g) / 1);
			rgbScreen[i].r = average;
			rgbScreen[i].g = average;
			rgbScreen[i].b = average;
			rgbScreen[i].rgb -= 999;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}
DWORD WINAPI payload6(LPVOID lpParam) { //by soheil shahrab and me
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	int pertcangfiy = 0;
	for (;;) {
		hdcScreen = GetDC(0);
		pertcangfiy = 5 + pertcangfiy;
		StretchBlt(hdcMem, pertcangfiy, 0, w, h, hdcScreen, 0, 0, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			int randPixel = rand() % w;
			int tempR = GetRValue(rgbScreen[i].rgb), tempG = GetGValue(rgbScreen[i].rgb), tempB = GetBValue(rgbScreen[i].rgb);
			int color = RGB(tempB, tempG, tempR);
			INT x = i % w, y = i / w;
			int Xii = x & x;
			int Yii = y & y;
			rgbScreen[i].r = color - color;
			rgbScreen[i].g = Xii - Yii;
			rgbScreen[i].b = x + y;
			rgbScreen[randPixel].rgb = RGB(tempR, tempG, tempB);
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, pertcangfiy, 0, SRCCOPY);
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, pertcangfiy - w + 160, 0, SRCCOPY);
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, pertcangfiy - w - w + 160, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}
DWORD WINAPI payload7(LPVOID lpParam) {
	for (int t = 0; ; t++)
	{
		HDC hdc = GetDC(NULL);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC hcdc = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(hcdc, hBitmap);
		BLENDFUNCTION blf = { 0 };
		blf.BlendOp = AC_SRC_OVER;
		blf.BlendFlags = 0;
		blf.SourceConstantAlpha = 200;
		blf.AlphaFormat = 0;
		POINT pos[3];
		pos[0].x = 0;
		pos[0].y = 0;
		pos[1].x = cos(M_PI / 36) * w;
		pos[1].y = sin(M_PI / 36) / w;
		pos[2].x = (-1) * (sin(M_PI / 36) * h);
		pos[2].y = cos(M_PI / 36) * h;
		PlgBlt(hcdc, pos, hdc, 0, 0, w, h, 0, 0, 0);
		AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
		ReleaseDC(NULL, hdc);
		ReleaseDC(NULL, hcdc);
		DeleteObject(hdc);
		DeleteObject(hcdc);
	}
	return 0;
}
DWORD WINAPI payload8(LPVOID lpParam) {
	for (int t = 0; ; t++)
	{
		HDC hdc = GetDC(NULL);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC hcdc = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(hcdc, hBitmap);
		BLENDFUNCTION blf = { 0 };
		blf.BlendOp = AC_SRC_OVER;
		blf.BlendFlags = 0;
		blf.SourceConstantAlpha = 200;
		blf.AlphaFormat = 0;
		POINT pos[3];
		pos[0].x = 0;
		pos[0].y = 0;
		pos[1].x = sin(M_PI / 36) * w;
		pos[1].y = sin(M_PI / 36) * h;
		pos[2].x = (-1) * (sin(M_PI / 36) * h);
		pos[2].y = cos(M_PI / 36) * h;
		PlgBlt(hcdc, pos, hdc, 0, 0, w, h, 0, 0, 0);
		AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
		ReleaseDC(NULL, hdc);
		ReleaseDC(NULL, hcdc);
		DeleteObject(hdc);
		DeleteObject(hcdc);
	}
	return 0;

}
DWORD WINAPI payload9(LPVOID lpParam) {
	for (int t = 0; ; t++)
	{
		HDC hdc = GetDC(NULL);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		HDC hcdc = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
		SelectObject(hcdc, hBitmap);
		BLENDFUNCTION blf = { 0 };
		blf.BlendOp = AC_SRC_OVER;
		blf.BlendFlags = 0;
		blf.SourceConstantAlpha = 200;
		blf.AlphaFormat = 0;
		POINT pos[3];
		pos[0].x = 0;
		pos[0].y = 0;
		pos[1].x = cos(M_PI / 36) * w;
		pos[1].y = sin(M_PI / 36) * h;
		pos[2].x = (-1) * (cos(M_PI / 36) * h);
		pos[2].y = cos(M_PI / 36) * h;
		PlgBlt(hcdc, pos, hdc, 0, 0, w, h, 0, 0, 0);
		AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
		ReleaseDC(NULL, hdc);
		ReleaseDC(NULL, hcdc);
		DeleteObject(hdc);
		DeleteObject(hcdc);
	}
	return 0;

}
DWORD WINAPI payload10(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	for (int t = 0; ; t++)
	{
		hdc = GetDC(NULL);
		POINT pos[999];
		for (int i = 0; i < 4; i++)
		{
			pos[i].x = rand() % w, pos[i].y = rand() % h;
		}
		HPEN pen15 = CreatePen(PS_SOLID, 99, 0);
		SelectObject(hdc, pen15);
		HBRUSH brush = CreateSolidBrush(Hue(239));
		SelectObject(hdc, brush);
		Polygon(hdc, pos, 4);
		SetBkColor(hdc, Hue(239));
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, 0);
		ReleaseDC(NULL, hdc);
		DeleteObject(brush);
		DeleteObject(pen15);
		DeleteObject(hdc);
		Sleep(10);
	}
	return 0;
}
DWORD WINAPI sine(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	HDC hcdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(hcdc, hBitmap);
	BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
	for (int t = 0; ; t -= 90)
	{
		hdc = GetDC(NULL);
		for (int y = 0; y <= h; y++)
		{
			float x = sin((y - t) * (M_PI / 1000)) * 25;
			BitBlt(hdc, x, y, w, 1, hcdc, 0, y, SRCCOPY);
		}
		ReleaseDC(NULL, hdc);
		DeleteObject(hdc);
	}
	ReleaseDC(NULL, hcdc);
	DeleteObject(hcdc);
	DeleteObject(hBitmap);
	return 0;
}
VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(((t ^ t >> 8) * (t | t >> 5) & (t | t << 4) + (t | t >> 7)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t ^ t >> 5) + (t >> t >> 9));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> t >> 5) * (t ^ t >> 7));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 5) * tan(t ^ 4));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t ^ t % 1001 + t ^ t % 1002 | (t << 2 ^ t >> 4 ^ t << 4 & t >> 8 | t << 1 & t >> 4));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t << 2 ^ t >> 4 ^ t << 4 & t >> 8 | t << 1 & t >> 4));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> t) | 3 * t ^ t >> 6 | t);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound8() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(3 >> 6 >> t ^ t >> 6 | t);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	if (MessageBoxW(NULL, L"WARNING!!!\r\n\Run GDI only?", L"Hypteria", MB_YESNO | MB_ICONWARNING) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"FINAL WARNING!!!\r\n\NOT FOR EPILEPSY", L"LAST WARNING - Hypteria", MB_YESNO | MB_ICONWARNING) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			sound1();
			HANDLE thread9 = CreateThread(0, 0, payload1, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			InvalidateRect(0, 0, 0);
			sound2();
			HANDLE thread10 = CreateThread(0, 0, payload2, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
			InvalidateRect(0, 0, 0);
			sound3();
			HANDLE thread11 = CreateThread(0, 0, payload3, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread11, 0);
			CloseHandle(thread11);
			InvalidateRect(0, 0, 0);
			sound4();
			HANDLE thread13 = CreateThread(0, 0, sine, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread13, 0);
			CloseHandle(thread13);
			InvalidateRect(0, 0, 0);
			sound5();
			HANDLE thread14 = CreateThread(0, 0, payload4, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread14, 0);
			CloseHandle(thread14);
			InvalidateRect(0, 0, 0);
			HANDLE thread15 = CreateThread(0, 0, payload5, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread15, 0);
			CloseHandle(thread15);
			InvalidateRect(0, 0, 0);
			sound7();
			HANDLE thread16 = CreateThread(0, 0, payload6, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread16, 0);
			CloseHandle(thread16);
			InvalidateRect(0, 0, 0);
			sound8();
			HANDLE thread17 = CreateThread(0, 0, payload7, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread17, 0);
			CloseHandle(thread17);
			InvalidateRect(0, 0, 0);
			sound8();
			HANDLE thread18 = CreateThread(0, 0, payload8, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread18, 0);
			CloseHandle(thread18);
			InvalidateRect(0, 0, 0);
			sound8();
			HANDLE thread19 = CreateThread(0, 0, payload9, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread19, 0);
			CloseHandle(thread19);
			InvalidateRect(0, 0, 0);
			sound8();
			HANDLE thread20 = CreateThread(0, 0, payload10, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread20, 0);
			CloseHandle(thread20);
			InvalidateRect(0, 0, 0);
		}
	}
}
