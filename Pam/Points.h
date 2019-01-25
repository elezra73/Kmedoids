#pragma once
class CPoints
{
public:
	CPoints(float _x, float _y) : x(_x), y(_y) {};
	CPoints();
	~CPoints();

	float x;
	float y;
	float* ManhattanDistance(CPoints DataPoints[] , int NumberOfPoints);
	bool CPoints::operator==(const CPoints& rhs) const;

};

