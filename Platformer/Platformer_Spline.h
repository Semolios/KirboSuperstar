#ifndef DEF_SPLINE
#define	DEF_SPLINE

#pragma once
#include <vector>

struct sPoint2D
{
	float x;
	float y;
	float length;
};

class cSpline
{
private:
	std::vector<sPoint2D> points;
	float fTotalSplineLength;

public:
	sPoint2D GetSplinePoint(float t, bool bLooped = false);
	sPoint2D GetSplineGradient(float t, bool bLooped = false);
	float CalculateSegmentLength(int node, bool bLooped = false);
	float GetNormalisedOffset(float p);
	float GetTotalLength();
	void AddPoint(float x, float y);
	void SetTotalLength(float l);
	int GetPointsSize();
	void IncreaseLength(float l);
	void SetIPointLength(int i, float l);
	float GetIX(int i);
	float GetIY(int i);
};

#endif // !DEF_SPLINE
