#include <iostream>
#include <Windows.h>
#include <GL/glut.h>
using namespace std;

class Point {
	float x, y;
public:
	Point(): x(0.0), y(0.0){}
	Point(float x, float y) : x(x), y(y) {}
	float getX() const {
		return x;
	}
	float getY() const {
		return y;
	}
	void setX(float x) {
		this->x = x;
	}
	void setY(float y) {
		this->y = y;
	}
	static Point getRightMostPoint(Point p1, Point p2) {
		return (p1.getX() >= p2.getX()) ? p1 : p2;
	}
	static Point getLeftMostPoint(Point p1, Point p2) {
		return (p1.getX() < p2.getX()) ? p1 : p2;
	}
};

const Point topLeft{ 300, 200 };
const Point bottomRight{ -200, -100 };
Point start;
Point last;

class ABRL {
	int a, b, r, l;
public:
	ABRL(bool a, bool b, bool r, bool l) : a(a), b(b), r(r), l(l) {}
	ABRL(Point point) :
		a(point.getY() > topLeft.getY()),
		b(point.getY() < bottomRight.getY()),
		r(point.getX() > topLeft.getX()),
		l(point.getX() < bottomRight.getX()) {}
	int getCodeInDecimal() {
		return a * 8 + b * 4 + r * 2 + l;
	}
	int getCodeInBinary() {
		return a * 1000 + b * 100 + r * 10 + l;
	}
};

float getUserInput(string);
void validateInput(string, float&);
void init();
void drawRectangle();
void clipAndDrawLine();
bool clipLine();
void actualClipping(Point&);
void drawLine();

int main(int argc, char** argv) {
	cout << "Start Point." << endl << endl;
	start.setX(getUserInput("Input x: "));
	start.setY(getUserInput("Input y: "));
	cout << endl << "Last Point." << endl << endl;
	last.setX(getUserInput("Input x: "));
	last.setY(getUserInput("Input y: "));

	glutInit(&argc, argv);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Cohen Sutherland Line Clipping Algorithm!");

	glClearColor(1.0, 1.0, 1.0, 1.0);
	gluOrtho2D(-400, 400, -400, 400);
	glutDisplayFunc(init);
	glutMainLoop();
}

float getUserInput(string message) {
	float temp;
	cout << message;
	cin >> temp;
	validateInput(message, temp);
	return temp;
}

void validateInput(string message, float& temp) {
	while (cin.fail()) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Invalid Input Provided!" << endl << "Please input a number." << endl << endl << message;
		cin >> temp;
	}
	cin.ignore(INT_MAX, '\n');
}

void init() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawRectangle();
	clipAndDrawLine();
	glFlush();
}

void drawRectangle() {
	glColor3f(0.0, 0.6, 0.0);
	glLineWidth(4.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(bottomRight.getX(), topLeft.getY());
	glVertex2f(bottomRight.getX(), bottomRight.getY());
	glVertex2f(topLeft.getX(), bottomRight.getY());
	glVertex2f(topLeft.getX(), topLeft.getY());
	glEnd();
}

void clipAndDrawLine() {
	if (clipLine()) {
		drawLine();
	}
}

void drawLine() {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(start.getX(), start.getY());
	glVertex2f(last.getX(), last.getY());
	glEnd();

}

bool clipLine() {
	ABRL startPoint{ start };
	ABRL endPoint{ last };
	int code;
	if ((startPoint.getCodeInDecimal() & endPoint.getCodeInDecimal()) != 0) return false;
	if (startPoint.getCodeInDecimal() == 0 && endPoint.getCodeInDecimal() == 0) return true;
	if (startPoint.getCodeInDecimal() != 0) {
		actualClipping(start);
		return clipLine();
	}
	if (endPoint.getCodeInDecimal() != 0) {
		actualClipping(last);
		return clipLine();
	}
}

void actualClipping(Point& line) {
	ABRL abrl{ line };
	int code = abrl.getCodeInDecimal();
	float gradient = (start.getY() - last.getY()) / (start.getX() - last.getX());
	float c = start.getY() - gradient * start.getX();
	switch (code) {
		case 8:
		case 10:
		case 9:
			line.setX((topLeft.getY() - c) / gradient);
			line.setY(topLeft.getY());
			break;
		case 4:
		case 5:
		case 6:
			line.setX((bottomRight.getY() - c) / gradient);
			line.setY(bottomRight.getY());
			break;
		case 2:
			line.setX(topLeft.getX());
			line.setY(gradient * topLeft.getX() + c);
			break;
		case 1:
			line.setX(bottomRight.getX());
			line.setY(gradient * bottomRight.getX() + c);
			break;
	}
}