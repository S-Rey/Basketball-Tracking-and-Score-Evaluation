//
// Created by Samuel on 28.09.15.
//

#include "Functions.h"


double distance(Keyframe const keyframe1, Keyframe const keyframe2) {
    return std::sqrt(
            std::pow((keyframe1.getX() - keyframe2.getX()),2) +
            std::pow((keyframe1.getY() - keyframe2.getY()),2) +
            std::pow((keyframe1.getZ() - keyframe2.getZ()),2)
        );
}

Keyframe estimatedNextKeyframe(Keyframe const keyframe1, Keyframe const keyframe2, size_t next_time) {
    double diff_frame = keyframe2.getFrame() - keyframe1.getFrame();
    double x = (keyframe2.getX() - keyframe1.getX()) / diff_frame;
    double y = (keyframe2.getY() - keyframe1.getY()) / diff_frame;
    double z = (keyframe2.getZ() - keyframe1.getZ()) / diff_frame;
    double frame = next_time - keyframe2.getFrame();

    return Keyframe(next_time, x * frame, y * frame, z * frame);
}

/**
 * Check if the position of the keyframe is inside the box
 */
bool inside_box(Keyframe keyframe, std::vector<Vertex> box) {
    //if()
    return false;
}
/**
 * Compute the speed of the ball between two keyframe
 *
 * @param two keyframe and the fps (if fps = 1 then return the speed per frame)
 */
double speed(Keyframe keyframe1, Keyframe keyframe2, int fps) {
    double diff_frame = keyframe2.getFrame() - keyframe1.getFrame();
    double key_distance = distance(keyframe1, keyframe2);
    return key_distance * fps /diff_frame;
}

std::vector<Sample_video> create_samples_video(std::string detection, std::string separation) {

    std::vector<Sample_video> samples_video;
    Video video_d(detection);
    std::vector<Keyframe>  video = video_d.getVideo();

    std::vector<Sample_info> samples_info = create_samples_info(separation);
    int key_info(0);
    int start;
    int end;
    bool score;
    for (int i = 0; i < samples_info.size(); ++i) {
        start = samples_info[i].start;
        end = samples_info[i].end;
        score = samples_info[i].score;

        int key_start(key_info);
        while(video[key_start].getFrame() < start) {
            ++key_start;
        }

        int key_end(key_start);
        while(video[key_end].getFrame() < end) {
            ++key_end;
        }

        std::vector<Keyframe> new_video;

        for (int j = key_start; j <= key_end; ++j) {
            new_video.push_back(video[j]);
        }

        samples_video.push_back(Sample_video{new_video, score});
        key_info = key_end;
    }

    return samples_video;
}

std::vector<Sample_info> create_samples_info(std::string separation) {
    std::vector<Sample_info> samples_info;
    std::string line;
    std::ifstream input(separation, std::ios::in);

    if(input.is_open()) {
        while(getline(input, line)) {
            std::istringstream iss(line);
            int start;
            int end;
            int score; // will be converted to boolean
            if(!(iss >> start >> end >> score)) {
                break;
            };
            samples_info.push_back(Sample_info{start,
                                               end,
                                               !!score // convert to boolean
            });
        }
        std::cout << "Separation Importation Done" << std::endl;
    } else {
        std::cerr << "/!\\ Separation Importation Error! /!\\" << std::endl;
    }
    input.close();
    return samples_info;
}

std::vector<bool> test_samples_Kalman_Filter(std::vector<Sample_video> input, std::vector<Zone> zones,
                                             std::vector<double> coefficients) {
    std::vector<bool> results;
    for (int i = 0; i < input.size(); ++i) {
        Video video(input[i].video);
        std::stringstream sstm;
        sstm << "sample" << i;
        video.writeOBJ("../../output/OBJ_files/samples/",sstm.str(),false);

        KFilter kfilter(1.0/30.0, video.getAll(), zones);
        if(coefficients.size() != 0) {
            kfilter.setQ(coefficients[0], coefficients[1]);
            kfilter.setR(coefficients[2], coefficients[3]);
        }
        Video kfilter_result = kfilter.run();
        std::vector<Score> scores = scoreDetection(kfilter_result, zones);

        sstm.str("");
        sstm << "sample" << i;
        kfilter_result.writeDAE("../../output/DAE_files/samples/", sstm.str());
        if(!scores.empty()) {
            results.push_back(scores[0].basketR == input[i].score);
        } else {
            results.push_back(!input[i].score);
        }
    }
    return results;
}


std::vector<bool> test_samples_Multiple_Step(std::vector<Sample_video> input, std::vector<Zone> zones, int step) {
    std::vector<bool> results;
    for (int i = 0; i < input.size(); ++i) {
        Video video(input[i].video);
        Video ms_result = multiple_step(video.getAll(), step);
        std::vector<Score> scores = scoreDetection(ms_result, zones);
        if(!scores.empty()) {
            results.push_back(scores[0].basketR == input[i].score);
        } else {
            results.push_back(!input[i].score);
        }
    }
    return results;
}


std::vector<bool> test_samples_Minimal_Distance(std::vector<Sample_video> input, std::vector<Zone> zones) {
    std::vector<bool> results;
    for (int i = 0; i < input.size(); ++i) {
        Video video(input[i].video);
        std::stringstream sstm;
        sstm << "sample" << i;
        Video md_result = minimal_distance(video.getAll());
        md_result.writeDAE("../../output/DAE_files/md_samples/", sstm.str());
        std::vector<Score> scores = scoreDetection(md_result, zones);
        if(!scores.empty()) {
            results.push_back(scores[0].basketR == input[i].score);
        } else {
            results.push_back(!input[i].score);
        }
    }
    return results;
}

void samples_analyze(std::vector<Sample_video> input, std::vector<Zone> zones) {
    std::vector<bool> results;
    std::vector<double> bests{0,0,0,0};
    int good(0), bad(0);

    for (double q_out = 0.001; q_out <= 0.001; q_out += 0.001) {
        for (double q_in = 1; q_in <= 1; q_in += 0.1) {
            for (double r_out = 0.1; r_out <= 0.3; r_out += 0.05) {
                for (double r_in = 0.2; r_in <= 0.5; r_in += 0.05) {

                    results = test_samples_Kalman_Filter(input, zones, std::vector<double>{q_out, q_in, r_out, r_in});
                    int r_good(0), r_bad(0);
                    for (int i = 0; i < results.size(); ++i) {
                        if(results[i]){
                            ++r_good;
                        } else {
                            ++r_bad;
                        }
                    }

                    if(r_good > good) {
                        bests = {q_out, q_in, r_out, r_in};
                        good = r_good;
                        bad = r_bad;
                    }
                }
            }
        }
    }

    std::cout << "Total : " << good << " / " << results.size() << std::endl;
    std::cout << "Coefficients : " << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << bests[i] << std::endl;
    }

}

void statistics(const std::vector<bool> &results, const std::vector<Sample_video> &samples) {
    int good_in(0), good_out(0);
    int bad_in(0), bad_out(0);
    std::cout << "Bons résultats :" << std::endl;
    for (int i = 0; i < results.size(); ++i) {
        if(results[i]) {
            std::cout << "sample" << i;
            if(samples[i].score) {
                ++good_in;
                std::cout << " in" << std::endl;
            } else {
                ++good_out;
                std::cout << " out" << std::endl;
            }
        }
    }
    std::cout << "Mauvais résultats :" << std::endl;
    for (int i = 0; i < results.size(); ++i) {
        if(!results[i]) {
            std::cout << "sample" << i;
            if(samples[i].score) {
                ++bad_in;
                std::cout << " in" << std::endl;
            } else {
                ++bad_out;
                std::cout << " out" << std::endl;
            }
        }
    }
    std::cout << "Total in :  " << good_in << " / " << (good_in + bad_in) << std::endl;
    std::cout << "Total out : " << good_out << " / " << (good_out + bad_out) << std::endl;
    std::cout << "Total :     " << (good_in + good_out) << " / " << results.size() << std::endl;
    std::cout << (good_in + good_out) << std::endl;
}

std::vector<Score> scoreDetection(const Video &video, std::vector<Zone> &zones) {
    int zones_state[4] = {0};
    std::vector<Score> scores;
    for (size_t i = 1; i < video.getVideo().size()-1; ++i) {
        Keyframe keyframe1(video.getAt(i));
        Keyframe keyframe2(video.getAt(i+1));

        if(zones.size() >= 4) {
            // if the ball gets inside the BasketBoxL from above
            if(zones[2].fromAbove(keyframe1, keyframe2)) {
                // the ball is inside the BaketBoxL
                zones_state[2] = 1;
            } else {
                // if the last state of the ball was in the basket
                if(zones_state[2] == 1) {
                    // if the ball exit the basket
                    if(!zones[2].isInside(keyframe2)) {
                        // if the ball didn't exit from above
                        if(!zones[2].fromAbove(keyframe2, keyframe1)) {
                            scores.push_back(Score{(int)i, 1, 0});
                        }
                        zones_state[2] = 0;
                    }
                }
            }

            if(zones[3].fromAbove(keyframe1, keyframe2)) {
                // the ball is inside the BaketBoxR
                zones_state[3] = 1;
            } else {
                // if the last state of the ball was in the basket
                if(zones_state[3] == 1) {
                    // if the ball exit the basket
                    if(!zones[3].isInside(keyframe2)) {
                        // if the ball didn't exit from above
                        if(!zones[3].fromAbove(keyframe2, keyframe1)) {
                            scores.push_back(Score{(int)i, 0, 1});
                            std::cout << keyframe1.getFrame() << std::endl;
                        }
                        zones_state[3] = 0;
                    }
                }
            }
        }
    }
    return scores;
}

Video minimal_distance(std::vector<std::vector<Keyframe>> videoAll) {
    std::vector<Keyframe> new_video;                            // construct the return Video
    new_video.push_back(videoAll[0][0]);                // take the videoAll[0][0]
    for(size_t i = 1; i < videoAll.size(); ++i){        // from 1(before) to size-1 (last element)
        double dist_min = std::numeric_limits<double>::infinity();  // initialize dist_min to infinity
        size_t pos = 0;                                             // initialize pos to 0
        if(videoAll[i].size() > 1) {
            for (size_t j = 0; j < videoAll.at(i).size(); ++j) {
                double dist_result = distance(new_video[i-1], videoAll[i][j]);
                if ( dist_result < dist_min) {
                    dist_min = dist_result;
                    pos = j;
                }
            }
        }
        new_video.push_back(videoAll[i][pos]);
    }
    return Video(new_video);
}

Video multiple_step(std::vector<std::vector<Keyframe>> videoAll, size_t n_steps) {
    std::vector <Keyframe> new_video;
    // initialize vector
    new_video.push_back(videoAll[0][0]);
    new_video.push_back(videoAll[1][0]);
    size_t i = 1;
    while(i < videoAll.size() - n_steps) {
        size_t index = 0;
        Keyframe* best = NULL;
        for (size_t j = 1; j <= n_steps; ++j) {
            double min_distance = std::numeric_limits<double>::max();
            Keyframe estim_keyframe = estimatedNextKeyframe(new_video[new_video.size()-2], new_video[new_video.size()-1], videoAll[i+j][0].getFrame());
            for (size_t k = 0; k < videoAll[i+j].size(); ++k) {
                double calc_distance = distance(estim_keyframe, videoAll[i+j][k]);
                if(calc_distance < min_distance) {
                    min_distance = calc_distance;
                    index = j; //know at which dist from i the keyframe is good
                    best = &(videoAll[i+j][k]);
                }
            }
        }
        new_video.push_back(*best);
        i += index;
    }
    return new_video;
}

void writes_samples_Kalman_Filter(std::vector<Sample_video> input, std::vector<Zone> zones,
                                               std::vector<double> coefficients) {
    std::vector<bool> results;
    std::vector<Keyframe> all= {Keyframe(0,0,0,0)};
    for (int i = 0; i < input.size(); ++i) {
        Video video(input[i].video);

        KFilter kfilter(1.0/30.0, video.getAll(), zones);
        if(coefficients.size() != 0) {
            kfilter.setQ(coefficients[0], coefficients[1]);
            kfilter.setR(coefficients[2], coefficients[3]);
        }
        Video kfilter_result = kfilter.run();
        all.push_back(Keyframe(all.end()->getFrame()+1, 0,0,0));
        all.push_back(Keyframe(kfilter_result.getVideo().begin()->getFrame()-1, 0,0,0));
        all.insert(all.end(), kfilter_result.getVideo().begin(), kfilter_result.getVideo().end());
        all.push_back(Keyframe(all.end()->getFrame(),0,0,0));
        std::vector<Score> scores = scoreDetection(kfilter_result, zones);
        kfilter_result;
        if(!scores.empty()) {
            results.push_back(scores[0].basketR == input[i].score);
        } else {
        results.push_back(!input[i].score);
        }
    }

    Video v_all(all);
    v_all.writeDAE("../../output/DAE_files/", "all_kalman");
}
