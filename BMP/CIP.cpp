#include "pch.h"
#include "CIP.h"

DOUBLE*** RGB2YUV(CDib dib)
{
	// Y =   0.299R + 0.587G + 0.114B
	// U = - 0.147R - 0.289G + 0.437B
	// V =   0.615R - 0.515G - 0.100B

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	RGBBYTE** ptr = dib.GetRGBPtr();
	DOUBLE*** YUVPtr = new DOUBLE **[3];
	
	YUVPtr[0] = new DOUBLE * [h];
	YUVPtr[1] = new DOUBLE * [h];
	YUVPtr[2] = new DOUBLE * [h];

	DOUBLE R = 0., G = 0., B = 0.;
	DOUBLE Y = 0., U = 0., V = 0.;
	DOUBLE value = 0;
	for (register int j = 0; j < h; j++)
	{
		YUVPtr[0][j] = new DOUBLE[w];
		YUVPtr[1][j] = new DOUBLE[w];
		YUVPtr[2][j] = new DOUBLE[w];

		for (register int i = 0; i < w; i++)
		{
			R = ptr[j][i].r / 255.;
			G = ptr[j][i].g / 255.;
			B = ptr[j][i].b / 255.;

			Y =   0.299 * R + 0.587 * G + 0.114 * B;
			U = - 0.147 * R - 0.289 * G + 0.437 * B;
			V =   0.615 * R - 0.515 * G - 0.100 * B;

			YUVPtr[0][j][i] = Y;
			YUVPtr[1][j][i] = U;
			YUVPtr[2][j][i] = V;
		}
	}
	return YUVPtr;
}

void YUV2RGB(CDib *dib, DOUBLE*** YUV)
{
	// R = 1.000Y + 0.000U + 1.403V
	// G = 1.000Y - 0.344U - 0.714V
	// B = 1.000Y + 1.773U + 0.000V

	int w = dib->GetWidth();
	int h = dib->GetHeight();

	RGBBYTE** ptr = dib->GetRGBPtr();

	DOUBLE Y = 0., U = 0., V = 0.;
	DOUBLE R = 0., G = 0., B = 0.;
	DOUBLE value = 0;
	for (register int j = 0; j < h; j++)
	{
		for (register int i = 0; i < w; i++)
		{
			Y = YUV[0][j][i];
			U = YUV[1][j][i];
			V = YUV[2][j][i];

			R = 1.000 * Y + 0.000 * U + 1.403 * V;
			G = 1.000 * Y - 0.344 * U - 0.714 * V;
			B = 1.000 * Y + 1.773 * U + 0.000 * V;

			ptr[j][i].r = (BYTE)limit(R * 255.);
			ptr[j][i].g = (BYTE)limit(G * 255.);
			ptr[j][i].b = (BYTE)limit(B * 255.);
		}
	}
}

DOUBLE** Alloc2DMem(int h, int w)
{
	DOUBLE** temp;
	int i, j;
	temp = new DOUBLE * [h];
	for (i = 0; i < h; i++)
	{
		temp[i] = new DOUBLE[w];
	}
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			temp[i][j] = 0.0;
		}
	}
	return temp;
}
