#pragma once


#include "bbe_model.hpp"

#include <memory>
namespace bbe {

    struct Transform2dComponent {
        glm::vec2 translation{};
        glm::vec2 scale{1.f,1.f};

        float rotation;
        glm::mat2 mat2(){
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c,s}, {-s, c}};
            glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}}; //COL 1. COL2
            return rotMatrix * scaleMat;
            
            };
    };
    class BbeGameObject {
        public:
        using id_t = unsigned int;
        static BbeGameObject createGameObject(){
            static id_t current_id = 0;
            return BbeGameObject{current_id++};
        }
        BbeGameObject(const BbeGameObject &) = delete;
        BbeGameObject &operator=(const BbeGameObject &) = delete;
        BbeGameObject(BbeGameObject&&) = default;
        BbeGameObject &operator=(BbeGameObject&&) = default;

        const id_t getId(){return id;}
        std::shared_ptr<BbeModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d{};

        private:
        BbeGameObject(id_t objId) : id{objId} {}
        id_t id;
    };
}