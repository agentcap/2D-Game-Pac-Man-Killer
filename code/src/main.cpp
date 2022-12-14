#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include "player.h"
#include "slope.h"
#include "trampoline.h"
#include "spike.h"
#include "pond.h"
#include "magnet.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

// Objects in the game
Ground underGround, foreGround;
Player player;
vector<Trampoline> trampolines;
vector<Ball> flying_balls;
vector<Slope> slopes;
vector<Spike> spikes;
vector<Pond> ponds;
Magnet magnet;

int score = 0;
int level = 1;

// Types of Flying Balls
vector<flying_ball_t> ball_types;

float gravity = 0.01;

// Setting the screen dimensions and zoom level.
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float screen_width = 20, screen_height = 10;

// Postion's relative to screen height
float ground_height = screen_height/5;
float ground_level = -screen_height/2+ground_height;


// moved_in_water => detects whether the player is moved up
// along the curvature of the pool
bool moved_in_water = false;
bool jump = false;

bool water_sound = false;
bool start = true;
// t60  - Frame rate of the game
// t2   - Ball Generation Rate
Timer t60(1.0 / 60);
Timer t2(1.0 /2);
Timer t5(9.0);


Timer magnet_timer(2.0);
Timer bgm(20);

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
    Matrices.view = glm::lookAt(glm::vec3(screen_center_x, screen_center_y, 3), glm::vec3(screen_center_x, screen_center_y, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render


    // Drawing the Ground layers
    underGround.draw(VP);
    foreGround.draw(VP);

    // Drawing the magnet
    if(magnet.active) {
        if(magnet.rotation == 0.0) magnet.position.x = screen_center_x - 8.0*screen_zoom;
        else magnet.position.x = screen_center_x + 8.0*screen_zoom;
        magnet.draw(VP);
    }

    // Drawing the flying Objects
    for(int i=0;i<flying_balls.size();i++) {
        flying_balls[i].draw(VP);
    }

    // Drawing the slopes
    for(int i=0;i<slopes.size();i++) {
        slopes[i].draw(VP);
    }

    if(level>2) {
        // Drawing the Spikes
        for(int i=0;i<spikes.size();i++) {
            spikes[i].draw(VP);
        }
    }

    // Drawing the pond
    for(int i=0;i<ponds.size();i++) {
        ponds[i].draw(VP);
    }

    // Drawing the player
    player.draw(VP);

    // Drawing the Trampoline
    for(int i=0;i<trampolines.size();i++) {
        trampolines[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left        = glfwGetKey(window, GLFW_KEY_A);
    int right       = glfwGetKey(window, GLFW_KEY_D);
    int paneRight   = glfwGetKey(window, GLFW_KEY_RIGHT);
    int paneLeft    = glfwGetKey(window, GLFW_KEY_LEFT);
    int paneDown    = glfwGetKey(window, GLFW_KEY_DOWN);
    int paneUp    = glfwGetKey(window, GLFW_KEY_UP);

    moved_in_water = false;
    if(paneLeft) screen_center_x -= 0.05;
    if(paneRight) screen_center_x += 0.05;
    if(paneDown) screen_center_y -= 0.05;
    if(paneUp) screen_center_y += 0.05;

    if (left) {
        // Moving the player according to the environment
        bool in_water = false;
        for(int i=0;i<ponds.size();i++) {
            if(!ponds[i].is_in_water(player.bounding_ball())) {
                player.move_left();
                in_water = true;
            }
        }
        if(!in_water) player.position.x -= 0.05;

        // If the player is in water and is on boundaries, setting the y coordiante according to the curvature.
        for(int i=0;i<ponds.size();i++) {
            if(ponds[i].is_in_water(player.bounding_ball()) && ponds[i].is_out(player.bounding_ball())) {
                player.position.y = ponds[i].set_y_boundary(player.bounding_ball());
                moved_in_water = true;
            }
        }
    }
    if (right) {
        // Moving the player according to the environment
        bool in_water = false;
        for(int i=0;i<ponds.size();i++) {
            if(!ponds[i].is_in_water(player.bounding_ball())) {
                player.move_right();
                in_water = true;
            }
        }
        if(!in_water) player.position.x += 0.05;

        // If the player is in water and is on boundaries, setting the y coordiante according to the curvature.
        for(int i=0;i<ponds.size();i++) {
            if(ponds[i].is_in_water(player.bounding_ball()) && ponds[i].is_out(player.bounding_ball())) {
                player.position.y = ponds[i].set_y_boundary(player.bounding_ball());
                moved_in_water = true;
            }
        }
    }
}

void tick_elements() {
    if(score > 100) level = 2;
    if(score > 200) level = 3;
    if(score > 300) level = 4;

    /*
     *
     * Change the values of the step length and speed acording to the environment
     *
     */

    magnet.tick();
    bool in_water = false;
    for(int i=0;i<ponds.size();i++) {
        if(ponds[i].is_in_water(player.bounding_ball())) {
            gravity = 0.005;
            if(!jump)player.speed_v = -0.01;
            else player.speed_v = 0.2;
            in_water = true;
            if(!water_sound) {
                system(" canberra-gtk-play -f  ../music/resources/water.wav --volume=\"1\"   &");
                water_sound = true;
            }
        }
    }
    if(!in_water){
        water_sound = false;
        jump = false;
        gravity = 0.01;
        player.speed_v -= gravity;
        if(magnet.active) player.speed_h += magnet.acceleration;
    }


    // Detecting collision with slopes
    for(int i=0;i<slopes.size();i++) {
        if(slopes[i].detect_collision(player.bounding_ball(),player.speed_v,player.speed_h)) {
            glm::vec3 temp = slopes[i].new_speed(player.speed_h,player.speed_v);
            player.speed_h = temp.x;
            player.speed_v = temp.y;
            // Playing sound
            system(" canberra-gtk-play -f  ../music/resources/rod.wav --volume=\"1\"   &");
        }
    }

    // Moving the player downwards only if player is not moving up the pool
    if(!moved_in_water) player.tick(ground_level);

    // Detecting Collision with flying balls, giving the boost and removing the balls
    for(int i=0;i<flying_balls.size();i++) {
        if(detect_collision(player.bounding_ball(),flying_balls[i].bounding_ball(),player.speed_v)) {
            player.speed_v = 0.2;
            score += flying_balls[i].score;
            flying_balls.erase(flying_balls.begin()+i);
            i--;

            // Playing sound
            system(" canberra-gtk-play -f  ../music/resources/ball_collision.wav --volume=\"1\"   &");
        }
    }

    // Detecting collision with trampoline from top direction and adjusting the height
    for(int i=0;i<trampolines.size();i++) {
        if(trampolines[i].detect_up_collision(player.bounding_ball(), player.speed_v)) {
            player.set_height(trampolines[i].base + trampolines[i].height + player.radius);
            player.speed_v = min(0.4,-player.speed_v + 0.03);
        }

        // Detecting collision with trampoline from left direction and adjusting the position
        else if(trampolines[i].detect_left_collision(player.bounding_ball())) {
            player.position.x = trampolines[i].position.x - trampolines[i].radius - trampolines[i].width - player.radius;
        }

        // Detecting collision with trampoline from right direction and adjusting the position
        else if(trampolines[i].detect_right_collision(player.bounding_ball())) {
            player.position.x = trampolines[i].position.x + trampolines[i].radius + trampolines[i].width + player.radius;
        }
    }

    // Detecting collision with spikes
    if(level>2) {
        for(int i=0;i<spikes.size();i++) {
            if(spikes[i].detect_collision(player.bounding_ball(),player.speed_v)) {
                spikes.erase(spikes.begin()+i);
                score -= 15;
                i--;
                // Playing sound
                system(" canberra-gtk-play -f  ../music/resources/porcupine.wav --volume=\"1\"   &");
            }
        }
    }

    // Moving the flying objects
    for(int i=0;i<flying_balls.size();i++) {
        flying_balls[i].tick();
    }

    // Moving the Slopes
    for(int i=0;i<slopes.size();i++) {
        slopes[i].tick();
    }

    // Oscillating the Spikes
    for(int i=0;i<spikes.size();i++) {
        spikes[i].tick();
    }

    in_water = false;
    for(int i=0;i<ponds.size();i++) {
        if(ponds[i].is_in_water(player.bounding_ball())) {
            if(player.position.y <= ponds[i].position.y-ponds[i].radius + player.radius && player.position.x == ponds[i].position.x) {
                player.position.y = ponds[i].position.y - ponds[i].radius + player.radius;
            }
            else if(ponds[i].is_out(player.bounding_ball())) {
                player.position.x = ponds[i].set_x_boundary(player.bounding_ball());
                player.speed_v = 0;
            }
            player.speed_h = 0;
            in_water = true;
        }
    }

    // If player is on ground setting the speed to zero and adjusting the height
    if(!in_water && detect_ground(player.bounding_ball())) {
        player.set_height(ground_level+player.radius);
        player.speed_v = 0;
        player.speed_h = 0;
    }

}

/* Function return true if there is a collision between player and ball
 * and the player is moving downwards */
bool detect_collision(bounding_ball_t player, bounding_ball_t ball, float speed) {
    return ((player.x-ball.x)*(player.x-ball.x) +
            (player.y-ball.y)*(player.y-ball.y) <=
            (player.radius + ball.radius)*(player.radius + ball.radius))
            && player.y > ball.y && speed <= 0;
}

void generate_trampolines() {
    trampolines.push_back(Trampoline(8,1,1.5,ground_level,0.2,COLOR_GREEN));
    trampolines.push_back(Trampoline(30,1,1.5,ground_level,0.2,COLOR_GREEN));
    trampolines.push_back(Trampoline(48,1,1.5,ground_level,0.2,COLOR_GREEN));
}

void generate_spikes() {
    spikes.push_back(Spike(-7,ground_level,0.5,2,5,0.005,0.1,COLOR_YELLOW));
    spikes.push_back(Spike(-2,ground_level+3,0.5,2,5,0.002,0.12,COLOR_YELLOW));
    spikes.push_back(Spike(15,ground_level,0.5,2,5,0.007,0.1,COLOR_YELLOW));
    spikes.push_back(Spike(18,ground_level,0.5,2,5,0.005,0.07,COLOR_YELLOW));
    spikes.push_back(Spike(25,ground_level+5,0.5,2,5,0.005,0.2,COLOR_YELLOW));
}

void generate_ponds() {
    ponds.push_back(Pond(-2,ground_level,2,0.5,COLOR_BLUE));
    ponds.push_back(Pond(25,ground_level,2,0.5,COLOR_BLUE));
    ponds.push_back(Pond(35,ground_level,2,0.5,COLOR_BLUE));
}

void generate_ball_types() {
    // Generate types of Ball
    // Add more later
    flying_ball_t type1;
    type1.color = COLOR_BLUE;
    type1.speed = 0.04;
    type1.score = 20;
    ball_types.push_back(type1);
    type1.color = COLOR_ORANGE;
    type1.speed = 0.05;
    type1.score = 30;
    ball_types.push_back(type1);
    type1.color = COLOR_VIOLET;
    type1.speed = 0.06;
    type1.score = 40;
    ball_types.push_back(type1);
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

    generate_trampolines();
    generate_spikes();
    generate_ponds();
    generate_ball_types();

    magnet = Magnet(-8,3,1,0.5,0.5,0.0005,5.0);


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
        if(bgm.processTick()||start) {
            start=false;
            system(" canberra-gtk-play -f  ../music/resources/harpsi-cs.wav --volume=\"10\" &");
        }

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            if(t2.processTick()) generate_balls();
            if(level>3) {
                if(t5.processTick()) {
                    if(magnet_timer.processTick()) {
                        system(" canberra-gtk-play -f  ../music/resources/magnet.wav --volume=\"1\"   &");
                    }

                    if(rand()%2) {
                        magnet.activate(-8,3,0.0);
                    }
                    else magnet.activate(8,3,180.0);
                }
            }
            remove_balls(2.5*screen_width);
            remove_slopes(2.5*screen_width);
            tick_elements();
            tick_input(window);

            string Result;
            stringstream convert;
            //cout << "nonedit score:" << score << endl;
            convert << score;
            Result = convert.str();

            const char *one = "Score ";
            const char *two = Result.c_str();
            const char *three = "   Level ";
            string Result1;
            stringstream convert1;
            convert1 << level;
            Result1 = convert1.str();
            const char *four = Result1.c_str();
            string total( string(one) + two + string(three) + four);
            glfwSetWindowTitle(window, total.c_str());
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

/* Increses speed of the player by 0.3 when on ground */
void jump_player() {
    jump = true;
    bool in_water = false;
    for(int i=0;i<ponds.size();i++) {
        if(ponds[i].is_in_water(player.bounding_ball())) in_water = true;
    }
    if(player.position.y == ground_level + player.radius || in_water) {
        player.speed_v = 0.3;
    }
}

/* Generates ground of given height
 * Creates 2 Layers with height in the ratio 1:5 */
void create_ground(float height) {
    float u_height = (height * 5.0)/6.0;
    float t_height = (height * 1.0)/6.0;
    underGround = Ground(screen_width, -screen_height/2 + u_height/2, u_height, 3*screen_width, COLOR_BROWN);
    foreGround   = Ground(screen_width, -screen_height/2 + u_height + t_height/2, t_height, 3*screen_width, COLOR_GREEN);
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

    // Attach a slope randomly to the ball with probability of 0.2
    if(level>1 && rand()%5 == 0) {
        int angle  = 30 + rand()%120;
//        int angle = 45;
        float height = 3;
        float width  = 0.25;
        Slope slope = Slope(ball.bounding_ball(),height,width,angle,ball.speed);
        slopes.push_back(slope);
    }

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

void remove_slopes(float right_bound) {
    for (int i=0;i<slopes.size();i++) {
        float x = slopes[i].position.x;
        if (x > right_bound) {
            slopes.erase(slopes.begin()+i);
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

void zoom_out() {
    screen_zoom -= 0.1;
    reset_screen();
}

void zoom_in() {
    screen_zoom += 0.1;
    reset_screen();
}
