# Project 2

An animated 3D robot is created by using OpenGL.

## Program attributes:

### Hierachy Objects

- Smooth, filled and colored 3D polygons are used. 
- Surface normals for each polygon must be defined.
- Robot's body part is represented in a tree with the head as the root, the right/left forearm as child of right/left upper arm and so on.
- Display function using push/pop matrix

### The scene
- Translation and rotation transformations are also used to control the animation of each body part. 
- One positional light are used in DIFFUSE and AMBIENT mode.
- Material properties are defined for scene objects in DIFFUSE mode. 

### Animation options
Lift right upper leg

- Rotate Robot: Rotating Torso around a circle with animation in arm and leg
- Rotate head with x-axis : Rotate head in 45 degrees continuously through x-axis
- Rotate head with y-axis: Rotate head in 45 degrees continuously through y-axis
- Lift right upper arm: Move the right upper arm with right lower arm up in a 45 degrees
- Lift right lower arm": Move the right lower arm down in a 45 degrees
- Lift left upper arm: Move the left upper arm with left lower arm up in a 45 degrees
- Lift left lower arm: Move the left lower arm down in a 45 degrees
- Lift right upper leg: Move the right upper leg with right lower leg up in a 45 degrees
- Lift right lower leg: Move the right lower leg down in a 45 degrees
- Lift left upper leg: Move the left upper leg with left lower leg up in a 45 degrees
- Lift left lower leg: Move the left lower leg down in a 45 degrees
- SHOOT: Shooting a ball continously 
- STOP: Stop animation
- RESET: Reset the object back to original position and initinal degrees for each particle. 
- Exit: Quit the program 

### Keyboard options
- Using key arrow to move the robot around the scene
- Hit "Q" to exit the program
