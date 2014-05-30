// Takes an arguments from command line which should be the frame number.

#include "../Header/TrackingPoints.h"


// Two frames will be stored
void TrackingPoints::getFrame(){

	cap.open("/home/visus/Desktop/kavsak.avi");

	// if((frameNumber > -1) && (frameNumber < cap.get(CV_CAP_PROP_FRAME_COUNT)) ){
	if( !cap.isOpened() )
		cout << "\n----------------Video cannot be started.-----------------------\n" << endl;
	while(true){
		framePrev = getCurrentFrame();
		frameNext = getCurrentFrame();
		if((framePrev.data == NULL ) || (frameNext.data == NULL)){
			cout << "Video is over" << endl;
			break;
		}
		else
			getFeatures(framePrev, frameNext);
		/*
		Mat currentFrame;
		cap >> currentFrame;

		if(frameNumber == cap.get(CV_CAP_PROP_POS_FRAMES) ){
			framePrev = currentFrame;
			cap.read(frameNext);
			break;
		}
		*/
	}
	// To check manually whether they are true.
	// imwrite("FramePrev.jpg", framePrev);
	// imwrite("FrameNext.jpg", frameNext);
	// else
		// cerr << "Frame number you have given is out of bounds" << endl;
}

int TrackingPoints::getCurrentFrameNumber(){
	return cap.get(CV_CAP_PROP_POS_FRAMES);
}


Mat TrackingPoints::getCurrentFrame(){
	Mat frame;
	if( getCurrentFrameNumber() == cap.get(CV_CAP_PROP_FRAME_COUNT - 1) )
		frame.data = NULL;
	else
		cap >> frame;
	return frame;
}


// Calculates the Euclidean distance between two points.
float TrackingPoints::calcEuclideanDist(Point2f p1, Point2f p2){
	float result = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	// return sqrt(result); // No need for square root, result is too small
	return result; // see above line
}


// goodFeaturesToTrack() is used for extracting features of two consecutive frames.
// void TrackingPoints::getFeatures(){
void TrackingPoints::getFeatures(Mat framePrev, Mat frameNext){
	Mat grayPrev, grayNext, errP2N, errN2P, errPNP;
	// vector<Point2f> featuresPrev, featuresNext, featuresP2N, featuresN2P; // FeaturesPrevToNext, FeaturesNextToPrev
	vector<uchar> statusP2N, statusN2P, statusPNP;

	cvtColor(framePrev, grayPrev, CV_BGR2GRAY);
	cvtColor(frameNext, grayNext, CV_BGR2GRAY);

	goodFeaturesToTrack(grayPrev, 			// (t) frame
						featuresPrev, 		// output detected corners
						numberOfFeatures,	// maximum number of features
						0.01, 				// quality level
						0.0, 				// min Euclidean distance between detected corners.
						Mat()); 			// mask

	goodFeaturesToTrack(grayNext, 			// (t+1) frame
						featuresNext, 		// output detected corners
						numberOfFeatures, 	// maximum number of features
						0.01, 				// quality level
						0.0, 				// min Euclidean distance between detected corners.
						Mat()); 			// mask

	calcOpticalFlowPyrLK(grayNext,
						 grayPrev,
						 featuresNext,
						 featuresN2P,
						 statusN2P,
						 errN2P);

	calcOpticalFlowPyrLK(grayPrev, 		// (t) frame
						 grayNext, 		// (t+1) frame
						 featuresPrev, 	// detected corners
						 featuresP2N, 	// output new positions of corners in next image.
						 statusP2N, 	// output status vector, 1 if corresponding feature is found
						 errP2N); 		// output error vector, error if corresponding feature has one


	calcOpticalFlowPyrLK(grayNext,		// (t+1) frame
						 grayPrev,		// (t) frame
						 featuresP2N, 	// Vector of points which flow needs to be found
						 featuresPNP,	// New positions of input features of (t+1) frame in (t) frame
						 statusPNP,		// output status vector, 1 if corresponding feature is found
						 errPNP);		// output error vector, error if corresponding feature has one


	// cout << "featuresP2N: " << featuresP2N.size() << endl << "featuresN2P: " << featuresN2P.size() << endl;

//	cout << "featuresPNP size: " << featuresPNP.size() << " featuresP2N size " << featuresP2N.size() << endl;
/*
	for(unsigned int i = 0; i < featuresPNP.size(); i++){
		cout << "featuresPNP " << featuresPNP[i] << " featuresP2N " << featuresP2N[i] << endl;
	}
*/
	framePrev.copyTo(frameIntersection);

	cout << "Frame number: " << getCurrentFrameNumber()  << " ,different features " << calcDistance(featuresP2N, featuresPNP) << endl;

/*
	cout << "At time t+1 " << endl;
	for(unsigned int i = 0; i < featuresP2N.size() ; i++){
		cout << "Prev to Next " << featuresP2N[i] << endl << "Corners from next" << featuresNext[i] << endl; // At time t+1
	}
*/
/* -------------------------------HISTOGRAM---------------------------------------- */
//	// keeps intersection points of two frames.
//	vector<Point2f> intersection;
//
//	// keeps minimum distance between two feature points.
//	vector<float> distVec;
//
//	// keeps the index of the second vector of points for intersecting points.
//	vector<int> indexVec;
//
//	// featuresPrev and featuresN2P supposed to have same size as stated by the goodFeaturesToTrack() function
//	for(unsigned int i=0; i < featuresPrev.size(); i++ ){
//
//		// Assigning minDistance to frame.height + frame.width which is more than the greatest distance between two points in the frame.
//		float minDistance = grayNext.cols + grayNext.rows;
//		int minIndex = -1;
//		for(unsigned int j=0; j < featuresN2P.size(); j++){
//
//			// result is a*a + b*b
//			float result = calcEuclideanDist(featuresN2P[j], featuresPrev[i]);
//
//			if(result < minDistance){
//				minDistance = result;
//				minIndex = j;
//			}
//			/* MAGIC NUMBER, use histogram %80 for it. threshold
//			if(result < 2){ // result is a*a + b*b. It is assumed that a and b can be at most 1.
//				cout << i << "th point on featuresN2P is " << j << "th point on featuresPrev " <<  featuresN2P[i] << " " << featuresPrev[j] << endl;
//			}
//			*/
//		}
//		distVec.push_back(minDistance);
//		indexVec.push_back(minIndex);
//	}
//
//	// createHist creates a histogram on given vector and returns threshold value
//	float thresh = createHist(distVec,  grayNext.cols, grayNext.rows);
//
//	// points of intersection vector is filled
//	fillIntersect(distVec, indexVec, thresh);
/* -------------------------------HISTOGRAM---------------------------------------- */

}




// Calculates distance for two vectors using calcOpticalFlowPyrLK() twice, one for (t+1) frame to (t) frame
int TrackingPoints::calcDistance(vector<Point2f> &vec1, vector<Point2f> &vec2){

	if(vec1.size() != vec2.size()){
		cout << "These vectors don't have the same size" << endl;
		return numDifferences;
	}
	else {
		for(unsigned int i = 0; i < vec1.size(); i++){
			int result = calcEuclideanDist(vec1[i], vec2[i]);
			if(result > 1/2)
				numDifferences++;
		}
	}
	return numDifferences;
}


// Draw circle on points in vector on given frame
void TrackingPoints::drawCircleOnPoints(vector<Point2f> vec, Mat frame){
	for(unsigned int i = 0; i < vec.size(); i++){
		// cout << "intersection points: " << vec[i] << endl;
		circle(frame, vec[i], 3, Scalar(0, 255, 0), 1, 8);
	}
}


// Displays the previous and next frame.
void TrackingPoints::displayFrames(){
	namedWindow("Previous", CV_WINDOW_AUTOSIZE);
	namedWindow("Next", CV_WINDOW_AUTOSIZE);
	namedWindow("IntersectionPrev", CV_WINDOW_AUTOSIZE);

	imshow("Previous",framePrev);
	imshow("Next",frameNext);
	imshow("IntersectionPrev",frameIntersection);
	waitKey(); // Do not forget to add, otherwise frames will not be shown.
}



// Fills the vector with intersection points.
void TrackingPoints::fillIntersect(vector<float> &distVec, vector<int> &indexVec, float thresh){
	vector<Point2f> intersectionVec;

	cout << "thresh is: " << thresh << endl;
/*
	for(unsigned int p = 0; p < distVec.size(); p++){
		cout << "distVec point " << distVec[p] << endl;
	}
*/

	for(unsigned int i=0; i < distVec.size(); i++){

		// distVect[i] keeps distance between two closest points.
		// thresh is range of the histogram bin where histogram is populated %80 including that bin.

		// if the distance between two points is less than thresh, they are assumed to intersect.
		if(distVec[i] < thresh){

			// featuresPrev is pushed since it stores int values,
			intersectionVec.push_back(featuresPrev[i]);
			int index = indexVec[i];
			cout << "At time t, prev " << featuresPrev[i] << " n2p " << featuresN2P[index] << endl;
		}
	}
	cout << "Number of intersecting points " << intersectionVec.size() << endl;
/*
	for(unsigned int p = 0; p < intersectionVec.size(); p++){
		cout << "intersection point " << intersectionVec[p] << endl;
	}
*/
	drawCircleOnPoints(intersectionVec, frameIntersection);
	drawCircleOnPoints(featuresPrev, framePrev);
	drawCircleOnPoints(featuresNext, frameNext);
	displayFrames();
}



// Creates distance histogram and returns the threshold value.
float TrackingPoints::createHist(vector<float> &distIndexVec, int width, int height){


	Mat histogram_input(distIndexVec), histogram_output;
/*
	for(unsigned int p = 0; p < distIndexVec.size(); p++){
		cout << fixed << "size is" << distIndexVec.size() << " distIndexVec: " << distIndexVec[p] << endl;
	}
*/
	// Number of bins, maximum value is width + height so each bin will have size of 1
	int histSize = width + height;

	// Getting min and max values of distance vector.
	float maxDistVect = *max_element( distIndexVec.begin(), distIndexVec.end() );
	float minDistVect = *min_element( distIndexVec.begin(), distIndexVec.end() );
	cout << fixed << "Max Element:" << maxDistVect << " " << "Min Element:" << minDistVect << endl;

	// Setting range for histogram.
	float range[] = { minDistVect, maxDistVect };
	const float* histRange = { range };

	// Histogram is *NOT* uniform.
	bool uniform = true;

	// Values will not be accumulated on histogram.
	bool accumulate = false;

	calcHist( &histogram_input, // source
			  1,				// number of source images (could be list of arrays)
			  0,				// channels, (array is single channeled)
			  Mat(),			// mask
			  histogram_output,	// output histogram
			  1,				// histogram dimensionality
			  &histSize,		// number of bins in histogram
			  &histRange,		// histogram range
			  uniform,			// histogram is uniform
			  accumulate);		// histogram is not accumulated

	// cout << "histogram of distance "<< histogram_output << endl;
	// cout << "hist data" << histogram_output << endl;
	// cout << "number of values in first bin "<< histogram_output.size().height << endl;
	// cout << "Max " << maxDistVect << " min " << minDistVect << " max - min " << maxDistVect - minDistVect << "over 40 " << (maxDistVect - minDistVect)/histSize << endl;

	// Range of each bin in histogram
	float binRange = (maxDistVect - minDistVect) / histSize;

	// Number of bins that populates more than %80 of the histogram, index starts from 0 so 1 is added for calculating threshold value.
	int bins = histPopulation(histogram_output, histSize) + 1;

	// Threshold value
	float thresh = bins * binRange;

	return thresh;
}

// Returns first n bins that populates more than %80 of the histogram.
int TrackingPoints::histPopulation(Mat hist, int bins){

	// Number of values in histogram bins
	int valuesInBin = 0;
	// One dimensional histogram, so size is (1 x bins)
	for(int i = 0; i < hist.size().height; i++ ){
		valuesInBin = valuesInBin + hist.at<int>(i); // or hist.at<int>(i,1) ?
		if(valuesInBin / bins > 4/5){
			cout << "Until bin " << i << " 80 percent of the values" << endl;
			cout << "i is " << i << endl;
			return i;
		}
	}
	// This case should not be work if the histogram is populated. Above condition checks when the histogram is %80 populated.
	return -1;
}
