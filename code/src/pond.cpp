#include "pond.h"
#include "pond.h"
#include "main.h"
#define PI 3.14159265

Pond::Pond(float x, float y, float radius, float bounce, color_t color) {
    this->position  = glm::vec3(x,y,0);
    this->radius    = radius;
    this->bounce    = bounce;

    int theta = 1, angle = 180;
    int size = (180.0/theta)*9;

    GLfloat vertex_buffer_data[size];

    for(int i=0;i<180.0/theta;i++) {
        vertex_buffer_data[i*9+0] = radius*cos(angle*PI/180);
        vertex_buffer_data[i*9+1] = radius*sin(angle*PI/180);
        vertex_buffer_data[i*9+2] = 0.0;

        vertex_buffer_data[i*9+3] = radius*cos((angle+theta)*PI/180);
        vertex_buffer_data[i*9+4] = radius*sin((angle+theta)*PI/180);
        vertex_buffer_data[i*9+5] = 0.0;


        vertex_buffer_data[i*9+6] = 0.0;
        vertex_buffer_data[i*9+7] = 0.0;
        vertex_buffer_data[i*9+8] = 0.0;

        angle += theta;
    }

    this->object = create3DObject(GL_TRIANGLES, (180/theta)*3, vertex_buffer_data, color, GL_FILL);
}

void Pond::draw(glm::mat4 VP) {
    Matrices.model      = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model     *= (translate);
    glm::mat4 MVP       = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bool Pond::is_in_water(bounding_ball_t player) {
    return (this->position.x-this->radius <= player.x && player.x <= this->position.x+this->radius) && (player.y - player.radius <= this->position.y);
}

bool Pond::is_out(bounding_ball_t player) {
    return (player.x-this->position.x)*(player.x-this->position.x) + (player.y-this->position.y)*(player.y-this->position.y)
            > (player.radius - this->radius)*(player.radius - this->radius);
}

float Pond::set_x_boundary(bounding_ball_t player) {
    float temp = std::min(std::abs(player.y - this->position.y),std::abs(this->radius-player.radius));
    if(player.x > this->position.x) {
        return this->position.x +
            std::sqrt((this->radius-player.radius)*(this->radius-player.radius)
                      - temp*temp);
    }
    else return this->position.x -
            std::sqrt((this->radius-player.radius)*(this->radius-player.radius)
                      - temp*temp);
}

float Pond::set_y_boundary(bounding_ball_t player) {
    if(this->position.y > player.y + player.radius && std::abs(this->position.x - player.x) < this->radius - player.radius) {
        return this->position.y -
                std::sqrt((this->radius-player.radius)*(this->radius-player.radius)
                          - (player.x - this->position.x)*(player.x - this->position.x));
    }
    else return player.y+ player.radius;
}
