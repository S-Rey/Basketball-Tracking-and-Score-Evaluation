//
// Created by Samuel on 22.12.15.
//

#include "Parabola.h"

Parabola::Parabola() {
    m_video.push_back(Keyframe(0,0,0,0));
    double p_x(0), p_y(0), p_z(0), v_x(0), v_y(1), v_z(10);
    const double g(-9.81), dt(1.0/30.0);

    for (int i = 1; i < 100; ++i) {
        p_x += dt * v_x;
        p_y += dt * v_y;
        p_z += dt * v_z + (pow(dt,2) * g) / 2.0;
        v_z += g * dt;
        Keyframe next_keyframe = Keyframe(i, p_x, p_y, p_z);
        m_video.push_back(next_keyframe);
        if(p_z < 0) {
            p_z *= -1;
            v_z *= -0.82;
        }
    }

}

const std::vector<Keyframe> &Parabola::getVideo() const {
    const std::vector<Keyframe>& r_video = m_video;
    return r_video;
}

std::vector<Keyframe> Parabola::getOnly(int step) const {
    std::vector<Keyframe> output;
    for (int i = 0; i < m_video.size(); i+=step) {
        output.push_back(m_video[i]);
    }

    return output;
}

void Parabola::randomize(double variation) {
    for(int i = 0; i < m_video.size(); ++i) {
        double diff_x = 2 * variation * ( (double)rand() / (double)RAND_MAX ) - variation + m_video[i].getX();
        double diff_y = 2 * variation * ( (double)rand() / (double)RAND_MAX ) - variation + m_video[i].getY();
        double diff_z = 2 * variation * ( (double)rand() / (double)RAND_MAX ) - variation + m_video[i].getZ();
        m_video[i] = Keyframe(m_video[i].getFrame(), diff_x, diff_y, diff_z);
    }
}


