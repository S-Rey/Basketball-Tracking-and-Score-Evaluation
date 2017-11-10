//
// Created by Samuel on 14.10.15.
//

#ifndef PROJET_VERTEX_H
#define PROJET_VERTEX_H


class Vertex {
private:
	double m_x;
	double m_y;
	double m_z;
public:
	Vertex(double x, double y, double z);

	double getX() const;

	void setM_x(double m_x);

	double getY() const;

	void setM_y(double m_y);

	double getZ() const;

	void setM_z(double m_z);


};


#endif //PROJET_VERTEX_H
