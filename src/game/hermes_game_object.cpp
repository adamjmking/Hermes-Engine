#include "hermes_game_object.hpp"

namespace HermesEngine
{
    // Matrix corresponds to Translate * Ry * Rx * Rz * Scale
    // Rotation convention uses Tait-Bryan angles Y(1), X(2), Z(3)
    glm::mat4 TransformComponent::mat4()
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4{
            {
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            },
            {translation.x, translation.y, translation.z, 1.0f}};
    }

    glm::mat3 TransformComponent::normalMatrix()
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        const glm::vec3 invScale = 1.0f / scale;

        return glm::mat3{
            {
                invScale.x * (c1 * c3 + s1 * s2 * s3),
                invScale.x * (c2 * s3),
                invScale.x * (c1 * s2 * s3 - c3 * s1),
            },
            {
                invScale.y * (c3 * s1 * s2 - c1 * s3),
                invScale.y * (c2 * c3),
                invScale.y * (c1 * c3 * s2 + s1 * s3),
            },
            {
                invScale.z * (c2 * s1),
                invScale.z * (-s2),
                invScale.z * (c1 * c2),
            },
        };
    }

    void Rigidbody::update(float deltaTime)
    {
        position += velocity * deltaTime + 0.5f * acceleration * deltaTime * deltaTime;
        velocity += acceleration * deltaTime;
    }

    void Rigidbody::applyConstantForce(glm::vec3 force)
    {
        acceleration += force;
    }

    HermesGameObject HermesGameObject::createPointLight(float intensity, float radius, glm::vec3 color)
    {
        HermesGameObject lightObj = HermesGameObject::createGameObject();
        lightObj.color = color;
        lightObj.transform.scale.x = radius;
        lightObj.pointLight = std::make_unique<PointLightComponent>();
        lightObj.pointLight->lightIntensity = intensity;
        return lightObj;
    }

    float randBetween(float low, float high)
    {
        return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high - low)));
    }

    HermesGameObject HermesGameObject::createRigidbody(float radius)
    {
        HermesGameObject obj = HermesGameObject::createGameObject();
        obj.transform.scale = glm::vec3{radius};

        obj.rigidbody = std::make_unique<Rigidbody>();
        obj.rigidbody->velocity = glm::vec3{randBetween(-1.f, 1.f), randBetween(-1.f, 1.f), randBetween(-1.f, 1.f)};
        obj.rigidbody->applyConstantForce(glm::vec3{0.f, 9.81f, 0.f});
        return obj;
    }

}