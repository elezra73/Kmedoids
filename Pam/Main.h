#pragma once
using namespace std;
#include "Points.h"
#include <vector>
#include <conio.h>
#include <cmath>
#include <algorithm>
#include <time.h> 
#include <chrono>
#include <ctime>

#define NumPoints 500
#define PadKapaoo 10000000

class CMain
{
public:
	CMain(int _size);

	vector<float*> Dist;
	CPoints DataPoints[NumPoints];
	float DistanceMatrix[NumPoints * NumPoints];
	vector<int> MedoidList;
	vector<float> MinSumRow;
	int NumberOfPoints;
	

	void BuildStep();
	int CalcMinDistance();
	int GetIndexByVector(float* MyVec);
	void FindNextMedoid(int k);
	vector<float*>DeleteVectorFromMatrix(vector<float*> Matrix, int row);
	bool SwapStep(int k, vector<float> ChosenMedoid);
	vector<float> GetVectorByIndex(int index);
	void SetDistVector();
	void GetPointsFromMedoid(int k);

};
