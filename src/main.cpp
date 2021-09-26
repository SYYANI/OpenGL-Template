#include <stdio.h>
#include <iostream>
#include <gl/glut.h>
#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include <string> 
#include <fstream>
#include "Vector3f.h" 
#include "displayBox.h"
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#define M_PI 3.14159265358979323846
using namespace std;
struct Point{
    double x;
    double y;
    double z;
};
static double currentX, currentY,currentZ;            //用于绘制曲线曲面时，临时保存当前点信息
static     double interval_1 = 0.05;            //绘制曲面时网格的间隔控制
static     double interval_2 = interval_1;     //绘制曲面时曲线的节点间隔控制
static Point eye = {2.0,2.0,2.0};
static Point at = {0,0,0};
static Point translate = {0,0,0};
//暂存鼠标操作获取的位置信息
static double scale=1.0;
static int prePosX,prePosY;
static float c=M_PI/180.0f; //弧度和角度转换参数
static int du=90,oldmy=-1,oldmx=-1; //du是视点绕y轴的角度,opengl里默认y轴是上方向
static float r=1.5f,h=0.0f; //r是视点绕y轴的半径,h是视点高度即在y轴上的坐标
float yRand = -10.0+(float)(rand())/(float)(RAND_MAX/(10.0-(-10.0)));
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const char* WINDOW_TITLE = "Bezier Surface";
const float WALKING_SPEED = 10.0;
const float MOUSE_SENSITIVITY = 0.2;
float LAST_TIME;
float CURRENT_TIME;
float DELTA_TIME;
float graphTime = 0;
int CTRLI=0;
int CTRLJ=0;
int MOUSE_LAST_X;
int MOUSE_LAST_Y;
int MOUSE_CURRENT_X;
int MOUSE_CURRENT_Y;
int MOUSE_DELTA_X;
int MOUSE_DELTA_Y;
Vector3f CAMERA_POSITION;
Vector3f CAMERA_ROTATION;
bool KEYS[256];
void display();
void reshape(int screenDisplayWidth, int screenDisplayHeight);
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void SpecialKeys(int key,int x,int y);
void mouseMove(int x, int y);
void mouseClick(int button, int state, int x, int y);
void preprocessEvents();
bool leftButton = false;
// GLOABLA GRAPH VARIABLES
float STEPPING_RATE=0.1;
GLfloat control[4][4][3];
//计算阶乘
int fac(int n)
{
    if (n == 1 || n == 0)
        return 1;
    else
        return n * fac(n - 1);
}
//计算幂
double powi(double base, int n)
{
    double res = 1;
    for (int i = 1; i <= n; ++i)
        res *= base;
    return res;
}
//用于每次画曲线时的第一个点
void MoveTo(double a, double b,double c)
{
    
    
    currentX = a;
    currentY = b;
    currentZ = c;
}
//用于每次画曲线过程中，连接下一个点
void LineTo(double a, double b,double c)
{
    glBegin(GL_LINES);
    glVertex3f(currentX, currentY,currentZ);
    glVertex3f(a, b,c);
    glEnd();
    currentX = a;
    currentY = b;
    currentZ = c;
}

//Bezier曲面
void BezierSuface(int m,int n)
{
    //分别绘制 u ，w 方向的曲线形成曲面的特征网格
    glColor3f(0.0f,0.0f,0.0f);
    double a, b, c;
    double temp, temp1, temp2, Bui, Bwj;
    // 在 u方向上 每隔interval_1 的间隔，绘制一条 w 方向的曲线（曲线节点的间隔为interval_2）
    for(double u =0 ; u <=1 ; u+=interval_1)
    {
        for(double w=0.0 ; w < 1+interval_2; w+=interval_2)
        {//结束条件在此本应该为w <= 1 ，但允许结果由于浮点数精度问题没能取到w=1,所以作此优化
            a = 0.0;
            b = 0.0;
            c = 0.0;
            for(int i=0;i<=m;i++)
            {
                for(int j=0;j<=n;j++)
                {
                    temp = double(fac(m) / fac(i) / fac(m - i));
                    temp1 = powi(u, i);
                    temp2 = powi(1 - u, m - i);
                    Bui = temp * temp1 * temp2;

                    temp = double(fac(n) / fac(j) / fac(n - j));
                    temp1 = powi(w, j);
                    temp2 = powi(1 - w, n - j);
                    Bwj = temp * temp1 * temp2;

                    a += Bui * Bwj *  control[i][j][0];
                    b += Bui * Bwj *  control[i][j][1];
                    c += Bui * Bwj *  control[i][j][2];
                }
            }
            if(w==0) MoveTo(a, b,c);
            else LineTo(a, b,c);
        }
    }

    // 在 w方向上 每隔interval_1 的间隔，绘制一条 u 方向的曲线（曲线节点的间隔为interval_2）
    for(double w =0 ; w <=1 ; w+=interval_1)
    {
        for(double u=0.0 ; u < 1+interval_2; u+=interval_2)
        {
            a = 0.0;
            b = 0.0;
            c = 0.0;
            for(int i=0;i<=m;i++)
            {
                for(int j=0;j<=n;j++)
                {
                    temp = double(fac(m) / fac(i) / fac(m - i));
                    temp1 = powi(u, i);
                    temp2 = powi(1 - u, m - i);
                    Bui = temp * temp1 * temp2;

                    temp = double(fac(n) / fac(j) / fac(n - j));
                    temp1 = powi(w, j);
                    temp2 = powi(1 - w, n - j);
                    Bwj = temp * temp1 * temp2;

                    a += Bui * Bwj *  control[i][j][0];
                    b += Bui * Bwj *  control[i][j][1];
                    c += Bui * Bwj *  control[i][j][2];
                }
            }
            if(u==0) MoveTo(a, b,c);
            else LineTo(a, b,c);
        }
    }
}

int main(int argc, char *argv[])
{
    //从文件读取矩阵数据
    float x,y,z;
    ifstream infile;   //输入流
    infile.open("C:\\Users\\Administrator\\Desktop\\OpenGL\\src\\1.txt", ios::in);
    if(!infile.is_open ())
        cout << "Open file failure" << endl;
    while (!infile.eof())            // 若未到文件结束一直循环
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                infile >> x >> y >> z;
                control[i][j][0]=x;
                control[i][j][1]=y;
                control[i][j][2]=z;
            }
        }
    }
    infile.close();   //关闭文件
    //矩阵放大5倍
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            control[i][j][0]*=5;
            control[i][j][1]*=5;
            control[i][j][2]*=5;
        }
    }
	glutInit(&argc, argv);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	// Create the window
	glutCreateWindow(WINDOW_TITLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable( GL_BLEND ); 
	glEnable( GL_POINT_SMOOTH ); 	
	glClearColor(0.6,0.6,0.8,0.0);
    //glClearColor(1.0,1.0,1.0,0.0);
    //曲面材质和光照
    GLfloat ambient[] = { 0.4, 0.6, 0.2, 1.0 };
    GLfloat position[] = { 0.0,0.5,1.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.2, 0.4, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 80.0 };
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,45.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	// Set GLUT Callback Functions
	glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(SpecialKeys);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseClick);
	// Enter Main Loop
	glutMainLoop();
	return 0;
}

//实现旋转、缩放和平移
void keyboardDown(unsigned char key, int x, int y){
	KEYS[key] = true;
}

void keyboardUp(unsigned char key, int x, int y){
	KEYS[key] = false;
}

void mouseMove(int x, int y){
	MOUSE_CURRENT_X = x;
	MOUSE_CURRENT_Y = y;
}

void mouseClick(int botton, int state, int x, int y){
	if (botton == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			leftButton = true;
		}
	if (state == GLUT_UP){
			leftButton = false;
		}
	}
}

void preprocessEvents(){
    
	// Update Globals
	CURRENT_TIME = ((float)glutGet(GLUT_ELAPSED_TIME)) / 1000;
	DELTA_TIME = CURRENT_TIME - LAST_TIME;
	LAST_TIME = CURRENT_TIME;

	MOUSE_DELTA_X = MOUSE_CURRENT_X - MOUSE_LAST_X;
	MOUSE_DELTA_Y = MOUSE_CURRENT_Y - MOUSE_LAST_Y;

	MOUSE_LAST_X = MOUSE_CURRENT_X;
	MOUSE_LAST_Y = MOUSE_CURRENT_Y;

	// Process Keys for Motion
	if (KEYS['w']){
		CAMERA_POSITION.z += WALKING_SPEED*DELTA_TIME;
	}
	if (KEYS['s']){
		CAMERA_POSITION.z -= WALKING_SPEED*DELTA_TIME;
	}
	if (KEYS['a']){
		CAMERA_POSITION.x -= WALKING_SPEED*DELTA_TIME;
	}
	if (KEYS['d']){
		CAMERA_POSITION.x += WALKING_SPEED*DELTA_TIME;
	}
	if (KEYS['q']){
		CAMERA_POSITION.y += WALKING_SPEED*DELTA_TIME;
	}
	if (KEYS['e']){
		CAMERA_POSITION.y -= WALKING_SPEED*DELTA_TIME;
	}
   	if (KEYS['n']){
		graphTime += 0.001; 
	}	
   	if (KEYS['b']){
		graphTime += 0.001;
	}
    if (KEYS['1']){
        control[CTRLI][CTRLJ][0]+=0.01;
    }
    if (KEYS['2']){
        control[CTRLI][CTRLJ][0]-=0.01;
    }
    if (KEYS['3']){
        control[CTRLI][CTRLJ][1]+=0.01;
    }
    if (KEYS['4']){
        control[CTRLI][CTRLJ][1]-=0.01;
    }
    if (KEYS['5']){
        control[CTRLI][CTRLJ][2]+=0.01;
    }
    if (KEYS['6']){
        control[CTRLI][CTRLJ][2]-=0.01;
    }
	// Process Mouse and Highlight Keys
	// if no highlight keys are pressed, rotate box normally
	if (leftButton && ! KEYS['x'] && ! KEYS['y'] && ! KEYS['z']){
		CAMERA_ROTATION.y += (float)MOUSE_DELTA_X*MOUSE_SENSITIVITY;
		CAMERA_ROTATION.x += (float)MOUSE_DELTA_Y*MOUSE_SENSITIVITY;
	}
    
}

//实现控制点移动
void SpecialKeys(int key,int x,int y)
{
    if(key==GLUT_KEY_UP)
    {
        if(CTRLI>0)
        {
            CTRLI--;
        }
    }
    if(key==GLUT_KEY_DOWN)
    {
        if(CTRLI<3)
        {
            CTRLI++;
        }
    }
    if(key==GLUT_KEY_LEFT)
    {
        if(CTRLJ>0)
        {
            CTRLJ--;;
        }
    }
    if(key==GLUT_KEY_RIGHT)
    {
        if(CTRLJ<3)
        {
            CTRLJ++;
        }
    }
    
}
void reshape(int screenDisplayWidth, int screenDisplayHeight){
	if (screenDisplayHeight == 0){
		screenDisplayHeight = 1;
	}
	float aspectRatio = (float)screenDisplayWidth / (float)screenDisplayHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // reset the identity matrix
    
	//glViewport(0, 0, screenDisplayWidth, screenDisplayHeight);

	gluPerspective(45, aspectRatio, 0.001, 1000); // zNear something small/close zFar something big/Far
	glMatrixMode(GL_MODELVIEW);
}

void display(){
    
    preprocessEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glLoadIdentity();
    gluLookAt(3*r*cos(c*du), 3*h, 3*r*sin(c*du), 0, 0, 0, 0, 1, 0);
    
    glPushMatrix();
	glTranslatef( CAMERA_POSITION.x, CAMERA_POSITION.y,CAMERA_POSITION.z);
	glRotatef(CAMERA_ROTATION.x, 1, 0, 0);
	glRotatef(CAMERA_ROTATION.z, 0, 0, 1);
	glRotatef(CAMERA_ROTATION.y, 0, 1, 0);
    
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &control[0][0][0]);
    //允许二维映射
    glEnable(GL_MAP2_VERTEX_3);
    //二维映射：x、y方向U和V的参数[0, 1]，且中间插值数量为各20个
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    //允许深度测试
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    BezierSuface(3, 3);
    glPointSize(8.0);
    glColor3f(0.3, 0.5, 0.8);
    glBegin(GL_POINTS);
        glVertex3f(control[CTRLI][CTRLJ][0],control[CTRLI][CTRLJ][1],control[CTRLI][CTRLJ][2]);
    glEnd();
    //外部正方体
	DisplayBox onScreenDisplayBox;
	onScreenDisplayBox.setUnitRange(1.0);
	onScreenDisplayBox.drawDisplayBox();
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    glPopMatrix();
	glutSwapBuffers();
}

