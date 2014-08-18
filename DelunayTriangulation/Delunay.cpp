/* Copyright (c) 2014 Krzysztof Pachulski
* License: The MIT License (MIT)
* MIT License page: http://opensource.org/licenses/MIT
*/

#include "Delunay.h"

#include <algorithm>

namespace Triangulation
{

    int Utils::getFirstBelowTop(std::stack<int>& stack)
    {
        auto top = stack.top();
        stack.pop();
        auto result = stack.top();
        stack.push(top);
        return result;
    }

    std::vector<Point> Delunay::getConvexHull()
    {
        std::vector<Point> convexHull;

        int min = 0;
        for (int i = 0; i < this->points.size(); ++i)
        {
            if (this->points[i].y < this->points[min].y)
                min = i;
        }

        std::swap(this->points[min], this->points[0]);

        std::sort(this->points.begin() + 1, this->points.end(), [&](const Point p1, const Point p2)
        {
            return (this->points[0] - p1).getAngle() >(this->points[0] - p2).getAngle();
        });

        std::stack<int> stack;
        int next = 2;
        stack.push(0);
        stack.push(1);

        do
        {
            Edge edge;
            edge.start = this->points[Utils::getFirstBelowTop(stack)];
            edge.end = this->points[stack.top()];

            while (!edge.isPointOnTheLeft(this->points[next]))
            {
                stack.pop();
                edge.start = this->points[Utils::getFirstBelowTop(stack)];
                edge.end = this->points[stack.top()];
            }

            stack.push(next);
            ++next;

        } while (next < this->points.size());
        stack.push(0);

        while (!stack.empty())
        {
            convexHull.push_back(this->points[stack.top()]);
            stack.pop();
        }

        return convexHull;
    }

    void Delunay::initialTriangulation()
    {
        auto convexHull = this->getConvexHull();

        for (int i = 2; i < convexHull.size(); ++i)
        {
            this->points.erase(std::remove(this->points.begin(), this->points.end(), convexHull[0]), this->points.end());
            this->points.erase(std::remove(this->points.begin(), this->points.end(), convexHull[i - 1]), this->points.end());
            this->points.erase(std::remove(this->points.begin(), this->points.end(), convexHull[i]), this->points.end());
            this->triangles.push_back({ { convexHull[0], convexHull[i - 1], convexHull[i] } });
        }
    }

    std::vector<Triangle> Delunay::triangulate()
    {
        if (this->points.size() < 3)
            return std::vector<Triangle>();

        this->initialTriangulation();

        for (int i = this->points.size() - 1; i >= 0; --i) {
            this->addVertex(this->points[i]);
        }

        return this->triangles;
    }

    void Delunay::addVertex(Point vertex)
    {
        std::vector<Edge> edgeBuffer;
        for (int i = this->triangles.size() - 1; i >= 0; --i)
        {
            if (this->triangles[i].getCircumCircle().isPointInside(vertex))
            {
                Edge e1{ this->triangles[i][0], this->triangles[i][1] };
                Edge e2{ this->triangles[i][1], this->triangles[i][2] };
                Edge e3{ this->triangles[i][2], this->triangles[i][0] };

                if (std::find(edgeBuffer.begin(), edgeBuffer.end(), e1) == edgeBuffer.end())
                    edgeBuffer.push_back(e1);
                else
                    edgeBuffer.erase(std::remove(edgeBuffer.begin(), edgeBuffer.end(), e1), edgeBuffer.end());
                if (std::find(edgeBuffer.begin(), edgeBuffer.end(), e2) == edgeBuffer.end())
                    edgeBuffer.push_back(e2);
                else
                    edgeBuffer.erase(std::remove(edgeBuffer.begin(), edgeBuffer.end(), e2), edgeBuffer.end());
                if (std::find(edgeBuffer.begin(), edgeBuffer.end(), e3) == edgeBuffer.end())
                    edgeBuffer.push_back(e3);
                else
                    edgeBuffer.erase(std::remove(edgeBuffer.begin(), edgeBuffer.end(), e3), edgeBuffer.end());

                this->triangles.erase(this->triangles.begin() + i);
            }
        }

        for (auto& edge : edgeBuffer)
        {
            this->triangles.push_back({ { edge.start, edge.end, vertex } });
        }
    }

}