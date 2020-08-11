#include "KMeans.hpp"
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>

#include "qfile.h"
#include "qfiledialog.h"
#include "qobject.h"
#include "qmessagebox.h"
#include "qtextstream.h"


KMeans::KMeans(const std::vector<short>& data)
{
    LoadData(data);
}


void KMeans::LoadData(const std::vector<short>& data)
{
    for (int i = 0; i < data.size(); i++)
    {
		Point temp(data.at(i++), data.at(i++), data.at(i++), data.at(i), -1);
        points.push_back(temp);
    }
}


void KMeans::CalculateKMeans(int numOfIterations, int numOfCentroids)
{  
    RandomCentroids(numOfCentroids);

    for (int i = 0; i < numOfIterations; i++)
    {
        bool clustersChanged = ClusterData();

		if (!clustersChanged)
		{
			break;
		}

        CalculateCentroids();
    }
}


std::vector<Point> KMeans::GetPoints() const
{
    return points;
}


std::vector<int> KMeans::GetPointsAsVector() const
{
    std::vector<int> temp;

    for (auto p : points)
    {
        temp.push_back(p.x);
		temp.push_back(p.y);
		temp.push_back(p.z);
		temp.push_back(p.value);
		temp.push_back(p.clusterIndex);
    }

    return temp;
}


std::vector<int> KMeans::GetIndexesAsVector() const
{
    std::vector<int> temp;

    for (auto p : points)
    {
        temp.push_back(p.clusterIndex);
    }

    return temp;
}


void KMeans::PrintToFile() const
{
	std::string output;

	for (auto p : points)
	{
		if (p.x < 10)
			output += ("    " + std::to_string(p.x));
		else if (p.x < 100)
			output += ("   " + std::to_string(p.x));
		else if (p.x < 1000)
			output += ("  " + std::to_string(p.x));
		else
			output += (" " + std::to_string(p.x));

		if (p.y < 10)
			output += ("    " + std::to_string(p.y));
		else if (p.y < 100)
			output += ("   " + std::to_string(p.y));
		else if (p.y < 1000)
			output += ("  " + std::to_string(p.y));
		else
			output += (" " + std::to_string(p.y));

		if (p.z < 10)
			output += ("    " + std::to_string(p.z));
		else if (p.z < 100)
			output += ("   " + std::to_string(p.z));
		else if (p.z < 1000)
			output += ("  " + std::to_string(p.z));
		else
			output += (" " + std::to_string(p.z));

		if (p.value < 10)
			output += ("    " + std::to_string(p.value));
		else if (p.value < 100)
			output += ("   " + std::to_string(p.value));
		else if (p.value < 1000)
			output += ("  " + std::to_string(p.value));
		else
			output += (" " + std::to_string(p.value));

		output += (" " + std::to_string(p.clusterIndex) + "\n");
	}

	QString fileName = QFileDialog::getSaveFileName(nullptr,
		QObject::tr("Save Clustering Results"), "",
		QObject::tr("Text File (*.txt);;All Files (*)"));

	if (fileName.isEmpty())
		return;
	else
	{
		QFile file(fileName);

		if (!file.open(QIODevice::WriteOnly))
		{
			QMessageBox::information(nullptr, QObject::tr("Unable to open file"),
				file.errorString());
			return;
		}

		QTextStream out(&file);
		out << output.c_str();
	}
}


void KMeans::RandomCentroids(int numOfCentroids)
{
    srand(time(NULL));

    for (int i = 0; i < numOfCentroids; i++)
    {
        centroids.push_back(points.at(rand() % points.size()));
    }   
}


bool KMeans::ClusterData()
{
	bool clustersChanged = false;

    for (auto& p : points)
    {
		int i = 0;
		int lastIndex = p.clusterIndex;

		for (auto c : centroids)
		{
			double dist = c.CalculateSquaredDistance(p);

			if (dist < p.minDist)
			{
				p.minDist = dist;
				p.clusterIndex = i;
			}

			i++;
		}

		if (!clustersChanged && (lastIndex != p.clusterIndex))
		{
			clustersChanged = true;
		}
    }

	return clustersChanged;
}


void KMeans::CalculateCentroids()
{
	std::vector<int> sumX, sumY, sumZ, sumValue;
	std::vector<int> numOfPoints;

    for (int i = 0; i < centroids.size(); i++)
    {
        sumX.push_back(0);
        sumY.push_back(0);
        sumZ.push_back(0);
        sumValue.push_back(0);

        numOfPoints.push_back(0);
    }
    

    for (auto& p : points)
    {
        int clusterIndex = p.clusterIndex;

        sumX.at(clusterIndex) += p.x;
        sumY.at(clusterIndex) += p.y;
        sumZ.at(clusterIndex) += p.z;
        sumValue.at(clusterIndex) += p.value;

        numOfPoints.at(clusterIndex)++;

        p.minDist = std::numeric_limits<double>::max();
    }

    for (int i = 0; i < centroids.size(); i++)
    {
        centroids.at(i).x = sumX.at(i) / numOfPoints.at(i);
        centroids.at(i).y = sumY.at(i) / numOfPoints.at(i);
        centroids.at(i).z = sumZ.at(i) / numOfPoints.at(i);
        centroids.at(i).value = sumValue.at(i) / numOfPoints.at(i);
    }
}