#include <gl/glut.h>
#include <GL/gl.h>
#include <iostream>

class DisplayBox{
    float unitRange = 1;
public:
    void setUnitRange(float);
    void drawDisplayBox();
};

void DisplayBox::setUnitRange(float unitRangeIn){
    unitRange = unitRangeIn;
}

void DisplayBox::drawDisplayBox(){
    float xMin = -unitRange;
    float xMax = unitRange;
    float yMin = -unitRange;
    float yMax = unitRange;
    float zMin = -unitRange;
    float zMax = unitRange;
    
    float coordinate1A[3] = {xMin, yMin, zMax};
    float coordinate1B[3] = {xMin, yMax, zMax};
    float coordinate1C[3] = {xMax, yMax, zMax};
    float coordinate1D[3] = {xMax, yMin, zMax};
    
    float coordinate2A[3] = {xMin, yMin, zMin};
    float coordinate2B[3] = {xMin, yMax, zMin};
    float coordinate2C[3] = {xMax, yMax, zMin};
    float coordinate2D[3] = {xMax, yMin, zMin};
    
    
    // draw z parrallel quads
    
    glColor3f(0.0,1.0,1.0);
               
    glBegin(GL_LINE_LOOP);
    glVertex3f(coordinate1A[0], coordinate1A[1], coordinate1A[2]);
    glVertex3f(coordinate1B[0], coordinate1B[1], coordinate1B[2]);
    glVertex3f(coordinate1C[0], coordinate1C[1], coordinate1C[2]);
    glVertex3f(coordinate1D[0], coordinate1D[1], coordinate1D[2]);
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    glVertex3f(coordinate2A[0], coordinate2A[1], coordinate2A[2]);
    glVertex3f(coordinate2B[0], coordinate2B[1], coordinate2B[2]);
    glVertex3f(coordinate2C[0], coordinate2C[1], coordinate2C[2]);
    glVertex3f(coordinate2D[0], coordinate2D[1], coordinate2D[2]);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(coordinate2A[0], coordinate2A[1], coordinate2A[2]);
    glVertex3f(coordinate1A[0], coordinate1A[1], coordinate1A[2]);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(coordinate2B[0], coordinate2B[1], coordinate2B[2]);
    glVertex3f(coordinate1B[0], coordinate1B[1], coordinate1B[2]);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(coordinate2C[0], coordinate2C[1], coordinate2C[2]);
    glVertex3f(coordinate1C[0], coordinate1C[1], coordinate1C[2]);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(coordinate2D[0], coordinate2D[1], coordinate2D[2]);
    glVertex3f(coordinate1D[0], coordinate1D[1], coordinate1D[2]);
    glEnd();
    
    // draw axis lines
    glBegin(GL_LINES);
    // x axis red
    glColor3f(1.0,0.0,0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(xMax, 0, 0);
    glEnd();
    
    glBegin(GL_LINES);
    // y axis green
    glColor3f(0.0,1.0,0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, yMax, 0);
    glEnd();
    
    glBegin(GL_LINES);
    // z axis blue
    glColor3f(0.0,0.0,1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, zMax);
    glEnd();
    
}
