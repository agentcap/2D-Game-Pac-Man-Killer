#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, float r, color_t color, float speed, float score);
    glm::vec3 position;
    float radius;
    float speed;
    float score;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_ball_t bounding_ball();
private:
    VAO *object;
};

#endif // BALL_H
