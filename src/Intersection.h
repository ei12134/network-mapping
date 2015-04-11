#ifndef Intersection_H_
#define Intersection_H_

#include <iostream>

const unsigned char INTERSECTION = 0;
const unsigned char HOME = 1;
const unsigned char CENTRAL = 2;

class Intersection {
	int id;
	int x, y;
	const unsigned char type;
public:
	Intersection(int id,int x,int y, unsigned char type);
	int getID() const;
	int getX() const;
	int getY() const;

	bool operator ==(const Intersection &v2) const;
	friend std::ostream & operator <<(std::ostream &os, const Intersection &v);
};

#endif
