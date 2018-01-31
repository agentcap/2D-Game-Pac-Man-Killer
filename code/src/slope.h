#include "main.h"

#ifndef SLOPE_H
#define SLOPE_H


class Slope {
public:
    Slope();
    Slope(bounding_ball_t ball, float height, float width, float rotation, float speed);
    bool detect_collision(bounding_ball_t player, float speed);
    void draw(glm::mat4 VP);
    void tick();
    glm::vec3 position;
    float rotation;
    float speed;
    float height;
    float width;
    float radius;
    bounding_ball_t ball;
private:
    VAO *object;
};

glm::vec3 trans_cord(glm::vec3 pos, glm::vec3 origin,float angle);

#endif // SLOPE_H
