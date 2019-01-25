#include "Main.h"
#include "Points.h"
#include <iostream>


int main()
{
	int UserNumOfPoints, k, i = 1;
	cout << "Enter number of points: ";	cin >> UserNumOfPoints;
	cout << "Enter the number of clusters: ";	cin >> k;
	auto start = std::chrono::system_clock::now();
	CMain Pam(UserNumOfPoints);
	Pam.BuildStep();
	Pam.SetDistVector();
	
	Pam.FindNextMedoid(k);
	while (Pam.MinSumRow.size() != 0)
	{
		vector<float> ChosenMedoid = Pam.GetVectorByIndex(Pam.MedoidList[i]);
		bool res = Pam.SwapStep(k - 1, ChosenMedoid);
		if (res == true)
			continue;
		else if (res == false && i == k - 1)
			break;
		else
			i++;
	}
	Pam.GetPointsFromMedoid(k);
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	cout << "finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";
	//plot
	_getch();
}


CMain::CMain(int _size)
{
	int UserAns = 1;
	NumberOfPoints = _size;
	cout << "Press 1 for randomize the points or 2 for insert you points" << endl;//	cin >> UserAns;
	srand(time(NULL));
	MedoidList = vector<int>(0);
	if (UserAns == 1)											//defaults points
	{
		int i = 0;
		for (int i = 0; i < NumberOfPoints; ++i)
			DataPoints[i] = CPoints(rand() % 15 + 1, rand() % 15 + 1);
	}
	else
	{
		float x, y;
		cout << "Enter your points" << endl;
		NumberOfPoints = _size;
		for (int i = 0; i < NumberOfPoints; ++i)
		{
			cout << "Point number " << i + 1 << endl;
			cin >> x >> y;
			DataPoints[i] = CPoints(x, y);
		}
	}

}

void CMain::BuildStep()
{
	float *temp;
	for (int i = 0; i < NumberOfPoints; i++)
	{
		temp = DataPoints[i].ManhattanDistance(DataPoints, NumberOfPoints);
		for (int j = NumberOfPoints * i, k = 0; j < NumberOfPoints * i + NumberOfPoints; j++, k++)
		{
			DistanceMatrix[j] = temp[k];
		}
	}
	MedoidList.insert(MedoidList.begin(), CalcMinDistance());
}

int CMain::CalcMinDistance()
{
	float sum = 0, min = PadKapaoo;
	float *res = new float[NumberOfPoints];
	int IndexToReturn = 0;
	for (int row = 0; row < NumberOfPoints; row++)
	{
		for (int col = 0; col < NumberOfPoints; col++)
			sum += DistanceMatrix[row * NumberOfPoints + col];
		if (sum < min) {
			min = sum;
			IndexToReturn = row;
		}
		sum = 0;
	}
	return IndexToReturn;
}

int CMain::GetIndexByVector(float* MyVec)			// input: array of float , output: index in distance matrix
{
	int i, j;
	vector<float>MedoidVector, temp;
	for (i = 0; i < NumberOfPoints; i++)
		MedoidVector.push_back(MyVec[i]);			//convert to Vector

	for (i = 0, j = NumberOfPoints; i < NumberOfPoints; i++)
	{
		for (int k = 0; k < NumberOfPoints; k++)
			temp.push_back(DistanceMatrix[i*NumberOfPoints + k]);
		if (MedoidVector == temp)
			break;
		temp.clear();
	}
	return i;
}

void CMain::FindNextMedoid(int k)
{
	int IndexOfNextMedoid ;
	float TempMinSum = 0.0, MinSum = PadKapaoo;
	vector<float*>::iterator it;
	auto iterator = MedoidList.front();
	vector<float*> TempMatrix;
	//Create copy of distance matrix without selected medoid
	for (int row = 0; row < NumberOfPoints; row++)
	{
		if (iterator == row)
			continue;
		float *line = new float[NumberOfPoints];
		for (int col = NumberOfPoints * row, index = 0; col < NumberOfPoints * row + NumberOfPoints; col++, index++) {
			line[index] = DistanceMatrix[col];
		}
		TempMatrix.push_back(line);
	}
	while (k != MedoidList.size())
	{
		//find the next medoid
		for (it = TempMatrix.begin(); it < TempMatrix.end(); it++)
		{
			for (int i = 0; i < NumberOfPoints; i++)
				TempMinSum += min((*it)[i], DistanceMatrix[MedoidList[0] * NumberOfPoints + i]);

			if (TempMinSum < MinSum)
			{
				IndexOfNextMedoid = it - TempMatrix.begin();
				MinSum = TempMinSum;
			}
			TempMinSum = 0.0;
		}
		it = TempMatrix.begin();
		int res = GetIndexByVector(it[IndexOfNextMedoid]);
		MedoidList.push_back(res);							//found the next medoid and isert it to the list
		TempMatrix = DeleteVectorFromMatrix(TempMatrix, IndexOfNextMedoid);
		MinSumRow.push_back(MinSum);
		MinSum = PadKapaoo;
	}
}

vector<float*> CMain::DeleteVectorFromMatrix(vector<float*> Matrix, int row)
{
	auto iterator = Matrix.begin() + row;
	Matrix.erase(iterator);
	return Matrix;
}

bool CMain::SwapStep(int k, vector<float> ChosenMedoid)
{
	int IndexOfNextMedoid = -1;
	float TempMinSum = 0;
	float MinToSwap = MinSumRow.at(0);
	vector<float*>::iterator it = Dist.begin();
	float *t = &ChosenMedoid[0];
	int ChosenMedoidIndex = GetIndexByVector(t);
	vector<float*> TempMatrix;
	float* index;
	for (int i = 0; i < NumberOfPoints; i++)
	{
		if (!(std::find(MedoidList.begin(), MedoidList.end(), i) != MedoidList.end()))
			TempMatrix.push_back(*it);
		it++;
	}


	//search for better medoid if exist
	for (it = TempMatrix.begin(); it < TempMatrix.end(); it++)
	{
		for (int i = 0; i < NumberOfPoints; i++)
			TempMinSum += min((*it)[i], ChosenMedoid.at(i));
		if (TempMinSum < MinToSwap)
		{
			IndexOfNextMedoid = it - TempMatrix.begin();
			MinToSwap = TempMinSum;
		}
		TempMinSum = 0.0;
	}
	MinSumRow.erase(MinSumRow.begin());
	if (IndexOfNextMedoid != -1)									//found a better medoid
	{
		MedoidList.erase(MedoidList.begin());
		it = TempMatrix.begin();
		MedoidList.push_back(GetIndexByVector(it[IndexOfNextMedoid]));
		MinSumRow.push_back(MinToSwap);
		return true;
	}
	return false;

}

vector<float> CMain::GetVectorByIndex(int index)
{
	vector<float> res;
	for (int i = 0; i < NumberOfPoints; i++)
		res.push_back(DistanceMatrix[index*NumberOfPoints + i]);
	return res;
}

void CMain::SetDistVector()
{
	for (int i = 0; i < NumberOfPoints; i++)
	{
		float *arr = new float[NumberOfPoints];
		for (int j = 0; j < NumberOfPoints; j++)
			arr[j] = DistanceMatrix[NumberOfPoints* i + j];
		Dist.push_back(arr);
	}
}

void CMain::GetPointsFromMedoid(int k)
{
	int flag;
	float *temp = new float[k];
	int IndexOfCluster;
	float MinTemp = 0;
	vector<CPoints> PointsThatAreNotMedoids;
	vector<vector<CPoints>> Clusters(k);
	for (int i = 0; i < MedoidList.size(); i++)
		Clusters[i].push_back(CPoints(DataPoints[MedoidList.at(i)].x, DataPoints[MedoidList.at(i)].y));
	for (int i = 0; i < NumberOfPoints; i++)
	{
		flag = 0;
		for (int j = 0; j < Clusters.size(); j++)
			temp[j] = abs(DataPoints[i].x - Clusters[j].front().x) + abs(DataPoints[i].y - Clusters[j].front().y);
		MinTemp = temp[0];
		IndexOfCluster = 0;
		for (int k = 0; k < Clusters.size(); k++)
		{
			if (temp[k] < MinTemp) {
				MinTemp = temp[k];
				IndexOfCluster = k;
			}
		}
		Clusters[IndexOfCluster].push_back(CPoints(DataPoints[i].x, DataPoints[i].y));
	}
	for (int i = 0; i < Clusters.size(); i++)
	{
		int flag = 0;
		cout << "Cluster Number " << i + 1 << " has this points:";
		for (auto iterator : Clusters[i])
		{
			if ((Clusters[i].front() == iterator) && (!flag))		//skip on the same point
			{
				flag++;
				continue;
			}
			else
				cout << "(" << iterator.x << "," << iterator.y << ")";
		}
		cout << endl << endl;
	}

}



