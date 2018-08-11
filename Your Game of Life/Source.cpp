#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
using namespace std;

int WINDOW_WIDTH, WINDOW_HEIGHT, STEP, SIZE, TIMER_PERIOD;

#define D2R 0.0174532

bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

enum states {START, LIFE};
states game = START;
bool **cells, **newCells;

int inBound(int x) {
	if (x == -1)
		return SIZE - 1;
	if (x == SIZE)
		return 0;
	return x;
}

int countNeighbours(bool c, int x, int y) {
	int xs[3], ys[3], n = 0;
	for (int i = 0; i < 3; i++) {
		xs[i] = inBound(x - 1 + i);
		ys[i] = inBound(y - 1 + i);
	}
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (cells[xs[i]][ys[j]] && !(xs[i] == x && ys[j] == y))
				n++;
	return n;
}

void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
// To display onto window using OpenGL commands
//
void display() {

	//
	// clear window to while
	//
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0, 0, 0);
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			if (cells[i][j])
				glRectf(-WINDOW_HEIGHT / 2 + j * STEP, WINDOW_HEIGHT / 2 - i * STEP,
					-(WINDOW_HEIGHT / 2 - STEP) + j * STEP, WINDOW_HEIGHT / 2 - STEP - i * STEP);

	// Grid
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_LINES);
	for (int i = 0, j = 0; i <= WINDOW_WIDTH, j <= WINDOW_HEIGHT; i += STEP, j += STEP) {

		// Vertical
		glVertex2f(WINDOW_WIDTH / (-2) + i, WINDOW_HEIGHT / 2);
		glVertex2f(WINDOW_WIDTH / (-2) + i, WINDOW_HEIGHT / (-2));

		// Horizontal
		glVertex2f(WINDOW_WIDTH / (-2), WINDOW_HEIGHT / 2 - j);
		glVertex2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - j);

	}
	glEnd();

	glutSwapBuffers();

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	// start/pause "life" when ENTER is pressed.
	if (key == 13)
		if (game == START)
			game = LIFE;
		else
			game = START;
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.

	if(game == START)
		cells[y / STEP][x / STEP] = 1;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void arrCpy(bool **a1, bool **a2) {
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			a1[i][j] = a2[i][j];
}

// Function that checks all life/death conditions and applies them.
// i and j are the coordinates of the cell
void life(int i, int j) {

	int n = countNeighbours(cells[i][j], i, j);

	/*
		TODO: Write instructions
	*/

	if (cells[i][j]) {
		if (n < 2 || n>3)
			newCells[i][j] = false;
	}

	else {
		if (n == 3)
			newCells[i][j] = true;
	}
}

void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (game == LIFE) {
		arrCpy(newCells, cells);
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++) {
				life(i, j);
			}
		arrCpy(cells, newCells);
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize cells to be alive like cells[pos1][pos2] = 1

}

void setInitialData() {
	printf("Welcome to \"Your Game of Life\"\nPlease enter required data to proceed.\n\nPlease enter desired window size: ");
	scanf("%d %d", &WINDOW_WIDTH, &WINDOW_HEIGHT);
	printf("Enter the size of the cell: ");
	scanf("%d", &STEP);
	SIZE = WINDOW_HEIGHT / STEP;
	printf("Enter the timer period in ms: ");
	scanf("%d", &TIMER_PERIOD);
	cells = (bool **)malloc(SIZE * sizeof(bool *));
	newCells = (bool **)malloc(SIZE * sizeof(bool *));
	for (int i = 0; i < SIZE; i++) {
		cells[i] = (bool *)malloc(SIZE * sizeof(bool));
		newCells[i] = (bool *)malloc(SIZE * sizeof(bool));
	}
	for(int i = 0 ; i<SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			cells[i][j] = 0;
}

void main(int argc, char *argv[]) {

	setInitialData();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Your Game of Life");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	Init();

	glutMainLoop();
}