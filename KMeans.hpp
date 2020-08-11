#include "Point/Point.hpp"
#include <vector>
#include <string>

class KMeans
{
public:
    KMeans() = default;
    KMeans(const std::vector<short>& data);

    void LoadData(const std::vector<short>& data);
    void CalculateKMeans(int numOfIterations, int numOfCentroids);

    std::vector<Point> GetPoints() const;
    
    std::vector<int> GetPointsAsVector() const;
    std::vector<int> GetIndexesAsVector() const;

	void PrintToFile() const;

private:
    std::vector<Point> points;
    std::vector<Point> centroids;

    void RandomCentroids(int numOfCentroids);
    bool ClusterData();
    void CalculateCentroids();
};
