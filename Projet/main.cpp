#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

#include "Keyframe.h"
#include "Video.h"
#include "Functions.h"
#include "Line.h"
#include "Parabola.h"

using namespace std;



int main() {


    Zone BoxL("../../input/zones/BigBox/BigBoxL.txt");
    Zone BoxR("../../input/zones/BigBox/BigBoxR.txt");
    Zone BasketBoxL("../../input/zones/BasketBox/BasketBoxL.txt");
    Zone BasketBoxR("../../input/zones/BasketBox/BasketBoxR.txt");

    vector<Zone> zones;
    zones.push_back(BoxL);
    zones.push_back(BoxR);
    zones.push_back(BasketBoxL);
    zones.push_back(BasketBoxR);

    vector<Sample_video> samples = create_samples_video("../../input/tracking/basket_detection.txt","../../input/tracking/separation.txt");

    vector<double> coefficients = {0.001, 1, 0.2, 0.35};
    vector<bool> results = test_samples_Kalman_Filter(samples, zones, coefficients);
    statistics(results, samples);

};










