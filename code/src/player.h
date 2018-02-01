#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player() {}
    Player(float x, float y, float r, color_t *colors, int noColors);
    glm::vec3 position;
    float rotation;
    float radius;
    float speed_h;
    float speed_v;
    void draw(glm::mat4 VP);
    void inc_position(float x, float y, float angle);
    void move_left();
    void move_right();
    void tick(float ground_level);
    void set_height(float y);
    bounding_ball_t bounding_ball();
private:
    VAO *object;
};

#endif // PLAYER_H
