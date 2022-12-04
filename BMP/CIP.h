#pragma once
#include "CDib.h"

DOUBLE*** RGB2YUV(CDib dib);
void YUV2RGB(CDib *dib, DOUBLE*** YUV);
DOUBLE** Alloc2DMem(int h, int w);