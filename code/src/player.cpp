#include "player.h"
#include "main.h"
#define PI 3.14159265

Player::Player(float x, float y, float r, color_t *colors, int noColors) {
    this->position  = glm::vec3(x, y, 0);
    this->rotation  = 0.0;
    this->radius    = r;
    this->speed_h   = 0;
    this->speed_v   = 0;

    int theta = 1, angle = 0;
    int size = (360.0/theta)*9;

    GLfloat vertex_buffer_data[size], color_buffer_data[size];

    for(int i=0;i<360.0/theta;i++) {
        int idx = std::min(int(angle*noColors/360),noColors);
        color_t color = colors[idx];

        vertex_buffer_data[i*9+0] = r*cos(angle*PI/180);
        vertex_buffer_data[i*9+1] = r*sin(angle*PI/180);
        vertex_buffer_data[i*9+2] = 0.0;

        vertex_buffer_data[i*9+3] = r*cos((angle+theta)*PI/180);
        vertex_buffer_data[i*9+4] = r*sin((angle+theta)*PI/180);
        vertex_buffer_data[i*9+5] = 0.0;


        vertex_buffer_data[i*9+6] = 0.0;
        vertex_buffer_data[i*9+7] = 0.0;
        vertex_buffer_data[i*9+8] = 0.0;

        color_buffer_data[i*9+0] = float(color.r)/255.0;
        color_buffer_data[i*9+1] = float(color.g)/255.0;
        color_buffer_data[i*9+2] = float(color.b)/255.0;

        color_buffer_data[i*9+3] = float(color.r)/255.0;
        color_buffer_data[i*9+4] = float(color.g)/255.0;
        color_buffer_data[i*9+5] = float(color.b)/255.0;

        color_buffer_data[i*9+6] = float(color.r)/255.0;
        color_buffer_data[i*9+7] = float(color.g)/255.0;
        color_buffer_data[i*9+8] = float(color.b)/255.0;

        angle += theta;
    }

    this->object = create3DObject(GL_TRIANGLES, (360/theta)*3, vertex_buffer_data, color_buffer_data, GL_FILL);
}

void Player::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);

    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Player::set_height(float y) {
    this->position.y = y;
}

void Player::inc_position(float x, float y, float angle) {
    this->position.x += x;
    this->position.y += y;
    this->rotation += angle;
}

void Player::move_left() {
    inc_position(-0.1,0,10);
}

void Player::move_right() {
    inc_position(0.1,0,-10);
}

void Player::tick() {
    this->position.x += speed_h;
    this->position.y += speed_v;
}

bounding_ball_t Player::bounding_ball() {
    float x = this->position.x, y = this->position.y, r = this->radius;
    bounding_ball_t bball = { x, y, r};
    return bball;
}
