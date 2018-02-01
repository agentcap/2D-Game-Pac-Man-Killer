#include "ball.h"
#include "main.h"
#define PI 3.14159265

Ball::Ball(float x, float y, float r, color_t color, float speed, float score) {
    this->position = glm::vec3(x, y, 0);
    this->radius   = r;
    this->speed    = speed;
    this->score    = score;

    int theta = 1, angle = 0;
    int size = (360.0/theta)*9;

    GLfloat vertex_buffer_data[size];

    for(int i=0;i<360.0/theta;i++) {
        vertex_buffer_data[i*9+0] = r*cos(angle*PI/180);
        vertex_buffer_data[i*9+1] = r*sin(angle*PI/180);
        vertex_buffer_data[i*9+2] = 0.0;

        vertex_buffer_data[i*9+3] = r*cos((angle+theta)*PI/180);
        vertex_buffer_data[i*9+4] = r*sin((angle+theta)*PI/180);
        vertex_buffer_data[i*9+5] = 0.0;


        vertex_buffer_data[i*9+6] = 0.0;
        vertex_buffer_data[i*9+7] = 0.0;
        vertex_buffer_data[i*9+8] = 0.0;

        angle += theta;
    }

    this->object = create3DObject(GL_TRIANGLES, (360/theta)*3, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model      = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model     *= (translate);
    glm::mat4 MVP       = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.x += speed;
}

bounding_ball_t Ball::bounding_ball() {
    float x = this->position.x, y = this->position.y, r = this->radius;
    bounding_ball_t bball = { x, y, r};
    return bball;
}
