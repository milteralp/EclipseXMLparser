#pragma once

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

class TrackingPoints{
public:


	// Number of features to extract on a frame.
	const int numberOfFeatures = 40;

	// Two consequent frames will be stored
	// void getFrame(int frameNumber); // Frame numbers will not be used, so comment out
	void getFrame();

	// goodFeaturesToTrack() is used for extracting features of two consecutive frames.
	// void getFeatures();
	void getFeatures(Mat framePrev, Mat frameNext);

	// Calculates the Euclidean distance between two points.
	float calcEuclideanDist(Point2f p1, Point2f p2);

	// Creates a histogram on given vector and returns threshold value
	// float createHist(vector<float> &distVector, int histSize);
	float createHist(vector<float> &distVector, int width, int height);


	// Returns first n bins that populates more than %80 of the histogram.
	int histPopulation(Mat hist, int bins);

	// Fills the vector with intersection points.
	void fillIntersect(vector<float> &distVec, vector<int> &indexVec, float thresh);

	// Draw circle on points in vector on given frame
	void drawCircleOnPoints(vector<Point2f> vec, Mat frame);

	// Display previous, next and intersection points frame.
	void displayFrames();

	// Calculates distance between two vectors
	int calcDistance(vector<Point2f> &vec1, vector<Point2f> &vec2);

	// Returns current frame number
	int getCurrentFrameNumber();

	// Returns the current frame
	Mat getCurrentFrame();

	// Calculating distance using threshold based on the value that is populating %80 percent of the histogram.
	// void histDistance(vector<Point2f> &vec1, vector <Point2f> &vec2, float minDistance);

	Mat framePrev, frameNext, frameIntersection;
	vector<Point2f> featuresPrev, featuresNext, featuresP2N, featuresN2P, featuresPNP; // FeaturesPrevToNext, FeaturesNextToPrev, FeaturesPrevToNextToPrev
	VideoCapture cap;

	// number of non-intersecting points
	int numDifferences = 0;

};
