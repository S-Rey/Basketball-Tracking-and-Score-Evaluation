//
// Created by Samuel on 25.11.15.
//

#ifndef PROJET_ZONE_H
#define PROJET_ZONE_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <tuple>

#import"Vertex.h"
#include "Keyframe.h"


class Zone {
private:
	std::vector<Vertex> vertices;
	double area[3][2];
public:
	Zone(std::string name);
	int isInside(const Keyframe &keyframe);
	int fromAbove(Keyframe k1, Keyframe k2);
	std::string toString();
};


#endif //PROJET_ZONE_H
