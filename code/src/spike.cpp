#include "spike.h"
#include "main.h"

Spike::Spike(float x , float y, float height, float width,int number,float range, float speed, color_t color) {
    this->position  = glm::vec3(x,y,0);
    this->center    = glm::vec3(x,y,0);
    this->height    = height;
    this->width     = width;
    this->range     = range;
    this->speed     = speed;

    GLfloat vertex_buffer_data[number*9];
    float sz = width/number;

    for(int i=0;i<number;i++) {
        vertex_buffer_data[i*9 + 0] = -width/2 + i * sz;
        vertex_buffer_data[i*9 + 1] = 0;
        vertex_buffer_data[i*9 + 2] = 0;

        vertex_buffer_data[i*9 + 3] = -width/2 + (i+1) * sz;
        vertex_buffer_data[i*9 + 4] = 0;
        vertex_buffer_data[i*9 + 5] = 0;

        vertex_buffer_data[i*9 + 6] = -width/2 + ((float)i+0.5) * sz;
        vertex_buffer_data[i*9 + 7] = height;
        vertex_buffer_data[i*9 + 8] = 0;
    }

    this->object = create3DObject(GL_TRIANGLES, number*3, vertex_buffer_data, color, GL_FILL);
}

void Spike::draw(glm::mat4 VP) {
    Matrices.model      = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model     *= (translate);
    glm::mat4 MVP       = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Spike::tick() {
    this->position.x += speed;
    if(this->position.x > this->center.x) this->speed -= range;
    else if (this->position.x < this->center.x) this->speed += range;
}

bool Spike::detect_collision(bounding_ball_t player, float speed) {
    return (std::abs(this->position.x - player.x) <= player.radius + this->width/2) &&
            (std::abs(this->position.y - player.y) <= player.radius + this->height) && speed <=0 ;
}
