#include "main.h"
#include "timer.h"

#ifndef MAGNET_H
#define MAGNET_H


class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, float radius, float width, float length, float acceleration, float interval);
    void draw(glm::mat4 VP);
    void activate(float x, float y, float angle);
    void tick();
    glm::vec3 position;
    float rotation;
    float acceleration;
    bool active;
    Timer timer;
    double prevTime;
    float interval;
private:
    VAO *object;

};

#endif // MAGNET_H
