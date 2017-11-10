//
// Created by Samuel on 22.12.15.
//

#ifndef PROJET_PARABOL_H
#define PROJET_PARABOL_H

#import"Keyframe.h"


class Parabola {
private:
	std::vector<Keyframe> m_video;
public:
	Parabola();
	const std::vector<Keyframe> &getVideo() const;
	std::vector<Keyframe> getOnly(int step) const;
	void randomize(double variation);
};


#endif //PROJET_PARABOL_H
