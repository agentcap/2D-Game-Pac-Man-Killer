#include "main.h"

#ifndef POND_H
#define POND_H


class Pond
{
public:
    Pond() {}
    Pond(float x, float y, float radius, float bounce, color_t color);
    bool is_in_water(bounding_ball_t player);
    bool is_out(bounding_ball_t player);
    float set_x_boundary(bounding_ball_t player);
    float set_y_boundary(bounding_ball_t player);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    float radius;
    float bounce;
private:
    VAO *object;
};

#endif // POND_H
