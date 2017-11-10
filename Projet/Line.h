//
// Created by Samuel on 02.12.15.
//


#ifndef PROJET_LINE_H
#define PROJET_LINE_H

#import"Video.h"

class Line {
private:
	std::vector<Keyframe> m_video;
public:
	Line(Keyframe start, Keyframe end);
	const std::vector<Keyframe> &getVideo() const;
	std::vector<Keyframe> getOnly(int step) const;
	void randomize(double variation);
};


#endif //PROJET_LINE_H
