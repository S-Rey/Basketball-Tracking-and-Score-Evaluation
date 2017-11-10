//
// Created by Samuel on 14.09.15.
//

#ifndef PROJET_VIDEO_H
#define PROJET_VIDEO_H


#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Keyframe.h"


class Video {
private:
	std::vector<Keyframe> m_video;
public:
	Video(std::string name);
	Video(std::vector<Keyframe>);
	std::vector<std::vector<Keyframe>> getAll();
	const Keyframe &getAt(size_t i) const;
	const std::vector<Keyframe> &getVideo() const;
	std::vector<Keyframe> getFrame(int frame) const;
	const std::vector<Keyframe> fromTo(int from, int to);
	std::vector<std::vector<Keyframe>> getRepet();
	void writeWithoutRepet(std::string name);
	void write(std::string name);
	void writeGS(std::string name);
	Video separateDist(double dist);

	/**
	 * write an OBJ file
	 *
	 * @param folder_name
	 * Where the file will be store
	 *
	 * @param obj_name
	 * Name of the file (will be also the name of OBJ)
	 *
	 * @param linked
	 * Choose if the points will be linked
	 */
	void writeOBJ(std::string folder_name, std::string obj_name, bool linked);

	void writeDAE(std::string folder_name, std::string dae_name);

};

std::ostream& operator<<(std::ostream &strm, const Video &video);

#endif //PROJET_VIDEO_H

