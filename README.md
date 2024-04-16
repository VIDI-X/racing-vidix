# Car Race game 

![Car_Racing](https://github.com/VIDI-X/racing-vidix/assets/68710971/4517de60-d04d-4e08-b7b1-c7c42988348e)

### Team: 3D _Owls
### School: Technical School Čakovec 
### Game name: Car Race 
### Original repo: https://github.com/IVskiller/racing-vidix/  


## Short description:  
In order to play the game, we need two players and two VIDIX. Players connect to each other and they race together on a map displayed on the screen. The aim of the game is to complete the track as quickly as possible. The optimal path through the bend and by keeping the car on the track, the player has the highest speed. By landing from the track, the car loses a lot speed. 

## Technical requirements: 
The only additional component we need for the game is a WiFi antenna that connects to the IPEX connector on the ESP32 microcontroller. 

## Game concept: 
At the very beginning, when we turn on one VIDI X, the message: "TURN ON ANOTHER VIDIX" will be displayed on it. When we turn on another VIDI X, the connection starts, which works only with the previously installed external antenna on the ESP32 (VIDI X is factory configured so that the integrated antenna is not used). After connection, the message: "CONNECTED" is displayed. Then a 3-second countdown begins. Screen will show 2 small cars on a track that we control with the BTN_A button for acceleration and the LEFT/RIGHT buttons for turn. The goal is to keep the car on the road; by going off the road the car loses speed and the edges of the road turn red. 

During the game, our position (1st or 2nd) is shown in the upper left corner. When the first car passes the finish line the game stops and the winner device tells the loser device that the game is over. The last position is displayed on the screen the end of the game. In order to restart the game, we can hold the START button on the VIDIX. It is important to emphasize how we use the "partial refresh" of the screen, which enables a faster refresh compared to the fillScreen method. 

The car has a certain inertia, so that after releasing the accelerator button, car slides on the track, pressing the accelerator adds to it acceleration vector to the car. We can create a path on [GeoGebra](https://www.geogebra.org/) and just insert points into it (the same goes for every other object in the game).  

In the future, we plan to place trees or similar objects along the path. For help when creating the 3D engine, we used the library BasicLinearAlgebra.h. When creating a two-way PTP connection, we used the library esp_now.h. 

The key part of this game and the main challenge was to make a 3D engine. With the help of matrix multiplication, we transferred 3D objects on a 2D perspective screen. Some of the transformation matrices we used are matrix position, rotation and size transformations. 
