#ifndef Intersection_H_
#define Intersection_H_

#include <iostream>

const unsigned char INTERSECTION = 0;
const unsigned char HOME = 1;
const unsigned char CENTRAL = 2;

class Intersection {
	unsigned int id;
	const unsigned char type;
public:
	Intersection(unsigned int id, unsigned char type);
	unsigned int getID() const;
	bool operator ==(const Intersection &v2) const;
	friend std::ostream & operator <<(std::ostream &os, const Intersection &v);
};

#endif
