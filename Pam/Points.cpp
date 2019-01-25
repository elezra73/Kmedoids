#include "Points.h"
#include "Main.h"

using namespace std;



CPoints::CPoints()
{
}

CPoints::~CPoints()
{
}

float* CPoints::ManhattanDistance(CPoints DataPoints[], int NumberOfPoints)
{
	float *res = new float[NumberOfPoints];
	for (int i = 0; i < NumberOfPoints; i++)
		res[i] = abs(x - DataPoints[i].x) + abs(y - DataPoints[i].y);
	return res;
}

bool CPoints::operator==(const CPoints & rhs) const
{
	if ((rhs.x == x) && (rhs.y == y))
		return true;
	return false;
}





