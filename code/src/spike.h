#include "main.h"

#ifndef SPIKE_H
#define SPIKE_H


class Spike {
public:
    Spike() {}
    Spike(float x , float y, float height, float width, int number, float range, float speed, color_t color);
    void draw(glm::mat4 VP);
    void tick();
    bool detect_collision(bounding_ball_t player);
    glm::vec3 position;
    glm::vec3 center;
    float height;
    float width;
    float range;
    float speed;
private:
    VAO *object;
};

#endif // SPIKE_H
