//
// Created by Samuel on 13.09.15.
//

#ifndef PROJET_FRAME_H
#define PROJET_FRAME_H


#include <string>
#include <sstream>
#include <iosfwd>
#include <cmath>
#include <opencv2/opencv.hpp>

class Keyframe {
private:
	int m_frame;
	double m_x;
	double m_y;
	double m_z;
public:
	int getFrame() const;

	double getX() const;

	double getY() const;

	double getZ() const;

	Keyframe(int frame, double x, double y, double z);

	Keyframe(const Keyframe & keyframe);

	Keyframe(const cv::Mat mat);

	std::string writePositionOBJ();

	double dist(Keyframe keyframe);


};
std::ostream& operator<<(std::ostream &strm, const Keyframe &keyframe);


#endif //PROJET_FRAME_H
