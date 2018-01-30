#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include "player.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ground underGround, foreGround;
Player player;
vector<Ball> flying_balls;


float gravity = 0.01;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float screen_width = 20, screen_height = 10;

// Postion's relative to screen height
float ground_height = screen_height/5;
float ground_level = -screen_height/2+ground_height;

// Types of Flying Balls
vector<flying_ball_t> ball_types;

Timer t60(1.0 / 60);
Timer t2(1.0 /2);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render


    underGround.draw(VP);
    foreGround.draw(VP);
    player.draw(VP);

    // Drawing the flying Objects
    for(int i=0;i<flying_balls.size();i++) {
        flying_balls[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_A);
    int right = glfwGetKey(window, GLFW_KEY_D);
    if (left) {
        player.move_left();
    }
    if (right) {
        player.move_right();
    }
}

void tick_elements() {
    player.speed -= gravity;
    player.tick();

    // Moving the flying objects
    for(int i=0;i<flying_balls.size();i++) {
        flying_balls[i].tick();
    }

    if(detect_ground(player.bounding_ball())) {
        player.set_height(ground_level+player.radius);
        player.speed = 0;
    }
}

void detect_collision(bounding_ball_t player, bounding_ball_t ball) {
//    if (player.)
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */

    // Create the models
    create_ground(ground_height);

    // Create Player with rainbow colors
    color_t rainbowColors[] = {COLOR_VIOLET,COLOR_INDIGO,COLOR_BLUE,COLOR_GREEN,COLOR_YELLOW,COLOR_ORANGE,COLOR_RED};
    player = Player(2, ground_level + 0.3, 0.3, rainbowColors,7);

    /*
     *
     *
     *
     *  Change this
     *
     *
     *
     *
     */
    // Generate types of Ball
    // Add more later
    flying_ball_t type1;
    type1.color = COLOR_RED;
    type1.speed = 0.05;
    type1.score = 100;
    ball_types.push_back(type1);
    type1.color = COLOR_ORANGE;
    type1.speed = 0.06;
    ball_types.push_back(type1);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1200;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            if(t2.processTick()) generate_balls();
            remove_balls(screen_width/2);
            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

/* Increses speed of the player by 0.3 when on ground */
void jump_player() {
    if(player.position.y == ground_level + player.radius) {
        player.speed += 0.3;
    }
}

/* Generates ground of given height
 * Creates 2 Layers with height in the ratio 1:5 */
void create_ground(float height) {
    float u_height = (height * 5.0)/6.0;
    float t_height = (height * 1.0)/6.0;
    underGround = Ground(0, -screen_height/2 + u_height/2, u_height, screen_width, COLOR_BROWN);
    foreGround   = Ground(0, -screen_height/2 + u_height + t_height/2, t_height, screen_width, COLOR_GREEN);
}


/* Generate a flying ball of random radius at random height
 * and adds it to the flying_balls vector */
void generate_balls() {

    // Select a Type of Ball
    int no = ball_types.size();
    flying_ball_t ball_t = ball_types[rand()%no];

    // Let it start at start of screen width;
    float x = -screen_width/2;

    // Generate the Y Cordinateof the ball
    float lower_bound = ground_level + 2;
    int mod = screen_height/2;
    float y = lower_bound + rand()%mod;

    //  Generate Radius in the range of 0.1 to 0.4
    float radius = float(2 + rand()%4)/10;

    Ball ball = Ball(x,y,radius,ball_t.color,ball_t.speed,ball_t.score);

    flying_balls.push_back(ball);
}

/* This function removes all the balls that are outside the world
 * The parameter passed is the right bound of the world */
void remove_balls(float right_bound) {
    for (int i=0;i<flying_balls.size();i++) {
        float x = flying_balls[i].position.x;
        if (x > right_bound) {
            flying_balls.erase(flying_balls.begin()+i);
            i--;
        }
    }
}

void reset_screen() {
    float top    = screen_center_y + (screen_height/2) / screen_zoom;
    float bottom = screen_center_y - (screen_height/2) / screen_zoom;
    float left   = screen_center_x - (screen_width/2) / screen_zoom;
    float right  = screen_center_x + (screen_width/2) / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

/* Function return true when the player collides with ground */
bool detect_ground(bounding_ball_t player) {
    return player.y - player.radius <= ground_level ;
}
