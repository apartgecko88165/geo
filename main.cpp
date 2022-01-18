// C libraries
#include <raylib.h>
#include <stdlib.h>

// C++ libraries
#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <complex>

// RayGUI implementation
#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

typedef std::complex<double> point;

#define dx real()
#define dy imag()
  
#define PI 3.1415926535897932384626
  
point rotate(point P, point Q, double theta)
{
    return (P-Q) * std::polar(1.0, theta) +  Q;
}

Vector2 rotateAbout(Vector2 a, Vector2 b, double thetaDeterm)
{
    point P((double)a.x, (double)a.y);
    point Q((double)b.x, (double)b.y);

    double theta = PI/thetaDeterm;
      
    point result = rotate(P, Q, theta);

    return Vector2{ (float)result.dx, (float)result.dy };
}

Vector2 reflectAbout(Vector2 a, Vector2 line[2])
{
    double slope = (line[0].y - line[1].y) / (line[0].x - line[1].x);
    double rSlope = -slope;

    double mpY = slope * (line[0].x - line[1].x) + line[1].y;
    
}

int main()
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "*NOT* Geogebra");

    SetTargetFPS(60);

    std::vector<Vector2> points;
    std::vector<std::array<Vector2, 2>> lines;

    Vector2 lastPoint = { -1, -1 };
    Vector2 lastRotationSelection = { -1, -1 };
    Vector2 lastReflectionSelection = { -1, -1 };

    bool gridShown = true;
    bool dropdownEditMode = false;
    bool rotationEditMode = false;

    int rmbMode = 0;
    int rotationDegrees = 90;

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            if (gridShown)
            {
                for (int i = 0; i < 90; i++)
                {
                    DrawLine(i*20, 0, i*20, screenHeight, GRAY);
                }
                for (int i = 0; i < 45; i++)
                {
                    DrawLine(0, i*20, screenWidth, i*20, GRAY);
                }
            }

            if (IsMouseButtonPressed(0))
            {
                if (!CheckCollisionPointRec(GetMousePosition(), Rectangle{ screenWidth - 450, 50, 400, 40 }) && !dropdownEditMode)
                {
                    Vector2 point = GetMousePosition();
                    point.x -= (int)point.x % 20;
                    point.y -= (int)point.y % 20;

                    points.push_back(point);
                } 
            }
            if (IsMouseButtonPressed(1))
            {
                if (!CheckCollisionPointRec(GetMousePosition(), Rectangle{ screenWidth - 450, 50, 400, 40 }) && !dropdownEditMode)
                {
                    if (rmbMode == 0)
                    {
                        if (lastPoint.x < 0 || lastPoint.y < 0)
                        {
                            lastPoint = Vector2{ GetMousePosition().x - (int)GetMousePosition().x % 20, GetMousePosition().y - (int)GetMousePosition().y % 20 };
                        } else
                        {
                            lines.push_back({ lastPoint, Vector2{ GetMousePosition().x - (int)GetMousePosition().x % 20, GetMousePosition().y - (int)GetMousePosition().y % 20 } });
                            lastPoint = Vector2{ -1, -1 };
                        }
                    } else if (rmbMode == 1)
                    {
                        for (Vector2 p : points)
                        {
                            if (lastRotationSelection.x < 0 || lastRotationSelection.y < 0)
                            {
                                if (CheckCollisionPointCircle(GetMousePosition(), p, 7)) lastRotationSelection = p;
                            } else
                            {
                                if (CheckCollisionCircles(GetMousePosition(), 25, p, 7))
                                {
                                    points.push_back(rotateAbout(lastRotationSelection, p, (double)180/rotationDegrees));
                                    lastRotationSelection = Vector2{ -1, -1 };
                                }
                            }
                        }
                    } else if (rmbMode == 2)
                    {
                        if (lastReflectionSelection.x < 0 || lastReflectionSelection.y < 0)
                        {

                            for (Vector2 p : points)
                            {
                                if (CheckCollisionPointCircle(GetMousePosition(), p, 7)) lastReflectionSelection = p;
                            }

                            for (auto l : lines)
                            {
                                if (CheckCollisionPointLine(GetMousePosition(), l[0], l[1], 8)) {

                                }
                            }
                        }
                    }
                }
            }

            for (Vector2 p : points)
            {
                DrawCircleV(p, 7, BLACK);
            }
            for (auto l : lines)
            {
                DrawLineEx(l[0], l[1], 5, BLUE);
            }

            if (lastPoint.x >= 0 && lastPoint.y >= 0) DrawCircleV(lastPoint, 7, BLUE);
            if (lastRotationSelection.x >= 0 && lastRotationSelection.y >= 0) DrawCircleV(lastRotationSelection, 7, ORANGE);
            if (lastReflectionSelection.x >= 0 && lastReflectionSelection.y >= 0) DrawCircleV(lastReflectionSelection, 7, RED);

            if (GuiButton(Rectangle{ screenWidth - 150, 50, 100, 40 }, "CLEAR"))
            {
                points.clear();
                lines.clear();
            }

            if (GuiButton(Rectangle{ screenWidth - 250, 50, 100, 40 }, gridShown ? "Hide Grid" : "Show Grid"))
            {
                gridShown = !gridShown;
            }

            if (GuiDropdownBox(Rectangle{ screenWidth - 350, 50, 100, 40 }, "Lines;Rotate;Reflect", &rmbMode, dropdownEditMode)) dropdownEditMode = !dropdownEditMode;
            if (rmbMode == 1) {
                DrawRectangle(screenWidth - 450, 50, 100, 40, WHITE);
                DrawRectangle(screenWidth - 500, 60, 50, 18, WHITE);
                if (GuiValueBox(Rectangle{ screenWidth - 450, 50, 100, 40 }, "Degrees", &rotationDegrees, 0, 360, rotationEditMode)) rotationEditMode = !rotationEditMode;
            }

        EndDrawing();
    }
}