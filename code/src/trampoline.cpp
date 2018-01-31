#include "trampoline.h"
#include "main.h"
#define PI 3.14159265

Trampoline::Trampoline(float x, float r, float height, float base, float width, color_t color) {
    this->position = glm::vec3(x, base, 0);
    this->radius = r;
    this->height = height;
    this->base   = base;
    this->width  = width;

    int theta = 1, angle = 180;
    int size = (180.0/theta)*9;

    GLfloat vertex_buffer_data[size+36];

    for(int i=0;i<180.0/theta;i++) {
        vertex_buffer_data[i*9+0] = r*cos(angle*PI/180);
        vertex_buffer_data[i*9+1] = height + r*sin(angle*PI/180);
        vertex_buffer_data[i*9+2] = 0.0;

        vertex_buffer_data[i*9+3] = r*cos((angle+theta)*PI/180);
        vertex_buffer_data[i*9+4] = height + r*sin((angle+theta)*PI/180);
        vertex_buffer_data[i*9+5] = 0.0;


        vertex_buffer_data[i*9+6] = 0.0;
        vertex_buffer_data[i*9+7] = height;
        vertex_buffer_data[i*9+8] = 0.0;

        angle += theta;
    }
    // Left Strip
    vertex_buffer_data[size+0] = - r - width;
    vertex_buffer_data[size+1] = height;
    vertex_buffer_data[size+2] = 0;

    vertex_buffer_data[size+3] = - r;
    vertex_buffer_data[size+4] = height;
    vertex_buffer_data[size+5] = 0;

    vertex_buffer_data[size+6] = - r - width;
    vertex_buffer_data[size+7] = 0;
    vertex_buffer_data[size+8] = 0;

    vertex_buffer_data[size+9] = - r;
    vertex_buffer_data[size+10] = height;
    vertex_buffer_data[size+11] = 0;

    vertex_buffer_data[size+12] = - r;
    vertex_buffer_data[size+13] = 0;
    vertex_buffer_data[size+14] = 0;

    vertex_buffer_data[size+15] = - r - width;
    vertex_buffer_data[size+16] = 0;
    vertex_buffer_data[size+17] = 0;

    // Right Strip
    vertex_buffer_data[size+18] = r + width;
    vertex_buffer_data[size+19] = height;
    vertex_buffer_data[size+20] = 0;

    vertex_buffer_data[size+21] = r;
    vertex_buffer_data[size+22] = height;
    vertex_buffer_data[size+23] = 0;

    vertex_buffer_data[size+24] = r + width;
    vertex_buffer_data[size+25] = 0;
    vertex_buffer_data[size+26] = 0;

    vertex_buffer_data[size+27] = r;
    vertex_buffer_data[size+28] = height;
    vertex_buffer_data[size+29] = 0;

    vertex_buffer_data[size+30] = r;
    vertex_buffer_data[size+31] = 0;
    vertex_buffer_data[size+32] = 0;

    vertex_buffer_data[size+33] = r + width;
    vertex_buffer_data[size+34] = 0;
    vertex_buffer_data[size+35] = 0;



    this->object = create3DObject(GL_TRIANGLES, (180/theta)*3 + 12, vertex_buffer_data, color, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP) {
    Matrices.model      = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model     *= (translate);
    glm::mat4 MVP       = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bool Trampoline::detect_up_collision(bounding_ball_t player, float speed) {
    return (std::abs(this->position.x - player.x) < player.radius + this->radius + this->width) &&
            ((this->base + this->height <= player.y && player.y <= this->base + this->height +player.radius) ||
             (player.y <= this->base + this->height + player.radius && player.y-speed >= this->base +this->height + player.radius));
}

bool Trampoline::detect_left_collision(bounding_ball_t player) {
    return (std::abs(this->position.x - player.x) < player.radius + this->radius + this->width && player.x < this->position.x) &&
            (this->base <= player.y && player.y <= this->base + this->height);
}

bool Trampoline::detect_right_collision(bounding_ball_t player) {
    return (std::abs(this->position.x - player.x) < player.radius + this->radius + this->width && player.x > this->position.x) &&
            (this->base <= player.y && player.y <= this->base + this->height);
}
