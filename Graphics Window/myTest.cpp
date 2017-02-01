
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

void display() {
    // clear screen (clear screen buffer)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(1, 1, 1);
    glRectf(10, 10, 40, 40);
    glEnd();
    
    // switch buffer into new window
    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Trev Rules");
    
    // set background to black
    glClearColor(0, 0, 0, 0);
    
    glutDisplayFunc(display);
    
    // set coordinate system - (0, 0) in upper left
    glOrtho(0, 400, 400, 0, -1, 1);
    glutMainLoop();
}
