//
// Created by Samuel on 28.09.15.
//

#ifndef PROJET_FUNCTION1_H
#define PROJET_FUNCTION1_H

#include <cmath>
#include <vector>
#include "Video.h"
#include "Vertex.h"
#include "Zone.h"
#include "KFilter.h"

struct Sample_video {
	std::vector<Keyframe> video;
	bool score;
};

struct Sample_info {
	int start;
	int end;
	bool score;
};

struct Score {
	int frame;          // when the score occured
	int basketL;        // how much point from basketL
	int basketR;        //  "   "     "     "  basketR
};

/**
 *
 */
double distance(Keyframe const keyframe1, Keyframe const keyframe2);
/**
 * 	Use the velocity to predict next position

    @param two positions and the time of the wanted frame
    @return the Keyframe of the
    bar and "," as the half bar.
 */
Keyframe estimatedNextKeyframe(Keyframe const keyframe1, Keyframe const keyframe2, size_t time);

bool inside_box(Keyframe keyframe, std::vector<Vertex> box);

double speed(Keyframe keyframe1, Keyframe keyframe2, int fps);

std::vector<Sample_video> create_samples_video (std::string detection, std::string separation);

std::vector<Sample_info> create_samples_info(std::string separation);

std::vector<bool> test_samples_Kalman_Filter(std::vector<Sample_video> input, std::vector<Zone> zones,
                                             std::vector<double> coefficients);

void writes_samples_Kalman_Filter(std::vector<Sample_video> input, std::vector<Zone> zones,
                                             std::vector<double> coefficients);

std::vector<bool> test_samples_Minimal_Distance(std::vector<Sample_video> input, std::vector<Zone> zones);

std::vector<bool> test_samples_Multiple_Step(std::vector<Sample_video> input, std::vector<Zone> zones, int step);

void samples_analyze(std::vector<Sample_video> input, std::vector<Zone> zones);

void statistics(const std::vector<bool> &results, const std::vector<Sample_video> &samples);

std::vector<Score> scoreDetection(const Video &video, std::vector<Zone> &zones);

Video minimal_distance(std::vector<std::vector<Keyframe>> videoAll);

Video multiple_step(std::vector<std::vector<Keyframe>> videoAll, size_t n_steps);


#endif //PROJET_FUNCTION1_H
