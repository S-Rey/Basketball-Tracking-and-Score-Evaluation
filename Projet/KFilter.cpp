//
// Created by Samuel on 19.11.15.
//

#include "KFilter.h"

KFilter::KFilter (double dt, std::vector<std::vector<Keyframe>> video_all, std::vector<Zone> zones) {

    this->dt = dt;
    this->zones = zones;
    for (int i = 0; i < zones.size(); ++i) {
        zones_state.push_back(0);
    }
    input = std::vector<std::vector<Keyframe>>(video_all);

    walls[0][0] = -2.014711;
    walls[0][1] = 11.034052;
    walls[1][0] = -3.355253;
    walls[1][1] = 21.284559;

    // default value can be modified by setter
    q_out = 0.001;
    q_in = 1;
    r_out = 0.2;
    r_in = 0.35;
    height = 2.0;


    F = cv::Mat::eye(6, 6, cv::DataType<double>::type);
    F.at<double>(0,3) = dt;
    F.at<double>(1,4) = dt;
    F.at<double>(2,5) = dt;

    B = cv::Mat::zeros(6, 3, cv::DataType<double>::type);
    B.at<double>(2,2) = std::pow(dt,2)/2.0;
    B.at<double>(5,2) = dt;

    // gravitationnal constant
    u = cv::Mat::zeros(3, 1, cv::DataType<double>::type);
    u.at<double>(2,0) = -9.7803278;

    H = cv::Mat::eye(6, 6, cv::DataType<double>::type);

    y = cv::Mat::zeros(6, 1, cv::DataType<double>::type);
    S = cv::Mat::zeros(6, 6, cv::DataType<double>::type);

    // Ball mat
    z = cv::Mat::zeros(6, 1, cv::DataType<double>::type);
    x = cv::Mat::zeros(6, 1, cv::DataType<double>::type);


    K = cv::Mat::zeros(6, 6, cv::DataType<double>::type);
    K_in = K.clone();

    double dt4 = std::pow(dt,4)/4;
    double dt3 = std::pow(dt,3)/4;
    double dt2 = std::pow(dt,2);

    P = cv::Mat::zeros(6, 6, cv::DataType<double>::type);
    P.at<double>(0,0) = dt4;
    P.at<double>(1,1) = dt4;
    P.at<double>(2,2) = dt4;

    P.at<double>(3,0) = dt3;
    P.at<double>(4,1) = dt3;
    P.at<double>(5,2) = dt3;

    P.at<double>(0,3) = dt3;
    P.at<double>(1,4) = dt3;
    P.at<double>(2,5) = dt3;

    P.at<double>(3,3) = dt2;
    P.at<double>(4,4) = dt2;
    P.at<double>(5,5) = dt2;

    P_in = P.clone();

}

void KFilter::setQ(double q_out, double q_in) {
    this->q_out = q_out;
    this->q_in = q_in;
}

void KFilter::setR(double r_out, double r_in) {
    this->r_out = r_out;
    this->r_in = r_in;
}

void KFilter::test() {
    cv::Mat test;
    test = 0.0001 * cv::Mat::eye(6, 6, cv::DataType<double>::type);
    test.at<double>(3,3) = 0.0;
    test.at<double>(4,4) = 0.0;
    test.at<double>(5,5) = 0.0;

    KFilter::matPrint("F ", F, 2); std::cout << std::endl;
    KFilter::matPrint("B ", F, 2); std::cout << std::endl;
    KFilter::matPrint("x ", x, 2); std::cout << std::endl;
    KFilter::matPrint("K ", K ,10); std::cout << std::endl;

    KFilter::matPrint("H ", H, 2); std::cout << std::endl;
    KFilter::matPrint("u ", u, 2); std::cout << std::endl;
    KFilter::matPrint("P ", P, 10); std::cout << std::endl;
}

Video KFilter::run() {
    std::vector<Keyframe> output;
    output.push_back(input[0][0]);

    Q = q_out * cv::Mat::eye(6, 6, cv::DataType<double>::type);
    Q_in = q_in * cv::Mat::eye(6, 6, cv::DataType<double>::type);
    R = r_out * cv::Mat::eye(6, 6, cv::DataType<double>::type);
    R_in = r_in * cv::Mat::eye(6, 6, cv::DataType<double>::type);
    bool inside_box(false);         // if the ball is inside the box

    double & px = z.at<double>(0,0);
    double & py = z.at<double>(1,0);
    double & pz = z.at<double>(2,0);


    // initial position
    x.at<double>(0, 0) = input[0][0].getX();
    x.at<double>(1, 0) = input[0][0].getY();
    x.at<double>(2, 0) = input[0][0].getZ();
    // initial velocity

    double vx = ((input[0][0].getX() -input[1][0].getX()) / (input[0][0].getFrame() - input[1][0].getFrame())) / dt;
    double vy = ((input[0][0].getY() -input[1][0].getY()) / (input[0][0].getFrame() - input[1][0].getFrame())) / dt;
    double vz = ((input[0][0].getZ() -input[1][0].getZ()) / (input[0][0].getFrame() - input[1][0].getFrame())) / dt;
    x.at<double>(3, 0) = vx;
    x.at<double>(4, 0) = vy;
    x.at<double>(5, 0) = vz;


    size_t key_measure(1); // as the first measure was added to the output
    size_t key_input(1);   // as the first measure was added to the output
    for(size_t t = input[0][0].getFrame(); t < input[input.size()-1][0].getFrame(); ++t) {

        // Calculate the ball estimate
        x = F.clone() * x.clone() +  B.clone() * u.clone();

        // Next covariance
        P = F.clone() * P.clone() * F.clone().t() + Q.clone();
        P_in = F.clone() * P_in.clone() * F.clone().t() + Q_in.clone();

        z = x.clone(); // take the ball_estimate if no measures. And if there is some measures just change the location

        // if there is a measure for the frame t
        if(input[key_measure][0].getFrame() == t) {

            int best(0);
            // Select the best z candidate at the frame t if multiple measurements
            if(input[key_measure].size() >= 2) {
                double dist(0);
                double min_dist = std::numeric_limits<double>::max();
                Keyframe estimate(t, x.at<double>(1, 0), x.at<double>(2, 0), x.at<double>(3, 0));
                for (int i = 0; i < input[key_measure].size(); ++i) {
                    dist = estimate.dist(input[key_measure][i]);
                    if (dist < min_dist) {
                        best = i;
                        min_dist = dist;
                    }
                }
            }

            // copy the measure to the measure vector
            z.at<double>(0, 0) = input[key_measure][best].getX();
            z.at<double>(1, 0) = input[key_measure][best].getY();
            z.at<double>(2, 0) = input[key_measure][best].getZ();


            // compute the speed between the previous keyframe and the actual measure
            double diff_time = dt * (input[key_measure][best].getFrame() - output[key_measure - 1].getFrame());
            double diff_x = input[key_measure][best].getX() - output[key_measure - 1].getX();
            double diff_y = input[key_measure][best].getY() - output[key_measure - 1].getY();
            double diff_z = input[key_measure][best].getZ() - output[key_measure - 1].getZ();
            double v_x = diff_x / diff_time;
            double v_y = diff_y / diff_time;
            double v_z = diff_z / diff_time;

            z.at<double>(3, 0) = v_x;
            z.at<double>(4, 0) = v_y;
            z.at<double>(5, 0) = v_z;

            // check if the measurement is in the box
            if(!zones.empty()) {
                zones_state[0] = zones[0].isInside(Keyframe(z));
                zones_state[1] = zones[1].isInside(Keyframe(z));
                // if the measure of the ball is in one or another box
                if(zones_state[0] || zones_state[1]) {
                    inside_box = true;
                } else {
                    inside_box = false;
                }
            }

            ++key_measure;


        }

        // Innovation
        y = z - x;

        // Innovation covariance
        S = H * P * H.t() + R;
        S_in = H * P_in * H.t() + R_in;

        // Kalman Gain
        K = P * H.t() * S.inv();
        K_in = P_in * H.t() * S_in.inv();

        // Update state estimate
        if(inside_box) {
            x = x + K_in * y;
        } else {
            x = x + K * y;
        }


        // Check if any collision to known surfaces
        // floor
        if(x.at<double>(2,0) < 0){
            x.at<double>(2,0) *= -1;
            x.at<double>(5,0) *= -0.82;
        }

        // walls
        if((x.at<double>(0,0) < walls[0][0]) || (x.at<double>(0,0) > walls[0][1])) {
            x.at<double>(3,0) *= -0.82;
        }
        if((x.at<double>(1,0) < walls[1][0]) || (x.at<double>(1,0) > walls[1][1])) {
            x.at<double>(4,0) *= -0,82;
        }

        // Update estimate covariance
        P = (cv::Mat::eye(6, 6, cv::DataType<double>::type) - K * H) * P;
        P_in = (cv::Mat::eye(6, 6, cv::DataType<double>::type) - K_in * H) * P_in;


        Keyframe ball_result(t, x.at<double>(0, 0), x.at<double>(1, 0), x.at<double>(2, 0));
        output.push_back(ball_result);
        ++key_input;

    }
    return output;
}


void KFilter::matPrint(std::string name, cv::Mat mat, int prec) {
    std::cout << name << std::endl;
    for(int i=0; i<mat.size().height; i++)
    {
        std::cout << "|";
        for(int j=0; j<mat.size().width; j++)
        {
            std::cout << "\t" << std::setiosflags(std::ios::fixed) << std::setprecision(prec) << mat.at<double>(i,j);
            if(j != mat.size().width-1)
                std::cout << "";
            else
                std::cout << " |" << std::endl;
        }
    }
}

KFilter::KFilter(double dt, Video video, std::vector<Zone> zones) {
    std::vector<Keyframe> new_video = video.getVideo();
    std::vector<std::vector<Keyframe>> new_video_all;

    for (int i = 0; i < new_video.size(); ++i) {
        new_video_all.push_back(std::vector<Keyframe>());
        new_video_all[i].push_back(new_video[i]);
    }

    KFilter(dt, new_video_all, zones);
}

const std::vector<KFilter::Score> &KFilter::getScores() {
    return scores;
}

