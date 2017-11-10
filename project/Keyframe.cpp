//
// Created by Samuel on 13.09.15.
//


#include "Keyframe.h"


int Keyframe::getFrame() const {
    return m_frame;
}

double Keyframe::getX() const {
    return m_x;
}

double Keyframe::getY() const {
    return m_y;
}

double Keyframe::getZ() const {
    return m_z;
}

Keyframe::Keyframe(int frame, double x, double y, double z) : m_frame(frame), m_x(x), m_y(y), m_z(z) { }

Keyframe::Keyframe(const Keyframe &keyframe) {
    m_frame = keyframe.getFrame();
    m_x     = keyframe.getX();
    m_y     = keyframe.getY();
    m_z     = keyframe.getZ();
}

Keyframe::Keyframe(const cv::Mat mat) {
    m_frame = 0;
    m_x = mat.at<double>(0,0);
    m_y = mat.at<double>(1,0);
    m_z = mat.at<double>(2,0);
}


std::ostream& operator<<(std::ostream &strm, const Keyframe &keyframe) {
    return strm << keyframe.getFrame() << ' ' <<
                   keyframe.getX() << ' ' <<
                   keyframe.getY() << ' ' <<
                   keyframe.getZ();
}

std::string Keyframe::writePositionOBJ() {
    std::ostringstream strs;
    strs << m_x << ' ' << m_z << ' ' << (-1*m_y);
    return strs.str();
}

double Keyframe::dist(Keyframe keyframe) {
    return std::sqrt(
            std::pow((this->getX() - keyframe.getX()),2) +
            std::pow((this->getY() - keyframe.getY()),2) +
            std::pow((this->getZ() - keyframe.getZ()),2)
    );
}


