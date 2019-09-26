# Tam Nguyen _ LAB 2
## Bresenham Algorithm Implementation
1. Old Software Algorithms now Implemented in Hardware - how are lines and circles drawn?
- It is not easy to display a continuous smooth arc on the computer screen as our computer screen is made of pixels organized in matrix form. So, to draw a circle on a computer screen we should always choose the nearest pixels from a printed pixel so as they could form an arc. There are two algorithm to do this:

    -  Mid-Point circle drawing algorithm
    -  Bresenham’s circle drawing algorithm

a. Implement Bresenham's midpoint line scan conversion algorithm
### Program function: 
- Create a GLUT window titled Lab2Midline that is positioned at 100,100 (upper-left hand corner). Make the window large enough to display a line with start coordinate (0,0) and end coordinate (400, 150). The slope of the line will be (150-0)/(400-0) = 3/8.

- Create a user-defined init function that clears the viewport to black.

- Create a keyboard callback function that allows the user to type "q" to exit the application.

- Use the reshape callback function discussed in class to handle window events.

- Create a display callback function that implements the pseudo-code using the openGL point primitive.

- My scan converted line look reasonable in comparison to the openGL line.

b. Implement Bresenham's midpoint circle scan conversion algorithm 
### Program function: 
- Create a GLUT window titled Lab2Midcircle that is positioned at 100,100 (upper-left hand corner). Make the window large enough to display a circle with radius = 100.

- Create a user-defined init function that clears the viewport to black.

- Create a keyboard callback function that allows the user to type "q" to exit the application.

- Modify the reshape callback function to orient the origin at the center of the viewport.

- Create a display callback function that implements the pseudo-code using the openGL point primitive.

- My scan converted line look reasonable in comparison to the openGL line.

