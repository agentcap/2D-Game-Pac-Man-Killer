#include "magnet.h"
#include "main.h"
#include "timer.h"
#define PI 3.14159265

Magnet::Magnet(float x, float y, float radius, float width, float length, float acceleration, float interval) {
    this->position  = glm::vec3(x,y,0);
    this->rotation  = 0.0;
    this->acceleration = acceleration;
    this->active    = false;
    this->interval      = interval;

    int theta = 1, angle = 90;
    int size = (180.0/theta)*9;

    GLfloat vertex_buffer_data[2*size+ 12*3], color_buffer_data[2*size+12*3];

    color_t color = COLOR_RED;
    for(int j=0;j<2;j++) {
        angle = 90;
        if(j==1) color = COLOR_BACKGROUND;
        printf("%d j, %lf inner\n",j,(radius-(float)j*width));
        for(int i=0;i<180.0/theta;i++) {
            vertex_buffer_data[size*j + i*9+0] = (radius-(float)j*width)*cos(angle*PI/180);
            vertex_buffer_data[size*j + i*9+1] = (radius-(float)j*width)*sin(angle*PI/180);
            vertex_buffer_data[size*j + i*9+2] = 0.0;

            vertex_buffer_data[size*j + i*9+3] = (radius-(float)j*width)*cos((angle+theta)*PI/180);
            vertex_buffer_data[size*j + i*9+4] = (radius-(float)j*width)*sin((angle+theta)*PI/180);
            vertex_buffer_data[size*j + i*9+5] = 0.0;

            vertex_buffer_data[size*j + i*9+6] = 0.0;
            vertex_buffer_data[size*j + i*9+7] = 0.0;
            vertex_buffer_data[size*j + i*9+8] = 0.0;

            color_buffer_data[size*j + i*9+0] = float(color.r)/255.0;
            color_buffer_data[size*j + i*9+1] = float(color.g)/255.0;
            color_buffer_data[size*j + i*9+2] = float(color.b)/255.0;

            color_buffer_data[size*j + i*9+3] = float(color.r)/255.0;
            color_buffer_data[size*j + i*9+4] = float(color.g)/255.0;
            color_buffer_data[size*j + i*9+5] = float(color.b)/255.0;

            color_buffer_data[size*j + i*9+6] = float(color.r)/255.0;
            color_buffer_data[size*j + i*9+7] = float(color.g)/255.0;
            color_buffer_data[size*j + i*9+8] = float(color.b)/255.0;

            angle += theta;
        }
    }

    for(int i=0;i<2;i++) {
        int tag = 1;
        if(i == 1) tag = -1;
        color = COLOR_BROWN;
        vertex_buffer_data[i*18+2*size+0] = 0.0;
        vertex_buffer_data[i*18+2*size+1] = tag*radius;
        vertex_buffer_data[i*18+2*size+2] = 0.0;

        vertex_buffer_data[i*18+2*size+3] = 0.0;
        vertex_buffer_data[i*18+2*size+4] = tag*(radius-width);
        vertex_buffer_data[i*18+2*size+5] = 0.0;

        vertex_buffer_data[i*18+2*size+6] = length;
        vertex_buffer_data[i*18+2*size+7] = tag*radius;
        vertex_buffer_data[i*18+2*size+8] = 0.0;

        color_buffer_data[i*18+2*size + 0] = float(color.r)/255.0;
        color_buffer_data[i*18+2*size + 1] = float(color.g)/255.0;
        color_buffer_data[i*18+2*size + 2] = float(color.b)/255.0;

        color_buffer_data[i*18+2*size + 3] = float(color.r)/255.0;
        color_buffer_data[i*18+2*size + 4] = float(color.g)/255.0;
        color_buffer_data[i*18+2*size + 5] = float(color.b)/255.0;

        color_buffer_data[i*18+2*size + 6] = float(color.r)/255.0;
        color_buffer_data[i*18+2*size + 7] = float(color.g)/255.0;
        color_buffer_data[i*18+2*size + 8] = float(color.b)/255.0;

        vertex_buffer_data[i*18+2*size+9] = length;
        vertex_buffer_data[i*18+2*size+10] = tag*radius;
        vertex_buffer_data[i*18+2*size+11] = 0.0;

        vertex_buffer_data[i*18+2*size+12] = length;
        vertex_buffer_data[i*18+2*size+13] = tag*(radius-width);
        vertex_buffer_data[i*18+2*size+14] = 0.0;

        vertex_buffer_data[i*18+2*size+15] = 0.0;
        vertex_buffer_data[i*18+2*size+16] = tag*(radius-width);
        vertex_buffer_data[i*18+2*size+17] = 0.0;

        color_buffer_data[i*18+2*size + 9] = float(color.r)/255.0;
        color_buffer_data[i*18+2*size + 10] = float(color.g)/255.0;
        color_buffer_data[i*18+2*size + 11] = float(color.b)/255.0;

        color_buffer_data[i*18+2*size + 12] = float(color.r)/255.0;
        color_buffer_data[i*18+2*size + 13] = float(color.g)/255.0;
        color_buffer_data[i*18+2*size + 14] = float(color.b)/255.0;

        color_buffer_data[i*18+2*size + 15] = float(color.r)/255.0;
        color_buffer_data[i*18+2*size + 16] = float(color.g)/255.0;
        color_buffer_data[i*18+2*size + 17] = float(color.b)/255.0;
    }

    this->object = create3DObject(GL_TRIANGLES, 2*(180/theta)*3 + 18, vertex_buffer_data, color_buffer_data, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model      = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP       = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Magnet::activate(float x, float y, float angle) {
    this->position.x = x;
    this->position.y = y;
    this->rotation   = angle;
    if(angle == 0) this->acceleration = -std::abs(this->acceleration);
    else this->acceleration = std::abs(this->acceleration);
    this->active     = true;
    this->prevTime = glfwGetTime();
}

//Timer timer
void Magnet::tick() {
    if(this->active && glfwGetTime() - this->prevTime > this->interval) {
        this->active = false;
    }
}
