//
// Created by Samuel on 14.10.15.
//

#include "Vertex.h"

void Vertex::setM_z(double m_z) {
	Vertex::m_z = m_z;
}

double Vertex::getZ() const {
	return m_z;
}

void Vertex::setM_y(double m_y) {
	Vertex::m_y = m_y;
}

double Vertex::getY() const {
	return m_y;
}

void Vertex::setM_x(double m_x) {
	Vertex::m_x = m_x;
}

double Vertex::getX() const {
	return m_x;
}

Vertex::Vertex(double x, double y, double z) : m_x(x), m_y(y), m_z(z) { }