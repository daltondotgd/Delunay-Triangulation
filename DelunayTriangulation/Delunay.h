/* Copyright (c) 2014 Krzysztof Pachulski
* License: The MIT License (MIT)
* MIT License page: http://opensource.org/licenses/MIT
*/

#ifndef __DELUNAY_H__
#define __DELUNAY_H__

#include <vector>
#include <stack>

namespace Triangulation
{

    const float EPSILON = 1.192092896e-07F;

    struct Point
    {
        float x;
        float y;

        float getAngle() const
        {
            return atan2(this->y, this->x);
        }

        bool operator==(Point other) const
        {
            return (*this - other).magnitude() <= EPSILON;
        }

        bool operator<(Point other) const
        {
            return this->x < other.x && this->y < other.y;
        }

        bool operator!=(Point other) const
        {
            return (*this - other).magnitude() > EPSILON;
        }

        enum Orientation { COLINEAR, CW, CCW };
        static Orientation orientation(Point p, Point q, Point r)
        {
            int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

            if (val == 0) return Orientation::COLINEAR;
            return (val > 0) ? Orientation::CW : Orientation::CCW;
        }

        Point operator+(Point other) const
        {
            Point point;
            point.x = this->x + other.x;
            point.y = this->y + other.y;
            return point;
        }

        Point operator-(Point other) const
        {
            Point point;
            point.x = this->x - other.x;
            point.y = this->y - other.y;
            return point;
        }

        float magnitude()
        {
            return sqrtf(powf(this->x, 2) + powf(this->y, 2));
        }
    };

    struct Edge
    {
        Point start;
        Point end;

        bool operator==(Edge other) const
        {
            return (this->start == other.start && this->end == other.end)
                || (this->start == other.end && this->end == other.start);
        }

        bool operator<(Edge other) const
        {
            return (this->start < other.start && this->end < other.end);
        }

        bool isPointOnTheLeft(Point point)
        {
            int val = (start.y - end.y) * (point.x - end.x) - (start.x - end.x) * (point.y - end.y);

            if (val == 0) return false;
            return (val > 0) ? false : true;
        }

    };

    struct Circle
    {
        Point center;
        float radius;

        bool isPointInside(Point point)
        {
            if ((this->center - point).magnitude() <= this->radius)
                return true;
            else
                return false;
        }
    };

    struct Triangle
    {
        Point vertices[3];

        bool containsPoint(Point point)
        {
            for (int i = 0; i < 3; ++i)
            {
                if (this->vertices[i] == point)
                    return true;
            }
            return false;
        }

        Point operator[](int i)
        {
            return this->vertices[i];
        }

        Circle getCircumCircle()
        {
            Circle circumCircle;
            auto a = this->vertices[0];
            auto b = this->vertices[1];
            auto c = this->vertices[2];
            auto d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
            circumCircle.center.x = ((powf(a.x, 2) + powf(a.y, 2)) * (b.y - c.y) + (powf(b.x, 2) + powf(b.y, 2)) * (c.y - a.y) + (powf(c.x, 2) + powf(c.y, 2)) * (a.y - b.y)) / d;
            circumCircle.center.y = ((powf(a.x, 2) + powf(a.y, 2)) * (c.x - b.x) + (powf(b.x, 2) + powf(b.y, 2)) * (a.x - c.x) + (powf(c.x, 2) + powf(c.y, 2)) * (b.x - a.x)) / d;
            circumCircle.radius = (circumCircle.center - a).magnitude();
            return circumCircle;
        }
    };

    class Utils
    {
    public:
        static int getFirstBelowTop(std::stack<int>& stack);
    };

    class Delunay
    {
    public:
        Delunay(std::vector<Point> points) : points(points) {};

        std::vector<Triangle> triangulate();
    private:
        std::vector<Point> getConvexHull();
        void initialTriangulation();
        void addVertex(Point vertex);

        std::vector<Point> points;
        std::vector<Triangle> triangles;

    };

}

#endif // __DELUNAY_H__