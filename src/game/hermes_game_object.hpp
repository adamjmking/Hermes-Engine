#pragma once

#include "../graphics/hermes_model.hpp"

#include "../physics/collider.hpp"
#include "../physics/AABB.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>

namespace HermesEngine
{
    struct TransformComponent
    {
        glm::vec3 translation{}; // position offset
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{0.f, 0.f, 0.f};

        // Matrix corresponds to Translate * Ry * Rx * Rz * Scale
        // Rotation convention uses Tait-Bryan angles Y(1), X(2), Z(3)
        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    struct PointLightComponent
    {
        float lightIntensity = 1.0f;
    };

    struct Rigidbody
    {
        float radius = 0.2f;
        glm::vec3 position{};
        glm::vec3 velocity{};
        glm::vec3 acceleration{};

        void update(float deltaTime);
        void applyConstantForce(glm::vec3 force);
    };

    class HermesGameObject
    {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, HermesGameObject>;

        static HermesGameObject createGameObject()
        {
            static id_t currentId = 0;
            return HermesGameObject{currentId++};
        }

        static HermesGameObject createPointLight(float intensity = 1.0f, float radius = 0.1f, glm::vec3 color = glm::vec3{1.f});

        static HermesGameObject createRigidbody(float radius = 0.1f);

        HermesGameObject(const HermesGameObject &) = delete;
        HermesGameObject &operator=(const HermesGameObject &) = delete;
        HermesGameObject(HermesGameObject &&) = default;
        HermesGameObject &operator=(HermesGameObject &&) = default;

        id_t getId() const { return id; }

        glm::vec3 color{};
        TransformComponent transform{};

        // Optional pointer components
        std::shared_ptr<HermesModel> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr;
        std::unique_ptr<Rigidbody> rigidbody = nullptr;

    private:
        HermesGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}