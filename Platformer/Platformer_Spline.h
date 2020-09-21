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
public:
	std::vector<sPoint2D> points;
	float fTotalSplineLength;

public:
	sPoint2D GetSplinePoint(float t, bool bLooped = false);
	sPoint2D GetSplineGradient(float t, bool bLooped = false);
	float CalculateSegmentLength(int node, bool bLooped = false);
	float GetNormalisedOffset(float p);
};