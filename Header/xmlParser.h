#pragma once

#include "opencv2/opencv.hpp"
#include "Ellipse.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace cv;
using namespace std;

class xmlParser {
public:
	FileStorage fs;
	void parse(vector<Ellipse::Circle> circles);
};
