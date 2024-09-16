#pragma once

#include "AABB.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace HermesEngine
{
    class Collider
    {
    public:
        virtual ~Collider() = default;

        const AABB &getBoundingBox() const { return boundingBox; }
        virtual void calculateBoundingBox() = 0;
        virtual bool checkCollision(const Collider &other) const = 0;
        virtual void resolveCollision(Collider &other) = 0;
        virtual void accept(class ColliderVisitor &visitor) const = 0;

    protected:
        AABB boundingBox;
    };

    class SphereCollider : public Collider
    {
    public:
        explicit SphereCollider(float r) : radius(r) {}

        void calculateBoundingBox() override;
        bool checkCollision(const Collider &other) const override;
        void resolveCollision(Collider &other) override;
        void accept(ColliderVisitor &visitor) const override;
        float getRadius() const { return radius; }

    private:
        float radius;
    };

    class ColliderVisitor
    {
    public:
        virtual void visit(const Collider &collider) = 0;
        virtual void visit(const SphereCollider &sphereCollider) = 0;
        // Add other visit methods for different collider types
    };

    class CollisionCheckVisitor : public ColliderVisitor
    {
    public:
        bool collisionDetected = false;

        void visit(const Collider &collider) override
        {
            // Default implementation if needed
        }

        void visit(const SphereCollider &sphereCollider) override
        {
            // Implement collision check for SphereCollider
        }

        // Add other visit methods for different collider types
    };

    // Example Visitor for Collision Resolution
    class CollisionResolveVisitor : public ColliderVisitor
    {
    public:
        void visit(const Collider &collider) override
        {
            // Default implementation if needed
        }

        void visit(const SphereCollider &sphereCollider) override
        {
            // Implement collision resolution for SphereCollider
        }

        // Add other visit methods for different collider types
    };
}