#include <GL/GLUT.H>
#include <iostream>
#include <math.h>
 
using namespace std;

void BresenhamCircle(int x0, int y0, int r)
{
	int x = 0;
	int y = r;
	int d = 2 * (1 -r);
	int d1 = 0;
	int d2 = 0;
	int direction;
	while (y >= 0)
	{
		glVertex2i(x0+x, y0+y);
		glVertex2i(x0+x, y0-y);
		glVertex2i(x0-x, y0-y);
		glVertex2i(x0-x, y0+y);
		if (d < 0)
		{
			d1 = 2 *(d + y) - 1;
			if (d1 < 0)
			{
				direction = 1;
			}
			else
			{
				direction = 2;
			}
		}
		else if (d > 0)
		{
			d2 = 2 * (d - x) -1;
			if (d < 0)
			{
				direction = 2;
			}
			else
			{
				direction = 3;
			}
		}
		else
			direction = 2;
		switch (direction)
		{
		case 1:
			x++;
			d += 2 * x + 1;
			break;
		case 2:
			x++;
			y--;
			d += 2 * (x - y + 1);
			break;
		case 3:
			y--;
			d += (-2 * y + 1);
			break;
		}
	}
}

void MidCircle(int x0, int y0, int r)
{
	int x=x0, y=y0+r, d, deltax, deltay;
	deltax=3;
	deltay=2-r-r;
	d=1-r;
	glVertex2i(x,y);
	while(x<y)
	{
		if(d<0)
		{
			d+=deltax;
			deltax+=2;
			x++;
		}
		else
		{
			d+=(deltax+deltay);
			deltax+=2;
			deltay+=2;
			x++;
			y--;
		}
		glVertex2i(x,y);
		glVertex2i(y,x);
		glVertex2i(x,y0+y0-y);
		glVertex2i(y0+y0-y,x);
		glVertex2i(x0+x0-x,y);
		glVertex2i(y,x0+x0-x);
		glVertex2i(x0+x0-x,y0+y0-y);
		glVertex2i(y0+y0-y,x0+x0-x);
	}
}
 
void CircleSegment()
{
	int xs = 0;
	int ys = 0;
	int r = 0;
	int st1 = 0;
	cout << "输入圆心坐标和半径（范围为0-500,0-500):";
	cin >> xs >> ys >> r;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_POINTS);
	cout << "输入绘图模式(1: Bresenham; 2: 中点): ";
	cin>>st1;
	switch (st1)
	{
	case 1:
		BresenhamCircle(xs, ys, r);
		break;
	case 2:
		MidCircle(xs,ys,r);
		break;
	default:
		break;
	}
	glEnd();
	glFlush();
}

void lineMidLine(int x0, int y0, int xEnd, int yEnd)
{
    float a, b, d1, d2, d, x, y;

    glClear(GL_COLOR_BUFFER_BIT);
    a = y0 - yEnd;
    b = xEnd - x0;
    d = 2 * a + b;
    d1 = 2 * a;
    d2 = 2 * (a + b);
    x = x0;
    y = y0;
    while (x < xEnd)
    {
        
        if (d < 0)
        {
  			glVertex2i (x, y);
            x++;
            y++;
            d += d2;

        }
        else
        {
   			glVertex2i (x, y);
            x++;
            d += d1;
        }
    }
   
}

void lineBres (int x0, int y0, int xEnd, int yEnd)
{
   int dx = fabs (xEnd - x0),  dy = fabs(yEnd - y0);
   int p = 2 * dy - dx;
   int twoDy = 2 * dy,  twoDyMinusDx = 2 * (dy - dx);
   int x, y;

   if (x0 > xEnd) {
      x = xEnd;
      y = yEnd;
      xEnd = x0;
   }
   else {
      x = x0;
      y = y0;
   }
   glVertex2i (x, y);

   while (x < xEnd) {
      x++;
      if (p < 0)
         p += twoDy;
      else {
         y++;
         p += twoDyMinusDx;
      }
      glVertex2i (x, y);
   }
}
 
void LineDDA(int x0, int y0, int x1, int y1)
{
	float x = 0.0;
	float y = 0.0;
	float m = 0.0;
	float dx = x1 - x0;
	float dy = y1 - y0;
	if (dx != 0)
	{
		m = dy/dx;
		if (m <= 1 && m >= -1)
		{
			y = y0;
			for (x = x0; x <= x1; x++)
			{
				glVertex2i(x, int(y+0.5));
				y += m;
			}
		}
		if (m>1 || m<-1)
		{
			m = 1/m;
			x = x0;
			for (y=y0; y<=y1; y++)
			{
				glVertex2i(int(x+0.5), y);
				x += m;
			}
		}
	}
	else
	{
		int x = x0;
		int y = 0;
		y = (y0 <= y1) ? y0 : y1;
		int d = fabs((double) (y0 - y1));
		while (d >= 0)
		{
			glVertex2i(x, y);
			y++;
			d--;
		}
	}
}
 
void lineSegment()
{
	int st1;
	float xs = 0.0;
	float ys = 0.0;
	float xe = 0.0;
	float ye = 0.0;
	cout << "输入线段起始和终点坐标（范围为 0 - 500, 0-500）: ";
	cin >> xs >> ys >> xe >> ye;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	cout << "输入绘图模式(1: DDA; 2: Bresenham; 3: 中点): ";
	cin>>st1;
	switch (st1)
	{
	case 1:
		LineDDA(xs, ys, xe, ye);
		break;
	case 2:
		lineBres(xs, ys, xe, ye);
		break;
	case 3:
		lineMidLine(xs, ys, xe, ye);
		break;
	default:
		break;
	}
	glEnd();
	glFlush();
}

int main(int argc, char *argv[])
{
	int st = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("计算机图形学");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 500, 0.0, 500.0);
	cout << "输入绘图模式(1: 画线, 2: 画圆): ";
	cin>>st;
	switch (st)
	{
	case 1:
		glutDisplayFunc(lineSegment);
		break;
	case 2:
		glutDisplayFunc(CircleSegment);
	default:
		break;
	}
	glutMainLoop();
	return 0;
}