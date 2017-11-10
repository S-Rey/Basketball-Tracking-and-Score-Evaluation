//
// Created by Samuel on 25.11.15.
//


#include "Zone.h"

Zone::Zone(std::string name) {
	std::string line;
	std::ifstream input(name, std::ios::in);

	if(input.is_open()) {
		while(getline(input, line)) {
			std::istringstream iss(line);
			char letter;
			double x;
			double y;
			double z;
			// the format is v x z y
			if(!(iss >> letter >> x >> z >> y)) {
				break;
			};
			vertices.push_back(Vertex(x, y, z));
		}
		std::cout << "Zone Importation Done" << std::endl;
	} else {
		std::cerr << "/!\\ Zone Importation Error! /!\\ " << std::endl;
	}
	input.close();

	// (min, max)
	area[0][0] = vertices[0].getX(); area[0][1] = vertices[0].getX();
	area[1][0] = vertices[0].getY(); area[1][1] = vertices[0].getY();
	area[2][0] = vertices[0].getZ(); area[2][1] = vertices[0].getZ();

	// find which coordinates are the maximum and minimum
	for(size_t i = 1; i < vertices.size(); ++i) {
		(area[0][0] < vertices[i].getX()) ? area[0][0] = vertices[i].getX() : 0;
		(area[0][1] > vertices[i].getX()) ? area[0][1] = vertices[i].getX() : 0;
		(area[1][0] < vertices[i].getY()) ? area[1][0] = vertices[i].getY() : 0;
		(area[1][1] > vertices[i].getY()) ? area[1][1] = vertices[i].getY() : 0;
		(area[2][0] < vertices[i].getZ()) ? area[2][0] = vertices[i].getZ() : 0;
		(area[2][1] > vertices[i].getZ()) ? area[2][1] = vertices[i].getZ() : 0;
	}
}


int Zone::isInside(const Keyframe &keyframe) {
	if(area[0][0] >= keyframe.getX() &&
	   area[0][1] <= keyframe.getX() &&
	   area[1][0] >= keyframe.getY() &&
	   area[1][1] <= keyframe.getY() &&
	   area[2][0] >= keyframe.getZ() &&
	   area[2][1] <= keyframe.getZ()
  			) return 1;
	return 0;
}



std::string Zone::toString(){
	return ("x: [" + std::to_string(area[0][0]) + ", " + std::to_string(area[0][1]) + "]\n" +
			"y: [" + std::to_string(area[1][0]) + ", " + std::to_string(area[1][1]) + "]\n" +
			"z: [" + std::to_string(area[2][0]) + ", " + std::to_string(area[2][1]) + "]\n"
	);
}

// return 1 id true and 0 if false
int Zone::fromAbove(Keyframe k1, Keyframe k2) {
	// if(k1 outside and k2 inside and k1 above the box)
	if((!this->isInside(k1) && (this->isInside(k2) && (k1.getZ() >= area[2][0])))){
		return 1;
	}
	return 0;
}
