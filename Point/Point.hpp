struct Point
{
	int x, y, z, value;
	int clusterIndex;
	double minDist;

	Point();
	~Point() = default;
	Point(int x, int y, int z, int value, int clusterIndex);
	double CalculateEuclideanDistance(const Point& point);
	double CalculateSquaredDistance(const Point& point);
	double CalculateTaxicabDistance(const Point& point);
};