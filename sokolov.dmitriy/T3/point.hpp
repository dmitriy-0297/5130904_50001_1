#ifndef POINT_H
#define POINT_H

struct Point {
    int x, y;

    bool operator==(const Point &other) const;
};

#endif
