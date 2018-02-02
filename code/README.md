##About the Game

### Introduction
- This is an arcade game where the player controls a metal player and uses it to land on and destroy other flying balls constantly traversing the screen from left to right.
- On landing on the flying balls, they disappear and the player gets launched upwards a little higher.
The primary objective is to destroy as many balls as possible.

### Levels
- 4 levels have been implemented.
- 1st level: moving balls, trampoline, pond will be present
- 2nd level: If score > 100 moving balls with slides will come into picture.
- 3rd level: If score > 200 a magnet which appears left and right at random times and pulls the ball towards it.
- 4th level: If score > 300 porcupines will appear and player looses ponits if he jumps on it.


## Running the Game
- create a build directory in the folder graphics-boilerplate.
- go to build and execute the following commands:

cmake ..
make
./graphics_asgn1

##Controls

### Basic controls
key 'A' 	--> move left
key 'D' 	--> move right
key 'space' --> move jump

### Other controls to adjust screen
scroll down 			--> zoom in 
scroll up       		--> zoom out
key 'left arrow key' 	--> pan left
key 'right arrow key' 	--> pan right
key 'up arrow key' 		--> pan up
key 'down arrow key' 	--> pan down

### Bonus Implemented
#### Sounds
- Kept a Background music
- Sound while interacting with objects like magnet, water, slopes, porcupines

#### Display Score
- Displaying the score and level on the top of the window
- Implemented 4 Levels
- Different score for destorying different kind of files.

#### Extendable World
- The world can be extended in the right direction.