#include "ground.h"
#include "main.h"

Ground::Ground(float x,float y,float height, float width, color_t color) {
    this->position = glm::vec3(x, y, 0);
    GLfloat vertex_buffer_data[] = {
        -width/2, height/2, 0, // vertex 1
        width/2, height/2, 0, // vertex 2
        width/2, -height/2, 0, // vertex 3

        -width/2, height/2, 0, // vertex 1
        width/2, -height/2, 0, // vertex 3
        -width/2, -height/2, 0, // vertex 4
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Ground::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
