#include "main.h"

#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H


class Trampoline {
public:
    Trampoline() {}
    Trampoline(float x, float radius, float height, float base, float width, color_t color);
    bool detect_up_collision(bounding_ball_t player, float speed);
    void draw(glm::mat4 VP);
    bool detect_left_collision(bounding_ball_t player);
    bool detect_right_collision(bounding_ball_t player);
    glm::vec3 position;
    float radius;
    float height;
    float base;
    float width;
private:
    VAO *object;
};

#endif // TRAMPOLINE_H
