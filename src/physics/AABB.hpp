#pragma once

#include <glm/glm.hpp>

namespace HermesEngine
{
    struct AABB
    {
        glm::vec3 min; // Minimum corner of the box
        glm::vec3 max; // Maximum corner of the box

        AABB(const glm::vec3 &min = glm::vec3(0.0f), const glm::vec3 &max = glm::vec3(1.0f)) : min(min), max(max) {}

        // Check if a point is inside the AABB
        bool contains(const glm::vec3 &point) const
        {
            return (point.x >= min.x && point.x <= max.x &&
                    point.y >= min.y && point.y <= max.y &&
                    point.z >= min.z && point.z <= max.z);
        }

        // Check if another AABB is inside this AABB
        bool contains(const AABB &other) const
        {
            return (min.x <= other.min.x && max.x >= other.max.x &&
                    min.y <= other.min.y && max.y >= other.max.y &&
                    min.z <= other.min.z && max.z >= other.max.z);
        }

        // Check if this AABB intersects with another AABB
        bool intersects(const AABB &other) const
        {
            return (min.x <= other.max.x && max.x >= other.min.x &&
                    min.y <= other.max.y && max.y >= other.min.y &&
                    min.z <= other.max.z && max.z >= other.min.z);
        }

        float left() const { return min.x; }
        float right() const { return max.x; }
        float top() const { return max.y; }
        float bottom() const { return min.y; }
        float near() const { return min.z; }
        float far() const { return max.z; }
    };
}