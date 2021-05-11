#include <assert.h>
#include "ShapeHandler.hpp"
#include "../olcPixelGameEngine.h"

ShapeHandler* ShapeHandler::m_s_Instance = nullptr;

ShapeHandler::ShapeHandler()
{
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;

    m_Circles.reserve(999);
    m_Rectangles.reserve(9999999);
}

ShapeHandler::~ShapeHandler()
{
    m_Circles.clear();
    m_Rectangles.clear();
}


Circle* ShapeHandler::CreateCircle()
{
    auto& cv = m_s_Instance->m_Circles;
    cv.push_back(Circle());

    return &cv[cv.size() - 1];
}

Rectangle* ShapeHandler::CreateRectangle()
{
    auto& rv = m_s_Instance->m_Rectangles;
    rv.push_back(Rectangle());

    return &rv[rv.size() - 1];
}

void ShapeHandler::SetWorldBounds(olc::vf2d tl, olc::vf2d br)
{
    m_s_Instance->m_WorldTopLeft = tl;
    m_s_Instance->m_WorldBottomRight = br;
}

void ShapeHandler::Update(float fElapsedTime)
{
    for (auto& object : m_s_Instance->m_Circles)
    {
        olc::vf2d vPotentialPosition = object.pos + object.vel * fElapsedTime;
        
        // ------------ Circle Collisions ------------ //

        for (auto& circle : m_s_Instance->m_Circles)
        {
            if (object != circle) // Make sure object and circle arent the same "Circle"
            {
                olc::vf2d vecDistance = (object.pos - circle.pos);
                float distance = vecDistance.mag2();
                float sumOfRadii = (object.radius + circle.radius);
                if (distance < sumOfRadii * sumOfRadii)
                {
                    distance = sqrt(distance);
                    olc::vf2d collision_normal = vecDistance.norm();
                    float distanceToMove = sumOfRadii - distance;
                    vPotentialPosition += collision_normal * distanceToMove;
                }
            }
        }

        //
        
        // ------------ Rectangle Collisions ------------ //

        for (auto& r : m_s_Instance->m_Rectangles)
        {
            olc::vf2d vNearestPoint;
            vNearestPoint.x = std::max(float(r.pos.x), std::min(vPotentialPosition.x, float(r.pos.x + r.size.x)));
            vNearestPoint.y = std::max(float(r.pos.y), std::min(vPotentialPosition.y, float(r.pos.y + r.size.y)));

            olc::vf2d vRayToNearest = vNearestPoint - vPotentialPosition;
            float fOverlap = object.radius - vRayToNearest.mag();
            if (std::isnan(fOverlap)) fOverlap = 0;
            if (fOverlap > 0)
            {
                // Statically resolve the collision
                vPotentialPosition = vPotentialPosition - vRayToNearest.norm() * fOverlap;
            }
        }

        //

        //World Boundaries
        vPotentialPosition.x = std::min(m_s_Instance->m_WorldBottomRight.x - object.radius, std::max(vPotentialPosition.x, m_s_Instance->m_WorldTopLeft.x + object.radius));
        vPotentialPosition.y = std::min(m_s_Instance->m_WorldBottomRight.y - object.radius, std::max(vPotentialPosition.y, m_s_Instance->m_WorldTopLeft.y + object.radius));

        object.pos = vPotentialPosition;

        // ------------ End Handle Collisions ------------ //
    }
}

void ShapeHandler::ClearCircles()
{
    m_s_Instance->m_Circles.clear();
}

void ShapeHandler::ClearRectangles()
{
    m_s_Instance->m_Rectangles.clear();
}