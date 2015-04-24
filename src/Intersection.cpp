#include "Intersection.h"

Intersection::Intersection(int id, int x, int y, int type) :
		id(id), x(x), y(y), type(type) {
}

int Intersection::getId() const {
	return id;
}

int Intersection::getX() const {
	return x;
}

int Intersection::getY() const {
	return y;
}

int Intersection::getType() const {
	return type;
}

void Intersection::setType(int type){
	this->type = type;
}

double Intersection::distance(Intersection &i) {
	return (sqrt(pow(x - i.x, 2) + pow(y - i.y, 2)));
}

bool Intersection::operator ==(const Intersection &i2) const {
	return (type == i2.type && id == i2.id);
}

std::ostream & operator <<(std::ostream &os, const Intersection &v) {
	os << v.getId();
	return os;
}
