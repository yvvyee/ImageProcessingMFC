#pragma once

template<typename T>
inline void swap(T& lha, T& rha)
{
	T tmp = lha;
	lha = rha;
	rha = tmp;
}

template<typename T>
inline T limit(const T& value)
{
	return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
}

template<typename T>
inline T limit(const T& value, const T& lower, const T& upper)
{
	T tmp = value;
	if (value > upper)
	{
		tmp = upper;
	}
	else
	{
		if (value < lower)
		{
			tmp = lower;
		}
	}
	return tmp;
}

class RGBBYTE
{
public:
	BYTE b;
	BYTE g;
	BYTE r;

	RGBBYTE& operator = (const RGBBYTE& pixel);
	RGBBYTE& operator = (const COLORREF& pixel);
	RGBBYTE& operator = (const BYTE& pixel);
	int operator==(const RGBBYTE& pixel);
	int operator!=(const RGBBYTE& pixel);
};