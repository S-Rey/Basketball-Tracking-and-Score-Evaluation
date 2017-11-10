//
// Created by Samuel on 19.11.15.
//

#ifndef PROJET_KFILTER_H
#define PROJET_KFILTER_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include "Video.h"
#include "Zone.h"

//using namespace cv;

class KFilter {
    struct Score {
        int frame;          // when the score occured
        int basketL;        // how much point from basketL
        int basketR;        //  "   "     "     "  basketR
    };

private:
    double dt;
    std::vector<std::vector<Keyframe>> input;
    double walls[2][2];         // the walls of the court {{Xmin, Xmax},{Ymin, Ymax}}

    // {BoxL, BoxR}
    std::vector<Zone> zones;
    std::vector<int> zones_state;          // keep track of the value of isInside() for each zones
    std::vector<Score> scores;              // keep track of each basket

    cv::Mat F;                  // state transition matrix: expected position of the ball
    cv::Mat B;                  // input control matrix:    expected effect of the input acceleration on the state
    cv::Mat H;                  // measurement matrix:      expected measurement given the predicted state (likelihood)

    cv::Mat Q;                  // estimated error in process
    cv::Mat Q_in;               // estimated error in process inside box
    cv::Mat R;                  // estimated error in measurment
    cv::Mat R_in;               // estimated error in measurment inside box

    double q_out, q_in;              // q_out: when outside, q_in: when inside
    double r_out, r_in;              // r_out: when outside, r_in: when inside
    double height;                  // if above then very reliable

    cv::Mat S;                  // innovation covariance (Compare real error against prediction)
    cv::Mat S_in;               // innovation covariance (Compare real error against prediction) inside box

    cv::Mat y;                  // innovation (Compare reality against prediction)

    cv::Mat u;                  // define acceleration magnitude
    cv::Mat P;                  // position variance
    cv::Mat P_in;               // position variance inside box
    cv::Mat x;                  // x_estimate of initial location estimation of where the ball is (what we are updating)
    cv::Mat z;                  // measurment of the ball
    cv::Mat K;                  // Kalman gain
    cv::Mat K_in;               // Kalman gain inside box

public:
    KFilter (double dt, std::vector<std::vector<Keyframe>> video_all, std::vector<Zone> zones);
    KFilter (double dt, Video video, std::vector<Zone> zones);
    void setQ(double q_out, double q_in);
    void setR(double r_out, double r_in);

    Video run();
    void test();
    void matPrint(std::string name, cv::Mat mat, int prec);
    const std::vector<Score> &getScores();

};


#endif //PROJET_KFILTER_H
