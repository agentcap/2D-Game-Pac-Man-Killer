#include "slope.h"
#include "main.h"

Slope::Slope(bounding_ball_t ball, float height, float width, float rotation, float speed) {
    this->position = glm::vec3(ball.x, ball.y, 0);
    this->rotation = rotation;
    this->speed    = speed;
    this->height   = height;
    this->width    = width;
    this->radius   = ball.radius;

    GLfloat vertex_buffer_data[] = {
        -width/2, height/2, 0, // vertex 1
        width/2, height/2, 0, // vertex 2
        width/2, -height/2, 0, // vertex 3

        -width/2, height/2, 0, // vertex 1
        width/2, -height/2, 0, // vertex 3
        -width/2, -height/2, 0, // vertex 4
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, COLOR_BROWN, GL_FILL);
}


void Slope::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);

    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate              = rotate * glm::translate(glm::vec3(this->radius+this->width/2,0, 0));
    Matrices.model     *= (translate * rotate);
    glm::mat4 MVP       = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}


void Slope::tick() {
    this->position.x += speed;
}

bool Slope::detect_collision(bounding_ball_t player, float speed_v,float speed_h) {
    float x = player.x + player.radius * (float)std::cos(180+this->rotation*M_PI/180.0f);
    float y = player.y + player.radius * (float)std::sin(180+this->rotation*M_PI/180.0f);

    float center_x = this->position.x + (this->radius + this->width/2)*(float)std::cos(this->rotation*M_PI/180.0f);
    float center_y = this->position.y + (this->radius + this->width/2)*(float)std::sin(this->rotation*M_PI/180.0f);

    glm::vec3 trans = trans_cord(glm::vec3(x,y,0),glm::vec3(center_x,center_y,0),-(90-this->rotation));

//    return (std::abs(trans.y) <= this->width/2 && std::abs(trans.x) <= this->height/2 && speed_v <= 0);
    x = player.x-speed_h + player.radius * (float)std::cos(180+this->rotation*M_PI/180.0f);
    y = player.y-speed_v + player.radius * (float)std::sin(180+this->rotation*M_PI/180.0f);

   glm::vec3 trans_before = trans_cord(glm::vec3(x,y,0),glm::vec3(center_x,center_y,0),-(90-this->rotation));

    return ((std::abs(trans.y) <= this->width/2 && std::abs(trans.x) <= this->height/2 && speed_v <= 0)||
            (trans_before.y) > this->width/2 && trans.y < -this->width/2 && std::abs(trans.x) <= this->height/2 && speed_v <= 0);
}

glm::vec3 trans_cord(glm::vec3 pos, glm::vec3 origin,float angle) {
    float x = pos.x - origin.x;
    float y = pos.y - origin.y;

    glm::vec3 cord;

    cord.x = x * (float)std::cos(angle*M_PI/180.f) + y * (float)std::sin(angle*M_PI/180.0f);
    cord.y = -x * (float)std::sin(angle*M_PI/180.f) + y * (float)std::cos(angle*M_PI/180.0f);
    cord.z = 0;

    return cord;
}

glm::vec3 Slope::new_speed(float vx, float vy) {
    if(this->rotation < 90) {
        vx = -std::min(vx,0.00f);
        vy = -std::min(vy,0.001f);
        float VX = vx*std::cos(2* this->rotation * M_PI/180.0f) + vy*std::sin(2*this->rotation * M_PI/180.0f);
        float VY = vy*std::cos(2* this->rotation * M_PI/180.0f) - vx*std::sin(2*this->rotation * M_PI/180.0f);

        return glm::vec3(VX,-VY,0);
    }
    else {
        vx = std::max(vx,0.00f);
        vy = -std::min(vy,0.001f);
        float VX = vx*std::cos(2* this->rotation * M_PI/180.0f) + vy*std::sin(2*this->rotation * M_PI/180.0f);
        float VY = vy*std::cos(2* this->rotation * M_PI/180.0f) - vx*std::sin(2*this->rotation * M_PI/180.0f);

        return glm::vec3(VX,-VY,0);
    }
}
