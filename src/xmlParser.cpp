#include "../Header/xmlParser.h"

//void xmlParser::parse(vector<Ellipse::Circle> * circles){
void xmlParser::parse(vector<Ellipse::Circle> circles){

	FileStorage fs("Crossroads_entrance_exit_points.xml", FileStorage::WRITE);
	fs << "crossroads_entrance_exit_points" << "[";
	for(int i=0; i<circles.size(); i++){
		Point2f point = circles[i].center;
		//char label = circles[i].entranceType; // delete below 3 lines.
		ostringstream s, x, y;
		s << "label" << i;
		string label = 	s.str();
		x << setprecision(8) << point.x;
		string PointX = x.str();
		y << setprecision(8) << point.y;
		string PointY = y.str();

		// Labels should be produced like on line9 but there is not any label yet, so it's produced like label1..n
		fs << "{:" << "id" << i << "PointX" << PointX << "PointY" << PointY << "Label" << label << "}";
	}
	fs << "]";
	fs.release();
}
