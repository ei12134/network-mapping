#ifndef Intersection_H_
#define Intersection_H_

#include <iostream>

static const unsigned char INTERSECTION = 0;
static const unsigned char HOME = 1;
static const unsigned char CENTRAL = 2;

class Intersection {
    int id;
    int x, y;
    unsigned char type;
public:
    Intersection(int id, int x, int y, unsigned char type);
    int getId() const;
    int getX() const;
    int getY() const;
    unsigned char getType() const;
    bool operator ==(const Intersection &v2) const;
    friend std::ostream & operator <<(std::ostream &os, const Intersection &v);
};

#endif
