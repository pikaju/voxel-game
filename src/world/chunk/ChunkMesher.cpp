#include "ChunkMesher.h"

int marchingCubesFaces[256][16] =
{ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
{ 8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1 },
{ 3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1 },
{ 4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
{ 4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1 },
{ 9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1 },
{ 10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1 },
{ 5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
{ 5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1 },
{ 8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1 },
{ 2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
{ 2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1 },
{ 11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1 },
{ 5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1 },
{ 11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1 },
{ 11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1 },
{ 2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1 },
{ 6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
{ 3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1 },
{ 6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
{ 6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1 },
{ 8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1 },
{ 7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1 },
{ 3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
{ 0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1 },
{ 9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1 },
{ 8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
{ 5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1 },
{ 0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1 },
{ 6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1 },
{ 10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
{ 1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1 },
{ 0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1 },
{ 3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
{ 6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1 },
{ 9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1 },
{ 8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1 },
{ 3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1 },
{ 10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1 },
{ 10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
{ 2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1 },
{ 7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
{ 2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1 },
{ 1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1 },
{ 11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1 },
{ 8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1 },
{ 0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1 },
{ 7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1 },
{ 7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1 },
{ 10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1 },
{ 0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1 },
{ 7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1 },
{ 6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1 },
{ 4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1 },
{ 10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1 },
{ 8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1 },
{ 1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1 },
{ 10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1 },
{ 10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1 },
{ 9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1 },
{ 7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1 },
{ 3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1 },
{ 7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1 },
{ 3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1 },
{ 6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1 },
{ 9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1 },
{ 1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1 },
{ 4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1 },
{ 7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1 },
{ 6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1 },
{ 0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1 },
{ 6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1 },
{ 0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1 },
{ 11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1 },
{ 6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1 },
{ 5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1 },
{ 9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1 },
{ 1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1 },
{ 10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1 },
{ 0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1 },
{ 11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1 },
{ 9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1 },
{ 7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1 },
{ 2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1 },
{ 9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1 },
{ 9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1 },
{ 1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1 },
{ 0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1 },
{ 10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1 },
{ 2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1 },
{ 0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1 },
{ 0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1 },
{ 9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1 },
{ 5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1 },
{ 5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1 },
{ 8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1 },
{ 9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1 },
{ 1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1 },
{ 3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1 },
{ 4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1 },
{ 9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1 },
{ 11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1 },
{ 2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1 },
{ 9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1 },
{ 3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1 },
{ 1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1 },
{ 4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1 },
{ 0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1 },
{ 1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 } };

void VoxelQuad::merge(VoxelQuad& quad)
{
	if (quad.m_x > m_x) {
		m_width += quad.m_width;
		m_p2 = quad.m_p2;
		m_p3 = quad.m_p3;
	}
	if (quad.m_y > m_y) {
		m_height += quad.m_height;
		m_p1 = quad.m_p1;
		m_p2 = quad.m_p2;
	}

	quad.m_width = 0;
	quad.m_height = 0;
}

bool VoxelQuad::canMerge(const VoxelQuad& quad)
{
	if (m_flipped != quad.m_flipped) {
		return false;
	}
	if (quad.m_x != m_x && quad.m_y != m_y) {
		return false;
	}
	
	if (quad.m_x == m_x) {
		if (quad.m_width != m_width) return false;

		if (m_c0 != m_c1) return false;
		if (quad.m_c0 != quad.m_c1) return false;
		if (m_c0 != quad.m_c0) return false;

		if (m_c2 != m_c3) return false;
		if (quad.m_c2 != quad.m_c3) return false;
		if (m_c2 != quad.m_c2) return false;
	}
	if (quad.m_y == m_y) {
		if (quad.m_height != m_height) return false;

		if (m_c0 != m_c3) return false;
		if (quad.m_c0 != quad.m_c3) return false;
		if (m_c0 != quad.m_c0) return false;

		if (m_c1 != m_c2) return false;
		if (quad.m_c1 != quad.m_c2) return false;
		if (m_c1 != quad.m_c1) return false;
	}

	return true;
}

void VoxelQuad::add(vt::IndexedMesh<VoxelVertex, unsigned int>& mesh)
{
	if (m_width <= 0 || m_height <= 0) {
		return;
	}
	vt::Vec3f d0 = (m_p1 - m_p0).normalized() * POLYGON_SCALE_INCREASE;
	vt::Vec3f d1 = (m_p3 - m_p0).normalized() * POLYGON_SCALE_INCREASE;

	VoxelVertex v0(m_p0 - d0 - d1, m_c0, m_normal);
	VoxelVertex v1(m_p1 + d0 - d1, m_c1, m_normal);
	VoxelVertex v2(m_p2 + d0 + d1, m_c2, m_normal);
	VoxelVertex v3(m_p3 - d0 + d1, m_c3, m_normal);
	if (m_flipped) {
		mesh.quad(v3, v0, v1, v2);
	}
	else {
		mesh.quad(v0, v1, v2, v3);
	}
}

void ChunkMesher::createMesh(Chunk& chunk, vt::IndexedMesh<VoxelVertex, unsigned int>& mesh, vt::IndexedMesh<VoxelVertex, unsigned int>& waterMesh, vt::IndexedMesh<VoxelVertex, unsigned int>& grassMesh)
{
	const vt::Vec3f v0(0.0f, 0.0f, 0.0f);
	const vt::Vec3f v1(0.0f, 0.0f, 1.0f);
	const vt::Vec3f v2(1.0f, 0.0f, 1.0f);
	const vt::Vec3f v3(1.0f, 0.0f, 0.0f);
	const vt::Vec3f v4(0.0f, 1.0f, 0.0f);
	const vt::Vec3f v5(0.0f, 1.0f, 1.0f);
	const vt::Vec3f v6(1.0f, 1.0f, 1.0f);
	const vt::Vec3f v7(1.0f, 1.0f, 0.0f);

	const vt::Vec3f normalTop(0.0f, 1.0f, 0.0f);
	const vt::Vec3f normalBottom(0.0f, -1.0f, 0.0f);
	const vt::Vec3f normalLeft(-1.0f, 0.0f, 0.0f);
	const vt::Vec3f normalRight(1.0f, 0.0f, 0.0f);
	const vt::Vec3f normalFront(0.0f, 0.0f, 1.0f);
	const vt::Vec3f normalBack(0.0f, 0.0f, -1.0f);

	VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>* container = new VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>();

	chunk.getVoxelContainer().decompress(*container);

	for (int x = -AMBIENT_OCCLUSION_SAMPLE_AREA; x < CHUNK_SIZE + AMBIENT_OCCLUSION_SAMPLE_AREA; x++) {
		for (int y = -AMBIENT_OCCLUSION_SAMPLE_AREA; y < CHUNK_SIZE + AMBIENT_OCCLUSION_SAMPLE_AREA; y++) {
			for (int z = -AMBIENT_OCCLUSION_SAMPLE_AREA; z < CHUNK_SIZE + AMBIENT_OCCLUSION_SAMPLE_AREA; z++) {
				if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
					container->setData(x, y, z, chunk.getVoxelDataRelative(x, y, z));
				}
			}
		}
	}

	VoxelQuad quads[CHUNK_SIZE * CHUNK_SIZE];
	
	// Top
	for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
		memset(quads, 0, sizeof(quads));
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				int xx = x;
				int yy = z;
				int zz = y;
				
				Voxel* voxel = VoxelType::getFromID(container->getData(xx, yy, zz).getID());
				if (voxel->isOpaque()) {
					vt::Vec3f o((float)xx, (float)yy, (float)zz);

					if (!VoxelType::getFromID(container->getData(xx, yy + 1, zz).getID())->isOpaque()) {
						quads[x + y * CHUNK_SIZE] = getQuad(chunk, *container, x, y, v4 + o, v5 + o, v6 + o, v7 + o, ((SolidVoxel*)voxel)->getColor(), normalTop);
					}
				}
			}
		}
#if GREEDY_MESHING
		optimizeSlice(quads);
#endif
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				quads[x + y * CHUNK_SIZE].add(mesh);
			}
		}
	}

	// Bottom
	for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
		memset(quads, 0, sizeof(quads));
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				int xx = x;
				int yy = z;
				int zz = CHUNK_SIZE - y - 1;

				Voxel* voxel = VoxelType::getFromID(container->getData(xx, yy, zz).getID());
				if (voxel->isOpaque()) {
					vt::Vec3f o((float)xx, (float)yy, (float)zz);

					if (!VoxelType::getFromID(container->getData(xx, yy - 1, zz).getID())->isOpaque()) {
						quads[x + y * CHUNK_SIZE] = getQuad(chunk, *container, x, y, v1 + o, v0 + o, v3 + o, v2 + o, ((SolidVoxel*)voxel)->getColor(), normalBottom);
					}
				}
			}
		}
#if GREEDY_MESHING
		optimizeSlice(quads);
#endif
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				quads[x + y * CHUNK_SIZE].add(mesh);
			}
		}
	}

	// Left
	for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
		memset(quads, 0, sizeof(quads));
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				int xx = z;
				int yy = CHUNK_SIZE - y - 1;
				int zz = x;

				Voxel* voxel = VoxelType::getFromID(container->getData(xx, yy, zz).getID());
				if (voxel->isOpaque()) {
					vt::Vec3f o((float)xx, (float)yy, (float)zz);

					if (!VoxelType::getFromID(container->getData(xx - 1, yy, zz).getID())->isOpaque()) {
						quads[x + y * CHUNK_SIZE] = getQuad(chunk, *container, x, y, v4 + o, v0 + o, v1 + o, v5 + o, ((SolidVoxel*)voxel)->getColor(), normalLeft);
					}
				}
			}
		}
#if GREEDY_MESHING
		optimizeSlice(quads);
#endif
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				quads[x + y * CHUNK_SIZE].add(mesh);
			}
		}
	}

	// Right
	for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
		memset(quads, 0, sizeof(quads));
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				int xx = z;
				int yy = CHUNK_SIZE - y - 1;
				int zz = CHUNK_SIZE - x - 1;

				Voxel* voxel = VoxelType::getFromID(container->getData(xx, yy, zz).getID());
				if (voxel->isOpaque()) {
					vt::Vec3f o((float)xx, (float)yy, (float)zz);

					if (!VoxelType::getFromID(container->getData(xx + 1, yy, zz).getID())->isOpaque()) {
						quads[x + y * CHUNK_SIZE] = getQuad(chunk, *container, x, y, v6 + o, v2 + o, v3 + o, v7 + o, ((SolidVoxel*)voxel)->getColor(), normalRight);
					}
				}
			}
		}
#if GREEDY_MESHING
		optimizeSlice(quads);
#endif
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				quads[x + y * CHUNK_SIZE].add(mesh);
			}
		}
	}

	// Front
	for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
		memset(quads, 0, sizeof(quads));
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				int xx = x;
				int yy = CHUNK_SIZE - y - 1;
				int zz = z;

				Voxel* voxel = VoxelType::getFromID(container->getData(xx, yy, zz).getID());
				if (voxel->isOpaque()) {
					vt::Vec3f o((float)xx, (float)yy, (float)zz);

					if (!VoxelType::getFromID(container->getData(xx, yy, zz + 1).getID())->isOpaque()) {
						quads[x + y * CHUNK_SIZE] = getQuad(chunk, *container, x, y, v5 + o, v1 + o, v2 + o, v6 + o, ((SolidVoxel*)voxel)->getColor(), normalFront);
					}
				}
			}
		}
#if GREEDY_MESHING
		optimizeSlice(quads);
#endif
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				quads[x + y * CHUNK_SIZE].add(mesh);
			}
		}
	}

	// Back
	for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
		memset(quads, 0, sizeof(quads));
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				int xx = CHUNK_SIZE - x - 1;
				int yy = CHUNK_SIZE - y - 1;
				int zz = z;

				Voxel* voxel = VoxelType::getFromID(container->getData(xx, yy, zz).getID());
				if (voxel->isOpaque()) {
					vt::Vec3f o((float)xx, (float)yy, (float)zz);

					if (!VoxelType::getFromID(container->getData(xx, yy, zz - 1).getID())->isOpaque()) {
						quads[x + y * CHUNK_SIZE] = getQuad(chunk, *container, x, y, v7 + o, v3 + o, v0 + o, v4 + o, ((SolidVoxel*)voxel)->getColor(), normalBack);
					}
				}
			}
		}
#if GREEDY_MESHING
		optimizeSlice(quads);
#endif
		for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
			for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
				quads[x + y * CHUNK_SIZE].add(mesh);
			}
		}
	}

	createWaterMesh(chunk, waterMesh, container);
	createGrassMesh(chunk, grassMesh, container);

	delete container;
}

void ChunkMesher::createMeshMarchingCubes(Chunk & chunk, vt::IndexedMesh<VoxelVertex, unsigned int>& mesh, vt::IndexedMesh<VoxelVertex, unsigned int>& waterMesh)
{
	vt::Vec3f edges[] = {
		{ 0.5f + 0.5f, 0.0f + 0.5f, 0.0f + 0.5f },
		{ 1.0f + 0.5f, 0.0f + 0.5f, 0.5f + 0.5f },
		{ 0.5f + 0.5f, 0.0f + 0.5f, 1.0f + 0.5f },
		{ 0.0f + 0.5f, 0.0f + 0.5f, 0.5f + 0.5f },
		{ 0.5f + 0.5f, 1.0f + 0.5f, 0.0f + 0.5f },
		{ 1.0f + 0.5f, 1.0f + 0.5f, 0.5f + 0.5f },
		{ 0.5f + 0.5f, 1.0f + 0.5f, 1.0f + 0.5f },
		{ 0.0f + 0.5f, 1.0f + 0.5f, 0.5f + 0.5f },
		{ 0.0f + 0.5f, 0.5f + 0.5f, 0.0f + 0.5f },
		{ 1.0f + 0.5f, 0.5f + 0.5f, 0.0f + 0.5f },
		{ 1.0f + 0.5f, 0.5f + 0.5f, 1.0f + 0.5f },
		{ 0.0f + 0.5f, 0.5f + 0.5f, 1.0f + 0.5f },
	};

	VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA + 1>* container = new VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA + 1>();

	chunk.getVoxelContainer().decompress(*container);

	for (int x = -AMBIENT_OCCLUSION_SAMPLE_AREA - 1; x < CHUNK_SIZE + AMBIENT_OCCLUSION_SAMPLE_AREA + 1; x++) {
		for (int y = -AMBIENT_OCCLUSION_SAMPLE_AREA - 1; y < CHUNK_SIZE + AMBIENT_OCCLUSION_SAMPLE_AREA + 1; y++) {
			for (int z = -AMBIENT_OCCLUSION_SAMPLE_AREA - 1; z < CHUNK_SIZE + AMBIENT_OCCLUSION_SAMPLE_AREA + 1; z++) {
				if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
					container->setData(x, y, z, chunk.getVoxelDataRelative(x, y, z));
				}
			}
		}
	}

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				unsigned int index = 0;
				if (VoxelType::getFromID(container->getData(x, y, z).getID())->isOpaque()) index |= 1;
				if (VoxelType::getFromID(container->getData(x + 1, y, z).getID())->isOpaque()) index |= 2;
				if (VoxelType::getFromID(container->getData(x + 1, y, z + 1).getID())->isOpaque()) index |= 4;
				if (VoxelType::getFromID(container->getData(x, y, z + 1).getID())->isOpaque()) index |= 8;
				if (VoxelType::getFromID(container->getData(x, y + 1, z).getID())->isOpaque()) index |= 16;
				if (VoxelType::getFromID(container->getData(x + 1, y + 1, z).getID())->isOpaque()) index |= 32;
				if (VoxelType::getFromID(container->getData(x + 1, y + 1, z + 1).getID())->isOpaque()) index |= 64;
				if (VoxelType::getFromID(container->getData(x, y + 1, z + 1).getID())->isOpaque()) index |= 128;

				vt::Vec3f o((float)x, (float)y, (float)z);
				for (unsigned int i = 0; marchingCubesFaces[index][i] != -1; i += 3) {
					const vt::Vec3f& v0 = edges[marchingCubesFaces[index][i + 0]] + o;
					const vt::Vec3f& v1 = edges[marchingCubesFaces[index][i + 1]] + o;
					const vt::Vec3f& v2 = edges[marchingCubesFaces[index][i + 2]] + o;

					vt::Vec3f co((float)(chunk.getPosition().x * CHUNK_SIZE), (float)(chunk.getPosition().y * CHUNK_SIZE), (float)(chunk.getPosition().z * CHUNK_SIZE));

					float ao0 = getAmbientOcclusionMarchingCubes(*container, v0);
					float ao1 = getAmbientOcclusionMarchingCubes(*container, v1);
					float ao2 = getAmbientOcclusionMarchingCubes(*container, v2);

					vt::Vec3f normal = (v1 - v0).cross(v2 - v0).normalized();
					vt::Vec4b c0((vt::Vec3b)vt::Vec3f(255.0f * ao0), 255);
					vt::Vec4b c1((vt::Vec3b)vt::Vec3f(255.0f * ao1), 255);
					vt::Vec4b c2((vt::Vec3b)vt::Vec3f(255.0f * ao2), 255);
					mesh.triangle(VoxelVertex(v0 + co, c0, normal), VoxelVertex(v1 + co, c1, normal), VoxelVertex(v2 + co, c2, normal));
				}
			}
		}
	}


	delete container;
}

void ChunkMesher::createWaterMesh(Chunk& chunk, vt::IndexedMesh<VoxelVertex, unsigned int>& waterMesh, VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>* container)
{
	for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
		for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
			for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
				const VoxelData& data = container->getData(x, y, z);
				const Voxel* voxel = VoxelType::getFromID(data.getID());
				
				if (voxel->getID() != VoxelType::WATER.getID()) {
					continue;
				}
				
				const VoxelData& neighborTop = container->getData(x, y + 1, z);
				if (neighborTop.getID() != VoxelType::WATER.getID()) {
					vt::Vec3f o((float)((int)x + chunk.getPosition().x * CHUNK_SIZE), (float)((int)y + chunk.getPosition().y * CHUNK_SIZE) + 0.6f, (float)((int)z + chunk.getPosition().z * CHUNK_SIZE));
					vt::Vec3f p0 = vt::Vec3f(0.0f, 0.0f, 0.0f) + o;
					vt::Vec3f p1 = vt::Vec3f(1.0f, 0.0f, 0.0f) + o;
					vt::Vec3f p2 = vt::Vec3f(1.0f, 0.0f, 1.0f) + o;
					vt::Vec3f p3 = vt::Vec3f(0.0f, 0.0f, 1.0f) + o;
					vt::Vec3f normal(0.0f, 1.0f, 0.0f);
					vt::Vec4b c0 = vt::Vec4b(vt::Vec3b(32, 64, 128), 255);
					vt::Vec4b c1 = vt::Vec4b(vt::Vec3b(32, 64, 128), 255);
					vt::Vec4b c2 = vt::Vec4b(vt::Vec3b(32, 64, 128), 255);
					vt::Vec4b c3 = vt::Vec4b(vt::Vec3b(32, 64, 128), 255);
					VoxelVertex v0(p0, c0, normal);
					VoxelVertex v1(p1, c1, normal);
					VoxelVertex v2(p2, c2, normal);
					VoxelVertex v3(p3, c3, normal);
					waterMesh.getIndices().push_back(waterMesh.getVertices().size() + 0);
					waterMesh.getIndices().push_back(waterMesh.getVertices().size() + 1);
					waterMesh.getIndices().push_back(waterMesh.getVertices().size() + 2);
					waterMesh.getIndices().push_back(waterMesh.getVertices().size() + 3);
					waterMesh.getIndices().push_back(waterMesh.getVertices().size() + 0);
					waterMesh.getIndices().push_back(waterMesh.getVertices().size() + 2);
					waterMesh.getVertices().push_back(v0);
					waterMesh.getVertices().push_back(v1);
					waterMesh.getVertices().push_back(v2);
					waterMesh.getVertices().push_back(v3);
				}
			}
		}
	}
}

void ChunkMesher::createGrassMesh(Chunk& chunk, vt::IndexedMesh<VoxelVertex, unsigned int>& grassMesh, VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>* container)
{
	for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
		for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
			for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
				const VoxelData& data = container->getData(x, y, z);
				const Voxel* voxel = VoxelType::getFromID(data.getID());

				if (voxel->getID() != VoxelType::GRASS_CULMS.getID()) {
					continue;
				}

				vt::Vec3f o((float)((int)x + chunk.getPosition().x * CHUNK_SIZE), (float)((int)y + chunk.getPosition().y * CHUNK_SIZE), (float)((int)z + chunk.getPosition().z * CHUNK_SIZE));
				
				float width = 0.125f;

				for (int i = 0; i < 8; i++) {
					float rotation = (rand() % 64) / 64.0f * 3.141592f * 2.0f;
					float height = 0.3f + ((rand() % 16) / 16.0f * 0.4f);
					float x = (rand() % 64) / 64.0f;
					float z = (rand() % 64) / 64.0f;

					vt::Vec3f p0 = vt::Vec3f(cos(rotation) * width / 2.0f + x, 0.0f, sin(rotation) * width / 2.0f + z) + o;
					vt::Vec3f p1 = vt::Vec3f(-cos(rotation) * width / 2.0f + x, 0.0f, -sin(rotation) * width / 2.0f + z) + o;
					vt::Vec3f p2 = vt::Vec3f(-cos(rotation) * width / 2.0f + x, height, -sin(rotation) * width / 2.0f + z) + o;
					vt::Vec3f p3 = vt::Vec3f(cos(rotation) * width / 2.0f + x, height, sin(rotation) * width / 2.0f + z) + o;

					vt::Vec3f normal(0.0f, 1.0f, 0.0f);
					vt::Vec4b c0 = vt::Vec4b(VoxelType::GRASS.getColor(), 255);
					vt::Vec4b c1 = vt::Vec4b(VoxelType::GRASS.getColor(), 255);
					vt::Vec4b c2 = vt::Vec4b(VoxelType::GRASS.getColor(), 255);
					vt::Vec4b c3 = vt::Vec4b(VoxelType::GRASS.getColor(), 255);
					VoxelVertex v0(p0, c0, normal);
					VoxelVertex v1(p1, c1, normal);
					VoxelVertex v2(p2, c2, normal);
					VoxelVertex v3(p3, c3, normal);
					grassMesh.getIndices().push_back(grassMesh.getVertices().size() + 0);
					grassMesh.getIndices().push_back(grassMesh.getVertices().size() + 1);
					grassMesh.getIndices().push_back(grassMesh.getVertices().size() + 2);
					grassMesh.getIndices().push_back(grassMesh.getVertices().size() + 3);
					grassMesh.getIndices().push_back(grassMesh.getVertices().size() + 0);
					grassMesh.getIndices().push_back(grassMesh.getVertices().size() + 2);
					grassMesh.getVertices().push_back(v0);
					grassMesh.getVertices().push_back(v1);
					grassMesh.getVertices().push_back(v2);
					grassMesh.getVertices().push_back(v3);
				}
			}
		}
	}
}

VoxelQuad ChunkMesher::getQuad(const Chunk& chunk, VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>& container, int x, int y, const vt::Vec3f& p0, const vt::Vec3f& p1, const vt::Vec3f& p2, const vt::Vec3f& p3, const vt::Vec3b& color, const vt::Vec3f normal)
{
#if AMBIENT_OCCLUSION_HEMISPHERE
	float ao0 = getAmbientOcclusion(container, p0, normal);
	float ao1 = getAmbientOcclusion(container, p1, normal);
	float ao2 = getAmbientOcclusion(container, p2, normal);
	float ao3 = getAmbientOcclusion(container, p3, normal);
#else
	float ao0 = getAmbientOcclusion(container, p0);
	float ao1 = getAmbientOcclusion(container, p1);
	float ao2 = getAmbientOcclusion(container, p2);
	float ao3 = getAmbientOcclusion(container, p3);
#endif
	ao0 *= ao0;
	ao1 *= ao1;
	ao2 *= ao2;
	ao3 *= ao3;
	vt::Vec4b c0 = vt::Vec4b((vt::Vec3b)((vt::Vec3f)color * ao0), 255);
	vt::Vec4b c1 = vt::Vec4b((vt::Vec3b)((vt::Vec3f)color * ao1), 255);
	vt::Vec4b c2 = vt::Vec4b((vt::Vec3b)((vt::Vec3f)color * ao2), 255);
	vt::Vec4b c3 = vt::Vec4b((vt::Vec3b)((vt::Vec3f)color * ao3), 255);
	vt::Vec3f chunkOffset = (vt::Vec3f)(chunk.getPosition() * CHUNK_SIZE);
	return VoxelQuad(x, y, 1, 1, p0 + chunkOffset, p1 + chunkOffset, p2 + chunkOffset, p3 + chunkOffset, c0, c1, c2, c3, normal, ao0 + ao2 < ao1 + ao3);
}

void ChunkMesher::optimizeSlice(VoxelQuad* quads)
{
	VoxelQuad* current = nullptr;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			if (current == nullptr) {
				current = &quads[x + y * CHUNK_SIZE];
				continue;
			}
			if (current->canMerge(quads[x + y * CHUNK_SIZE])) {
				current->merge(quads[x + y * CHUNK_SIZE]);
			}
			else {
				current = &quads[x + y * CHUNK_SIZE];
			}
		}
	}
	current = nullptr;
	for (int y = 0; y < CHUNK_SIZE; y++) {
		for (int x = 0; x < CHUNK_SIZE; x++) {
			if (current == nullptr) {
				current = &quads[x + y * CHUNK_SIZE];
				continue;
			}
			if (current->canMerge(quads[x + y * CHUNK_SIZE])) {
				current->merge(quads[x + y * CHUNK_SIZE]);
			}
			else {
				current = &quads[x + y * CHUNK_SIZE];
			}
		}
	}
}

float ChunkMesher::getAmbientOcclusion(VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>& container, const vt::Vec3f& vertex)
{
	float ambientOcclusion = 0.0f;
	float numSamples = 0.0f;
	for (int x = (int)vertex.x - AMBIENT_OCCLUSION_SAMPLE_AREA; x < (int)vertex.x + AMBIENT_OCCLUSION_SAMPLE_AREA; x++) {
		for (int y = (int)vertex.y - AMBIENT_OCCLUSION_SAMPLE_AREA; y < (int)vertex.y + AMBIENT_OCCLUSION_SAMPLE_AREA; y++) {
			for (int z = (int)vertex.z - AMBIENT_OCCLUSION_SAMPLE_AREA; z < (int)vertex.z + AMBIENT_OCCLUSION_SAMPLE_AREA; z++) {
				if (!VoxelType::getFromID(container.getData(x, y, z).getID())->isOpaque()) {
					ambientOcclusion++;
				}
				numSamples++;
			}
		}
	}
	return ambientOcclusion / numSamples;
}

float ChunkMesher::getAmbientOcclusion(VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>& container, const vt::Vec3f& vertex, const vt::Vec3f& normal)
{
	int xo = normal.x > 0 ? 0 : -AMBIENT_OCCLUSION_SAMPLE_AREA;
	int yo = normal.y > 0 ? 0 : -AMBIENT_OCCLUSION_SAMPLE_AREA;
	int zo = normal.z > 0 ? 0 : -AMBIENT_OCCLUSION_SAMPLE_AREA;
	int xs = normal.x < 0 ? 0 : AMBIENT_OCCLUSION_SAMPLE_AREA;
	int ys = normal.y < 0 ? 0 : AMBIENT_OCCLUSION_SAMPLE_AREA;
	int zs = normal.z < 0 ? 0 : AMBIENT_OCCLUSION_SAMPLE_AREA;

	float ambientOcclusion = 0.0f;
	float numSamples = 0.0f;

	for (int x = (int)vertex.x + xo; x < (int)vertex.x + xs; x++) {
		for (int y = (int)vertex.y + yo; y < (int)vertex.y + ys; y++) {
			for (int z = (int)vertex.z + zo; z < (int)vertex.z + zs; z++) {
				float length = abs(vertex.x - 0.5f - (float)x) + abs(vertex.y - 0.5f - (float)y) + abs(vertex.z - 0.5f - (float)z);
				float sample = 1.0f / length;
				if (!VoxelType::getFromID(container.getData(x, y, z).getID())->isOpaque()) {
					ambientOcclusion += sample;
				}
				numSamples += sample;
			}
		}
	}
	return ambientOcclusion / numSamples;
}

float ChunkMesher::getAmbientOcclusionMarchingCubes(VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA + 1>& container, const vt::Vec3f& vertex)
{
	float ambientOcclusion = 0.0f;
	float numSamples = 0.0f;
	for (int x = (int)floor(vertex.x) - AMBIENT_OCCLUSION_SAMPLE_AREA; x < (int)floor(vertex.x) + AMBIENT_OCCLUSION_SAMPLE_AREA; x++) {
		for (int y = (int)floor(vertex.y) - AMBIENT_OCCLUSION_SAMPLE_AREA; y < (int)floor(vertex.y) + AMBIENT_OCCLUSION_SAMPLE_AREA; y++) {
			for (int z = (int)floor(vertex.z) - AMBIENT_OCCLUSION_SAMPLE_AREA; z < (int)floor(vertex.z) + AMBIENT_OCCLUSION_SAMPLE_AREA; z++) {
				if (!VoxelType::getFromID(container.getData(x, y, z).getID())->isOpaque()) {
					ambientOcclusion++;
				}
				numSamples++;
			}
		}
	}
	return ambientOcclusion / numSamples;
}
