/* Copyright (c) 2014 Krzysztof Pachulski
 * License: The MIT License (MIT)
 * MIT License page: http://opensource.org/licenses/MIT
 */

#include <iostream>
#include <ctime>

#include "Drawing.h"
#include "Delunay.h"

int main()
{
    system("Title Delunay");
    hConWnd = GetConsoleWndHandle();
    if (hConWnd)
    {
        const int pointsCount = 250;

        srand(time(NULL));

        // It can crash if there are two or more points with the same coordinates
        std::vector<Triangulation::Point> points;
        for (int i = 0; i < pointsCount; ++i)
            points.push_back({ rand() % 600 + 20, rand() % 600 + 20 });

        auto triangles = Triangulation::Delunay(points).triangulate();

        std::cout << "triangles: " << triangles.size() << std::endl;

        for (auto& triangle : triangles)
        {
            BCX_Line(hConWnd, triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, Blue);
            BCX_Line(hConWnd, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, Blue);
            BCX_Line(hConWnd, triangle[2].x, triangle[2].y, triangle[0].x, triangle[0].y, Blue);

            // For debugging
            auto circle = triangle.getCircumCircle();
            BCX_Circle(hConWnd, circle.center.x, circle.center.y, circle.radius, Red);
        }

        for (int i = 0; i < points.size(); ++i)
            BCX_Circle(hConWnd, points[i].x, points[i].y, 1, White, 1);

        getchar();  // wait
    }
    return 0;
}
