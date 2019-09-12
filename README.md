# Tam Nguyen _ LAB 1
## OpenGL Graphic Primitives

This is a program using OpenGL to display all primitives in OpenGL, including: 

| Type of primitive  | Explaination |
| ------------- | ------------- |
| GL_POINTS  | individual points  |
| GL_LINES  | pairs of vertices interpreted as line segments. |
|GL_LINE_STRIP | connected line segments.|
|GL_LINE_LOOP | same as 3, segment between first and last segments.|
|GL_TRIANGLES | triples of vertices interpreted as triangles.|
|GL_TRIANGLE_STRIP | linked strip of triangles.|
|GL_TRIANGLE_FAN | linked fan of triangles.|
|GL_QUADS | quads. of vertices interpreted as 4-sided polygons.|
|GL_QUAD_STRIP | linked strip of quadrilaterals.|
|GL_POLYGON | boundary of a simple convex polygon|

### Program function: 

- Create a GLUT window titled Lab1 that is positioned at 100, 100 (upper-left hand corner). Make the window large enough to display ALL of the openGL graphic primitives.

- Create a user-defined init function that clears the viewport to black.

- Create a keyboard callback function that allows the user to type "q" to exit the application.

- Use the reshape callback function discussed in class to handle window events.

- Create a display callback function that draws each graphic primitive using a different color. Points are drawn with a larger pen size so they are visible.

- Draw a new arrow using GL_POLYGON primitive. The arrow uses GL_SMOOTH mode to fill color. 


