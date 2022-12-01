#include "pch.h"
#include "RGBBYTE.h"

RGBBYTE& RGBBYTE::operator = (const RGBBYTE& pixel) {
	if (this == &pixel)
		return *this;
	this->r = pixel.r;
	this->g = pixel.g;
	this->b = pixel.b;
	return *this;
}

RGBBYTE& RGBBYTE::operator = (const COLORREF& pixel) {
	r = (BYTE)(pixel);
	g = (BYTE)((WORD)(pixel) >> 8);
	b = (BYTE)((pixel) >> 16);
	return *this;
}

RGBBYTE& RGBBYTE::operator = (const BYTE& pixel) {
	r = pixel;
	g = pixel;
	b = pixel;
	return *this;
}

int RGBBYTE::operator==(const RGBBYTE& pixel) {
	if (r == pixel.r && g == pixel.g && b == pixel.b)
		return 1;
	else return 0;
}

int RGBBYTE::operator!=(const RGBBYTE& pixel) {
	if (r != pixel.r || g != pixel.g || b != pixel.b)
		return 1;
	else return 0;
}