#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct color_t {
    int r;
    int g;
    int b;
};

// nonedit.cpp
GLFWwindow *initGLFW(int width, int height);
GLuint     LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat *color_buffer_data, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const color_t color, GLenum fill_mode = GL_FILL);
void       draw3DObject(struct VAO *vao);

// input.cpp
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void keyboardChar(GLFWwindow *window, unsigned int key);
void mouseButton(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// other_handlers.cpp
void error_callback(int error, const char *description);
void quit(GLFWwindow *window);
void reshapeWindow(GLFWwindow *window, int width, int height);

// Types
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int    NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint    MatrixID;
};

extern GLMatrices Matrices;

// ---- Logic ----

enum direction_t { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };

struct bounding_ball_t {
    float x;
    float y;
    float radius;
};

struct flying_ball_t {
    color_t color;
    float speed;
    float score;
};

void jump_player();
void create_ground(float height);
//bool detect_collision(bounding_box_t a, bounding_box_t b);
bool detect_ground(bounding_ball_t player);
void generate_balls();
void remove_balls(float right_bound);
void remove_slopes(float right_bound);
bool detect_collision(bounding_ball_t player, bounding_ball_t ball, float speed);

void generate_trampolines();
void generate_spikes();
void generate_ponds();
void generate_ball_types();


extern float screen_zoom, screen_center_x, screen_center_y;
void reset_screen();
void zoom_out();
void zoom_in();

// ---- Colors ----
//extern const color_t COLOR_RED;
//extern const color_t COLOR_GREEN;
//extern const color_t COLOR_BLACK;
//extern const color_t COLOR_BACKGROUND;
//extern const color_t COLOR_BROWN;

extern const color_t COLOR_BLACK;
extern const color_t COLOR_BACKGROUND;
extern const color_t COLOR_BROWN;

extern const color_t COLOR_VIOLET;
extern const color_t COLOR_INDIGO;
extern const color_t COLOR_BLUE;
extern const color_t COLOR_GREEN;
extern const color_t COLOR_YELLOW;
extern const color_t COLOR_ORANGE;
extern const color_t COLOR_RED;


#endif
