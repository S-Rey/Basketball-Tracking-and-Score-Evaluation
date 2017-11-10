//
// Created by Samuel on 02.12.15.
//

#include "Line.h"

Line::Line(Keyframe start, Keyframe end) {
    double diff_frame = end.getFrame() - start.getFrame();
    double dx = (end.getX() - start.getX()) / diff_frame;
    double dy = (end.getY() - start.getY()) / diff_frame;
    double dz = (end.getZ() - start.getZ()) / diff_frame;

    for (size_t t = start.getFrame(); t <= (end.getFrame() - start.getFrame()); ++t) {
        m_video.push_back(Keyframe(t, dx * (double)t, dy * (double)t, dz * (double)t));
    }
}

const std::vector<Keyframe> &Line::getVideo() const {
    const std::vector<Keyframe>& r_video = m_video;
    return r_video;
}

void Line::randomize(double variation) {
    for(int i = 0; i < m_video.size(); ++i) {
        double diff_x = 2 * variation * ( (double)rand() / (double)RAND_MAX ) - variation + m_video[i].getX();
        double diff_y = 2 * variation * ( (double)rand() / (double)RAND_MAX ) - variation + m_video[i].getY();
        double diff_z = 2 * variation * ( (double)rand() / (double)RAND_MAX ) - variation + m_video[i].getZ();
        m_video[i] = Keyframe(m_video[i].getFrame(), diff_x, diff_y, diff_z);
    }
}

std::vector<Keyframe> Line::getOnly(int step) const {
    std::vector<Keyframe> output;
    for (int i = 0; i < m_video.size(); i+=step) {
        output.push_back(m_video[i]);
    }

    return output;
}
