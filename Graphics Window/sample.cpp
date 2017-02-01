
#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>

int width, height;   /* window width and height */
int wd;                   /* GLUT window handle */

/* Program initialization NOT OpenGL/GLUT dependent,
 as we haven't created a GLUT window yet */
void init() {
    width  = 400;
    height = 400;
}

/* Callback functions for GLUT */

/* Draw the window - this is where all the GL actions are */
void display() {
    /* clear the screen to white */
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

/* Called when window is resized,
 also when window is first created,
 before the first call to display(). */
void reshape(int w, int h) {
    width = w;
    height = h;
    
    /* tell OpenGL to use the whole window for drawing */
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    
    /* do an orthographic parallel projection with the coordinate
     system set to first quadrant, limited by screen/window size */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
}

int main(int argc, char *argv[]) {
    init(); // custom init (not OpenGL dependent)
    
    /* initialize GLUT, let it extract command-line
     GLUT options that you may provide
     - NOTE THE '&' BEFORE argc */
    glutInit(&argc, argv);
    
    /* specify the display to be single
     buffered and color as RGBA values */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    
    /* set the initial window size */
    glutInitWindowSize(width, height);
    
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Trev's OpenGL Program");
    
    /* --- register callbacks with GLUT --- */
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    
    /* start the GLUT main loop */
    glutMainLoop();
    
    return 0;
}
