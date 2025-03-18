#pragma once

#include <vector>
#include <memory>

#include "../component/Mesh.hpp"
#include "Vector.hpp"

struct Plane 
{
    float a, b, c, d;

    float distanceToPoint(const Vector3f& p) const
    {
        return a * p.x + b * p.y + c * p.z + d;
    }
};

class Frustum 
{
public:
    std::array<Plane, 6> planes;

    void SetPlane(int index, float a, float b, float c, float d) 
    {
        planes[index] = {a, b, c, d};
    }
};

class OcTreeNode
{
public:
    Vector3f centerPoint;
    float size;
    std::vector<Vector3f> points;
    std::vector<std::unique_ptr<OcTreeNode>> children;

    OcTreeNode(Vector3f centerPoint, float size) : centerPoint(centerPoint), size(size)
    {
        for(int i = 0; i < 8; i++)
        {
            children.push_back(nullptr);
        }
    }

    bool ContainsPoint(const Vector3f& point) 
    {
        return point.x >= centerPoint.x - size / 2.0f && point.x <= centerPoint.x + size / 2.0f &&
               point.y >= centerPoint.y - size / 2.0f && point.y <= centerPoint.y + size / 2.0f &&
               point.z >= centerPoint.z - size / 2.0f && point.z <= centerPoint.z + size / 2.0f;
    }

    void Insert(Vector3f point)
    {
        if (!ContainsPoint(point)) 
        {
            //std::cout<<"OcTreeNode Not ContainsPoint "<< point.x<<" "<<point.y<<" "<<point.z<<" "<<centerPoint.x<<" "<<centerPoint.y<<" "<<centerPoint.z<<" "<<size<<std::endl;
            return;
        }

        if (size <= 0.01f) 
        {
            //std::cout<<"Insert Success"<<std::endl;
            points.push_back(point);
            return;
        }

        float newSize = size / 2.0f;
        for (int i = 0; i < 8; i++) 
        {
            Vector3f newCenter
            (
                centerPoint.x + newSize * (i & 4 ? 0.5f : -0.5f),
                centerPoint.y + newSize * (i & 2 ? 0.5f : -0.5f),
                centerPoint.z + newSize * (i & 1 ? 0.5f : -0.5f)
            );

            if (children[i] == nullptr) 
            {
                children[i] = std::make_unique<OcTreeNode>(newCenter, newSize);
            }
            children[i]->Insert(point);
        }
    }

    bool Intersects(const Frustum& frustum) 
    {
        for (const auto& plane : frustum.planes) 
        {
            if (plane.distanceToPoint(Vector3f(centerPoint.x - size/2.0f, centerPoint.y - size/2.0f, centerPoint.z - size/2.0f)) > 0 &&
                plane.distanceToPoint(Vector3f(centerPoint.x + size/2.0f, centerPoint.y + size/2.0f, centerPoint.z + size/2.0f)) > 0 &&
                plane.distanceToPoint(Vector3f(centerPoint.x - size/2.0f, centerPoint.y + size/2.0f, centerPoint.z + size/2.0f)) > 0 &&
                plane.distanceToPoint(Vector3f(centerPoint.x + size/2.0f, centerPoint.y - size/2.0f, centerPoint.z - size/2.0f)) > 0 &&
                plane.distanceToPoint(Vector3f(centerPoint.x + size/2.0f, centerPoint.y + size/2.0f, centerPoint.z - size/2.0f)) > 0 &&
                plane.distanceToPoint(Vector3f(centerPoint.x - size/2.0f, centerPoint.y - size/2.0f, centerPoint.z + size/2.0f)) > 0 &&
                plane.distanceToPoint(Vector3f(centerPoint.x + size/2.0f, centerPoint.y - size/2.0f, centerPoint.z + size/2.0f)) > 0 &&
                plane.distanceToPoint(Vector3f(centerPoint.x - size/2.0f, centerPoint.y + size/2.0f, centerPoint.z - size/2.0f)) > 0)
                return false; 
        }
        return true; 
    }

    void Cull(const Frustum& frustum, std::vector<Vector3f>& visiblePoints) 
    {
        if (!Intersects(frustum))
        {
            return;
        }

        if (size <= 0.01f && !points.empty()) 
        {
            visiblePoints.insert(visiblePoints.end(), points.begin(), points.end());
            std::cout<<points.size()<<std::endl;
        } 
        else 
        {
            for (auto& child : children) 
            {
                if (child) 
                {
                    child->Cull(frustum, visiblePoints);
                }
            }
        }
    }
};