#include "Intersection.h"

Intersection::Intersection(int id, int x, int y, unsigned char type) :
		id(id), x(x), y(y), type(type) {
}

int Intersection::getID() const {
	return id;
}

int Intersection::getX() const{
	return x;
}

int Intersection::getY() const{
	return y;
}

bool Intersection::operator ==(const Intersection &i2) const {
	return (type == i2.type && id == i2.id);
}

std::ostream & operator <<(std::ostream &os, const Intersection &v) {
	os << v.getID();
	return os;
}
