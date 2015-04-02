/*
 * Intersection.cpp
 */

#include "Intersection.h"

Intersection::Intersection(unsigned int id, unsigned char type): id(id), type(type) {}

unsigned int Intersection::getID() const {
	return id;
}

bool Intersection::operator == (const Intersection &i2) const {
	return (type==i2.type && id==i2.id);
}
/*
ostream & operator << (ostream &os, Intersection &v) {
	os << v.getID();
	return os;
}*/