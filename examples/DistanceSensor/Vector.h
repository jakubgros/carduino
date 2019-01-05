#ifndef Vector_h
#define Vector_h

struct Vector
{
    Vector(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    int x = 0;
    int y = 0;
};

#endif