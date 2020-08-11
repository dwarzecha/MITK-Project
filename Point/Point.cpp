#include "Point.hpp"
#include <cmath>
#include <cstdlib>
#include <limits>


Point::Point() :
x(0), y(0), z(0), value(0), clusterIndex(-1), minDist(std::numeric_limits<double>::max())
{

}


Point::Point(int x, int y, int z, int value, int clusterIndex) :
x(x), y(y), z(z), value(value), clusterIndex(clusterIndex), minDist(std::numeric_limits<double>::max())
{

}

double Point::CalculateEuclideanDistance(const Point& point)
{
	return sqrt( (x - point.x) * (x - point.x) + (y - point.y) * (y - point.y) + (z - point.z) * (z - point.z) + (value - point.value) * (value - point.value) );
}

double Point::CalculateSquaredDistance(const Point& point)
{
    return (x - point.x) * (x - point.x) + (y - point.y) * (y - point.y) + (z - point.z) * (z - point.z) + (value - point.value) * (value - point.value);
}


double Point::CalculateTaxicabDistance(const Point& point)
{
    return ( abs(x - point.x) + abs(y - point.y) + abs(z - point.z) + abs(value - point.value) );
}

