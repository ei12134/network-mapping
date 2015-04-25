#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include <iostream>
#include <math.h>

static const int INTERSECTION = 0;
static const int HOUSE = 1;
static const int CENTRAL = 2;

class Intersection {
    int id;
    int x;
	int y;
    int type;
public:
    Intersection(int id, int x, int y, int type);
    int getId() const;
    int getX() const;
    int getY() const;
    int getType() const;
	void setType(int type);
    double distance(Intersection &i);
    bool operator ==(const Intersection &v2) const;
    friend std::ostream & operator <<(std::ostream &os, const Intersection &v);
};

#endif // INTERSECTION_H_
