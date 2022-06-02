#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")
#pragma warning (disable:4996)
#pragma warning (disable:4305)

#define WINDOW_TITLE "OpenGL Window"

// Utility Variable
int pc = 0, pcTotal = 0; // polygon count
int cubeC = 0, cubeTotal = 0;
int pyramidC = 0, pyramidTotal = 0;
int sphereC = 0, sphereTotal = 0;
int cylinderC = 0, cylinderTotal = 0;
int coneC = 0, coneTotal = 0;
int quadC = 0, quadTotal = 0;
int triangleC = 0, triangleTotal = 0;
int circleC = 0, circleTotal = 0;
int lineC = 0, lineTotal = 0;

boolean pcCount = true;  // stop counting
int vs = 1;										// variable selected (1-5, a-e)
float va = 0, vb = 0, vc = 0, vd = 0, ve = 0, vf = 0, vg = 0, vh = 0; 	// variable to use
float vadjust = 0.1;							// add or minus how much for variable
boolean showGuideLine = false;

// Projection
float cx = 0, cy = 0, cr = 0;					// camera up, down, rotate
boolean isOrtho = true;							// view type (ortho / perspective)

// Rotate Object & Zoom
int objRotateAxis = 1;
float objRotateX = 0, objRotateY = 0, objRotateZ = 0;
float zoom = 0;

// Lighting
int onLight = 0;
float amb[] = { 1.0, 1.0, 1.0 };		// Ambient Light
float pos[] = { 0.0, 0.0, 10.0 };		// Position
float diff[] = { 1.0, 1.0, 1.0 };		// Diffuse Light

float red[] = { 1.0, 0.0, 0.0 };
float yellow[] = { 1.0, 1.0, 0.0 };
float white[] = { 1.0, 1.0, 1.0 };
float black[] = { 0.0,0.0,0.0 };

float crystalBlue[] = { 0.012, 0.784, 0.992 };
float darkOliveGreen[] = { 0.604, 0.608, 0.545 };
float eyeBlue[] = { 0.135, 0.457, 0.853 };

float swordBase[] = { 0.082, 0.082, 0.082 };
float swordSharpBase[] = { 0.773, 0.082, 0.0 };
float swordSharp[] = { 0.965, 0.373, 0.082 };
float swordGreen[] = { 0.5, 1.0, 0.5 };

float grey[] = { 0.2, 0.2, 0.2 };
float grey1[] = { 0.1, 0.1, 0.1 };
float grey2[] = { 0.2, 0.2, 0.2 };
float grey3[] = { 0.3, 0.3, 0.3 };
float grey4[] = { 0.4, 0.4, 0.4 };
float grey5[] = { 0.5, 0.5, 0.5 };
float grey6[] = { 0.6, 0.6, 0.6 };
float grey7[] = { 0.7, 0.7, 0.7 };
float grey8[] = { 0.8, 0.8, 0.8 };
float grey9[] = { 0.9, 0.9, 0.9 };

// Texture
BITMAP BMP;				// bitmap structure
HBITMAP hBMP = NULL;	// bitmap handle
int isTexture = 0;
GLuint textures[10];

//goh's variable
bool isWalking = false, isTop = false, isCannon = false, isShooting = false;
float walkRotate = 0, walkx = 0;
float legRotatey = 0;
float walktx = 0;
float lbRotatey = 0;
float cannonRotate = false, cannony = 0, cannontz = 0, bulletz = 0;
float vi = 0, rx = 0, ry = 0, rz = 0, radjust = 1;

// so part----------------------------------------------------------------------
// Animation
int playAnimation = 0;
float animSpeed = 5;
float animDelay = 0;

// Head Animation
float headRotateDegree = 0.0;

// Sword Animation
float swordMachine = 0;
float swordGlow = 0, swordGlowSpeed = 0.001;

// Arm Animation
int armAnimState = 1;
float armLift = 0, armOpen = 0;							// Upper arm
float armFlex = 0, armRotate = 0, armInOut = 0;			// Fore arm
float handFlex = 0, handRotate = 0, handTilt = 0;		// Hand
float iB = 0, mB = 0, rB = 0, pB = 0, tB = 0, tBX = 0;	// Finger

float lArmLift = 0, lArmOpen = 0;							// Upper arm
float lArmFlex = 0, lArmRotate = 0, lArmInOut = 0;			// Fore arm
float lHandFlex = 0, lHandRotate = 0, lHandTilt = 0;		// Hand
float lIB = 0, lMB = 0, lRB = 0, lPB = 0, lTB = 0, lTBX = 0;	// Finger

void clearArm()
{
	armLift = armOpen = armFlex = armRotate = armInOut = handFlex = handRotate = handTilt = tBX = 0;
	iB = mB = rB = pB = tB = 90;
	lArmLift = lArmOpen = lArmFlex = lArmRotate = lArmInOut = lHandFlex = lHandRotate = lHandTilt = lTBX = 0;
	lIB = lMB = lRB = lPB = lTB = 0;
	armAnimState = 1;
}
// so part end----------------------------------------------------------------------

//chee foo part-----------------------------------------------------------------------
float heartRotate = 0;
float wingLeftRotate = -50, wingRightRotate = 50, wing2LeftRotate = -20, wing2RightRotate = 20, wingRotateSpeed = 3;
float wingLeftRotateY = 45, wingRightRotateY = -45, wingRotateSpeedY = 2;
boolean wingOn = false;
float narutoRotate = 0;
float upperBodyRotate = 0;
// chee foo end----------------------------------------------------------------------

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == 0x20)
		{
			rx = ry = rz = cx = cy = cr = objRotateX = objRotateY = objRotateZ = legRotatey = lbRotatey = headRotateDegree = upperBodyRotate = 0;
			playAnimation = 0;
			armAnimState = 1;
			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 0;
		}
		else if (wParam == 0x31)	// 1
			vs = 1;
		else if (wParam == 0x32)	// 2
			vs = 2;
		else if (wParam == 0x33)	// 3
			vs = 3;
		else if (wParam == 0x34)	// 4
			vs = 4;
		else if (wParam == 0x35)	// 5
			vs = 5;
		else if (wParam == 0x36)	// 6
			vs = 6;
		else if (wParam == 0x37)	// 7
			vs = 7;
		else if (wParam == 0x38)	// 8
			vs = 8;
		else if (wParam == 0x39)	// 9
			vs = 9;
		else if (wParam == 0x41) // A
			cx -= vadjust;
		else if (wParam == 0x44) // D
			cx += vadjust;
		else if (wParam == 0x57) // W
			cy += vadjust;
		else if (wParam == 0x53) // S
			cy -= vadjust;
		else if (wParam == 0x46) // F
			cr -= 5;
		else if (wParam == 0x48) // H
			cr += 5;
		else if (wParam == 0x56) // V
			isOrtho = !isOrtho;
		else if (wParam == 'T') {
			if (isTexture == 2)
				isTexture = 0;
			else
				isTexture += 1;
		}
		else if (wParam == 0x51) // Q
		{
			switch (objRotateAxis)
			{
			case 1:
				objRotateX -= 10;
				break;
			case 2:
				objRotateY -= 10;
				break;
			case 3:
				objRotateZ -= 10;
				break;
			}
		}
		else if (wParam == 0x45) // E
		{
			switch (objRotateAxis)
			{
			case 1:
				objRotateX += 10;
				break;
			case 2:
				objRotateY += 10;
				break;
			case 3:
				objRotateZ += 10;
				break;
			}
		}
		else if (wParam == 0x52) // R
		{
			if (objRotateAxis == 1)
				objRotateAxis = 2;
			else if (objRotateAxis == 2)
				objRotateAxis = 3;
			else if (objRotateAxis == 3)
				objRotateAxis = 1;
		}
		else if (wParam == 0x30)
			showGuideLine = !showGuideLine;
		else if (wParam == VK_UP)	// Up
			vadjust *= 10;
		else if (wParam == VK_DOWN)	// Down
			vadjust /= 10;
		else if (wParam == VK_LEFT)
		{
			switch (vs)	// variable selection
			{
			case 1:
				if (headRotateDegree > -45)
					headRotateDegree -= 5;
				break;
			case 2: {
				if (upperBodyRotate > -20)
					upperBodyRotate -= 0.5;
				}
				break;
			case 3:
				if (!isWalking && legRotatey > -2) {
					legRotatey -= 2;
				}
				break;
			case 4:
				lbRotatey -= 1;
				break;
			case 5:
				if (onLight && pos[0] > -10) {
					pos[0] -= 1;
				}
				break;
			case 6:
				if (onLight && pos[1] > -10) {
					pos[1] -= 1;
				}
				break;
			case 7:
				if (onLight && pos[2] > -10) {
					pos[2] -= 1;
				}
				break;
			default:
				;
			}
		}
		else if (wParam == VK_RIGHT)
		{
			switch (vs)	// variable selection
			{
			case 1:
				if (headRotateDegree < 45)
					headRotateDegree += 5;
				break;
			case 2: {
				if (upperBodyRotate < 20)
					upperBodyRotate += 0.5;
			}
				break;
			case 3:
				if (!isWalking) {
					legRotatey += 2;
				}
				break;
			case 4:
				lbRotatey += 1;
				break;
			case 5:
				if (onLight && pos[0] < 10) {
					pos[0] += 1;
				}
				break;
			case 6:
				if (onLight && pos[1] < 10) {
					pos[1] += 1;
				}
				break;
			case 7:
				if (onLight && pos[2] < 10) {
					pos[2] += 1;
				}
				break;
			default:
				;
			}
		}
		else if (wParam == 0x4B) // K
		{
			isWalking = false;
			isCannon = false;
			clearArm();
			playAnimation = 1;
		}
		else if (wParam == 0x4C) { // L
		if (onLight == 2) {
			onLight = 0;
			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 10;
		}
		else
			onLight += 1;
		}
		else if (wParam == 0x4F) // O
			zoom > -0.5 ? zoom -= 0.05 : NULL;
		else if (wParam == 0x50) // P
			zoom < 0.5 ? zoom += 0.05 : NULL;
		else if (wParam == 'M')
		{
			isWalking = !isWalking;
			if (isWalking)
			{
				if (!isCannon)
				{
					clearArm();
					playAnimation = 2;
				}
			}
			else
			{
				if (!isCannon)
				{
					clearArm();
					playAnimation = 0;
				}
			}
		}
		else if (wParam == 'C')
		{
			isCannon = !isCannon, wingOn = true;
			if (isCannon)
			{
				clearArm();
				playAnimation = 3;
			}
			else
			{
				if (playAnimation == 3)
				{
					armAnimState = 2;
				}
				else
				{
					clearArm();
					playAnimation = 0;
				}
			}
		}
		else if (wParam == 0x4E) { // N
			lbRotatey = 0;
			legRotatey = 0;
			upperBodyRotate = 0;
			wingOn = !wingOn;
		}
		else if (wParam == 0x23) {
		if (upperBodyRotate > -20)
			upperBodyRotate -= 0.5;
		}
		else if (wParam == 0x24) {
		if (upperBodyRotate < 20)
			upperBodyRotate += 0.5;
		}
		else if (wParam == 'X')
			isShooting = !isShooting;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

/** Utility **/
void guideLine()
{
	glEnable(GL_DEPTH_TEST);

	// X Positive
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(10.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	// X Negative
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-10.0f, 0.0f, 0.0f);
	glEnd();

	// Y Positive
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0f, 10.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	// Y Negative
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -10.0f, 0.0f);
	glEnd();

	// Z Positive
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0f, 0.0f, 10.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	// Z Negative
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -10.0f);
	glEnd();
}

/** Projection **/
void Projection() {
	glMatrixMode(GL_PROJECTION);	// Refer to projection matrix
	glLoadIdentity();		// Reset projection matrix
	glTranslatef(cx, cy, 0);
	glRotatef(cr, 0.0, 1.0, 0.0);

	if (isOrtho)
	{
		// Viewing volume left right bottom top near far
		glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 55.0);	// match the 800x600 window size
	}
	else {
		// Perspective 
		// fovy, aspect, near, far
		glRotatef(180, 0.0, 1.0, 0.0);
		gluPerspective(20.0, 1.0, -1, 1.0); // third param = camera position at -1.
		// xmin, xmax, ymin, ymax, -zmin, -zmax
		glFrustum(-10.0, 10.0, -10.0, 10.0, 20, 30.0); // follow the ortho one (last param cannot behind camera)
	}
	glMatrixMode(GL_MODELVIEW);
}

/** Primitives **/

void drawCube(int i)
{
	cubeC++;
	// Base
	pc++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5, -0.5, -0.5);
	glEnd();

	// Top
	pc++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5, 0.5, -0.5);
	glEnd();

	// Left
	pc++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	// Right
	pc++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5, 0.5, 0.5);
	glEnd();

	// Back
	pc++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5, 0.5, -0.5);
	glEnd();

	// Front
	pc++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5, 0.5, 0.5);
	glEnd();
}

void drawPyramid(int i)
{
	pyramidC++;
	// Base
	pc++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	glEnd();

	// Front
	pc++;
	if (i == 1)
	{
		triangleC++;
		glBegin(GL_TRIANGLES);
	}
	else
	{
		lineC++;
		glBegin(GL_LINE_LOOP);
	}

	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0.5, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	// Left
	pc++;
	if (i == 1)
	{
		triangleC++;
		glBegin(GL_TRIANGLES);
	}
	else
	{
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0.5, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	// Right
	pc++;
	if (i == 1)
	{
		triangleC++;
		glBegin(GL_TRIANGLES);
	}
	else
	{
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0.5, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	// Back
	pc++;
	if (i == 1)
	{
		triangleC++;
		glBegin(GL_TRIANGLES);
	}
	else
	{
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
}

void drawSphere(float rad, int sl, int st, int t)
{
	sphereC++;
	pc++;
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	if (t == 1)
		gluQuadricDrawStyle(sphere, GLU_LINE);
	else if (t == 2)
		gluQuadricDrawStyle(sphere, GLU_FILL);
	else
		gluQuadricDrawStyle(sphere, GLU_POINT);
	gluQuadricTexture(sphere, true);
	gluSphere(sphere, rad, sl, st);
	gluDeleteQuadric(sphere);					// prevent memory burst
}

void drawCylinder(float rad, float h, int sl, int st, int t)
{
	cylinderC++;
	pc++;
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	if (t == 1)
		gluQuadricDrawStyle(cylinder, GLU_LINE);
	else
		gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, true);
	gluCylinder(cylinder, rad, rad, h, sl, st);
	gluDeleteQuadric(cylinder);					// prevent memory burst
}

void drawCone(float bRad, float h, int sl, int st, int t)
{
	coneC++;
	pc++;
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	if (t == 1)
		gluQuadricDrawStyle(cylinder, GLU_LINE);
	else
		gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, true);
	gluCylinder(cylinder, bRad, 0, h, sl, st);
	gluDeleteQuadric(cylinder);					// prevent memory burst
}

void drawDisk(float iRad, float oRad, int sl, int loops, int t)
{
	pc++;
	GLUquadricObj* disk = NULL;
	disk = gluNewQuadric();
	if (t == 1)
		gluQuadricDrawStyle(disk, GLU_LINE);
	else
		gluQuadricDrawStyle(disk, GLU_FILL);
	gluQuadricTexture(disk, true);
	gluDisk(disk, iRad, oRad, sl, loops);

	gluDeleteQuadric(disk);					// prevent memory burst
}

// Partial disk
void drawPDisk(float iRad, float oRad, int sl, int loops, float stAngle, float swAngle, int t)
{
	pc++;
	GLUquadricObj* pdisk = NULL;
	pdisk = gluNewQuadric();
	if (t == 1)
		gluQuadricDrawStyle(pdisk, GLU_LINE);
	else
		gluQuadricDrawStyle(pdisk, GLU_FILL);
	gluQuadricTexture(pdisk, true);
	gluPartialDisk(pdisk, iRad, oRad, sl, loops, stAngle, swAngle);

	gluDeleteQuadric(pdisk);					// prevent memory burst
}

void twoSideTriangle(int i)
{
	// Base
	pc++;
	quadC++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, -1.0, -0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5, -1.0, -0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5, -1.0, 0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, -1.0, 0.5);
	glEnd();

	// Side Right
	pc++;
	quadC++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, -1.0, -0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.5, -1.0, 0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, 0.0, 0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.5);
	glEnd();

	// Side Left
	pc++;
	quadC++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.5, -1.0, -0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.5, -1.0, 0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, 0.0, 0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, 0.0, -0.5);
	glEnd();

	// Front
	pc++;
	if (i == 1)
	{
		triangleC++;
		glBegin(GL_TRIANGLES);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, -1.0, -0.5);
	glTexCoord2f(0.5, 1.0);
	glVertex3f(0.0, 0.0, -0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5, -1.0, -0.5);
	glEnd();

	// Back
	pc++;
	if (i == 1)
	{
		triangleC++;
		glBegin(GL_TRIANGLES);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, -1.0, 0.5);
	glTexCoord2f(0.5, 1.0);
	glVertex3f(0.0, 0.0, 0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5, -1.0, 0.5);
	glEnd();
}

void drawFullCylinder(float rad, float h, int t)
{
	drawCylinder(rad, h, 20, 20, t);
	circleC++;
	drawDisk(0.0, rad, 20, 20, t);

	glPushMatrix();
	glTranslatef(0.0, 0.0, h);
	circleC++;
	drawDisk(0.0, rad, 20, 20, t);
	glPopMatrix();
}

void drawDonut(float iRad, float oRad, float h, int t)
{
	drawDisk(iRad, oRad, 20, 20, t);
	drawCylinder(iRad, h, 20, 20, t);
	drawCylinder(oRad, h, 20, 20, t);
	glPushMatrix();
	glTranslatef(0.0, 0.0, h);
	drawDisk(iRad, oRad, 20, 20, t);
	glPopMatrix();
}

void drawRightTriangle(int i)
{
	// Front
	pc++;
	triangleC++;
	if (i == 1)
	{
		triangleC++;
		glBegin(GL_TRIANGLES);
	}
	else
	{
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.0, 0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -1.0, 0.5);
	glEnd();

	// Back
	pc++;
	triangleC++;
	if (i == 1)
	{
		triangleC++;
		glBegin(GL_TRIANGLES);
	}
	else
	{
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.0, -0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -1.0, -0.5);
	glEnd();

	// Top
	pc++;
	quadC++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.0, 0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 0.0, -0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.5);
	glEnd();

	// Slope Side
	pc++;
	quadC++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, -1.0, 0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.0, 0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 0.0, -0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -1.0, -0.5);
	glEnd();

	// Left Side
	pc++;
	quadC++;
	if (i == 1)
	{
		quadC++;
		glBegin(GL_QUADS);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		lineC++;
	}
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, 0.0, 0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, -1.0, 0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -1.0, -0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, 0.0, -0.5);
	glEnd();
}

/** Lighting **/
void lighting()
{
	// To move the light
	/*posA[0] = posD[0] = va;
	posA[1] = posD[1] = vb;
	posA[2] = posD[2] = vc;*/


	if (onLight != 0)
		glEnable(GL_LIGHTING);
	else {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}

	// Light 0
	if (onLight == 1) {
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glEnable(GL_LIGHT0);
	}

	// Light 1
	if (onLight == 2) {
		glDisable(GL_LIGHT0);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
		glLightfv(GL_LIGHT1, GL_POSITION, pos);
		glEnable(GL_LIGHT1);
	}


}

/** Texture **/
GLuint loadTexture(LPCSTR filename) {

	//step1
	GLuint texture = 0;

	//step 3
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION |
		LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	//step 4
	if (isTexture)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
		BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	//from step 5
	DeleteObject(hBMP);

	return texture;
}

/** Sky box **/
void skyBox(float r, int t) {
	textures[0] = loadTexture("night.bmp");
	pc++;
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	if (t == 1)
		gluQuadricDrawStyle(sphere, GLU_LINE);
	else if (t == 2)
		gluQuadricDrawStyle(sphere, GLU_FILL);
	else
		gluQuadricDrawStyle(sphere, GLU_POINT);
	gluQuadricTexture(sphere, true);
	glRotatef(-90, 1, 0, 0);
	if (isTexture == 0)
		glColor3f(0, 0, 0);
	else
		glColor3f(1, 1, 1);
	gluSphere(sphere, r, 30, 30);
	gluDeleteQuadric(sphere);
	glDeleteTextures(1, &textures[0]);
}

// Head

void headTopCore(int i, int j)
{
	// Base
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, 1.0, 1.35);
	glRotatef(180, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.3, 0.2, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	// Crystal
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.012, 0.784, 0.992), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, crystalBlue), textures[0] = loadTexture("lightblue.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("lightblue.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("lightblue.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	
	glPushMatrix();
	glTranslatef(0.0, 1.8, 1.2);
	glScalef(0.8, 1.2, 0.4);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Top
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, 2.56, 0.65);
	glScalef(1.0, 0.5, 2.0);
	drawCube(i);
	glPopMatrix();

	// Side
	glPushMatrix();	//right
	glTranslatef(0.45, 1.8, 0.9);
	glScalef(0.1, 1.2, 1.2);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	//left
	glTranslatef(-0.45, 1.8, 0.9);
	glScalef(0.1, 1.2, 1.2);
	drawCube(i);
	glPopMatrix();

	// Go down
	glPushMatrix();
	glTranslatef(0.0, 2.32, -0.35);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glScalef(0.49, 0.8, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.73, -1.53);
	glRotatef(39, 1.0, 0.0, 0.0);
	glScalef(1.0, 1.6, 0.1);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Head Deco Yellow
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 0.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	
	glPushMatrix();	//right
	glTranslatef(0.52, 2.57, 0.3);
	glRotatef(30, 1.0, 0.0, 0.0);
	glScalef(0.05, 0.5, 0.1);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	//left
	glTranslatef(-0.52, 2.57, 0.3);
	glRotatef(30, 1.0, 0.0, 0.0);
	glScalef(0.05, 0.5, 0.1);
	drawCube(i);
	glPopMatrix();
}

void rightEar(int i, int j)
{
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();	// top
	glTranslatef(1.9, 0.8, -0.1);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glScalef(0.4, 0.7, 0.8);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();	// bottom
	glTranslatef(1.9, 0.0, -0.1);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.4, 0.7, 0.8);
	drawRightTriangle(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey), textures[0] = loadTexture("black1.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();	// top
	glTranslatef(1.9, 0.8, -0.1);
	glRotatef(180, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glScalef(1.0, 0.4, 0.8);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();	// bottom
	glTranslatef(1.9, 0.0, -0.1);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(1.0, 0.4, 0.8);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();	//right
	glTranslatef(2.25, 0.4, -0.25);
	glScalef(0.1, 0.8, 1.7);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	//left
	glTranslatef(1.55, 0.4, -0.25);
	glScalef(0.1, 0.8, 1.7);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	if (i == 1)
	{
		glColor3f(0.604, 0.608, 0.545), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, darkOliveGreen);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(1.9, 0.4, 0.0);
	glScalef(0.6, 0.8, 0.1);
	drawCube(i);
	glPopMatrix();

	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey), textures[0] = loadTexture("black1.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(1.9, 0.4, -1.05);
	glScalef(0.6, 0.8, 0.1);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Ear Outer deco
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();	// right
	glTranslatef(2.3, 0.4, 0.25);
	glScalef(0.01, 0.8, 0.7);
	drawCube(i);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();	// right
	glTranslatef(-2.3, 0.4, 0.25);
	glScalef(0.01, 0.8, 0.7);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	if (i == 1)
	{
		glColor3f(0.563, 0.606, 0.688), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(2.32, 0.4, -0.25);
	glScalef(0.05, 0.7, 1.5);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.32, 0.4, -0.25);
	glScalef(0.05, 0.7, 1.5);
	drawCube(i);
	glPopMatrix();

	// Yelow Deco
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 0.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();	//right top
	glTranslatef(2.32, 0.83, -0.7);
	glScalef(0.05, 0.07, 0.3);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	//right bottom
	glTranslatef(2.32, -0.02, -0.7);
	glScalef(0.05, 0.07, 0.3);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	// left top
	glTranslatef(-2.32, 0.83, -0.7);
	glScalef(0.05, 0.07, 0.3);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	// left bottom
	glTranslatef(-2.32, -0.02, -0.7);
	glScalef(0.05, 0.07, 0.3);
	drawCube(i);
	glPopMatrix();

	// Yellow Deco
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();	// right
	glTranslatef(1.91, 0.71, -0.44);
	glScalef(0.4, 0.05, 1.0);
	drawCube(i);
	glPopMatrix();

	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();	//left
	glTranslatef(1.91, 0.08, -0.44);
	glScalef(0.4, 0.05, 1.0);
	drawCube(i);
	glPopMatrix();
}

void leftEar(int i, int j)
{
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();	// top
	glTranslatef(-1.9, 0.8, -0.1);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glScalef(0.4, 0.7, 0.8);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();	// bottom
	glTranslatef(-1.9, 0.0, -0.1);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.4, 0.7, 0.8);
	drawRightTriangle(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey), textures[0] = loadTexture("black1.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();	// top
	glTranslatef(-1.9, 0.8, -0.1);
	glRotatef(180, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glScalef(1.0, 0.4, 0.8);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();	// bottom
	glTranslatef(-1.9, 0.0, -0.1);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(1.0, 0.4, 0.8);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();	//right
	glTranslatef(-2.25, 0.4, -0.25);
	glScalef(0.1, 0.8, 1.7);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	//left
	glTranslatef(-1.55, 0.4, -0.25);
	glScalef(0.1, 0.8, 1.7);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	if (i == 1)
	{
		glColor3f(0.604, 0.608, 0.545), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, darkOliveGreen);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-1.9, 0.4, 0.0);
	glScalef(0.6, 0.8, 0.1);
	drawCube(i);
	glPopMatrix();

	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey), textures[0] = loadTexture("black1.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-1.9, 0.4, -1.05);
	glScalef(0.6, 0.8, 0.1);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Yellow Deco
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 0.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();	// right
	glTranslatef(-1.91, 0.71, -0.44);
	glScalef(0.4, 0.05, 1.0);
	drawCube(i);
	glPopMatrix();

	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();	//left
	glTranslatef(-1.91, 0.08, -0.44);
	glScalef(0.4, 0.05, 1.0);
	drawCube(i);
	glPopMatrix();
}

void headAntenna(int i, int j)
{
	if (i == 1)
	{
		glColor3f(0.604, 0.608, 0.545), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, darkOliveGreen);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.7);
	drawFullCylinder(0.15, 0.7, 2);
	glPopMatrix();

	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.6);
	drawFullCylinder(0.17, 0.05, 2);
	glPopMatrix();

	if (i == 1)
	{
		glColor3f(0.604, 0.608, 0.545), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, darkOliveGreen);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, 0.0, -2.7);
	drawFullCylinder(0.1, 2, 2);
	glPopMatrix();
}

void head(int i, int j)
{
	glPushMatrix();
	glTranslatef(0.0, 1.2, 0.0);
	glRotatef(10, 1.0, 0.0, 0.0);

	// Head Base
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, -1.0, -0.46);
	glScalef(2.9, 0.1, 1.2);
	drawCube(i);
	glPopMatrix();

	// Eye
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.135, 0.457, 0.853), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, eyeBlue), textures[0] = loadTexture("blue.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("blue.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("blue.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, 0.35, 1.25);
	glScalef(1.0, 1.3, 0.5);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.35, 1.0);
	glRotatef(270, 1.0, 0.0, 0.0);
	glScalef(0.5, 0.5, 1.3);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0, 0.35, 0.5);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glScalef(0.5, 0.5, 1.3);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.35, 1.0);
	glRotatef(270, 1.0, 0.0, 0.0);
	glRotatef(270, 0.0, 0.0, 1.0);
	glScalef(0.5, 0.5, 1.3);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, 0.35, 0.5);
	glRotatef(270, 1.0, 0.0, 0.0);
	glRotatef(270, 0.0, 0.0, 1.0);
	glScalef(0.5, 0.5, 1.3);
	drawRightTriangle(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Mouth
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, -0.65, 1.3);
	glScalef(1.0, 0.7, 0.6);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -1.0, 1.6);
	glRotatef(180, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.4, 0.7, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, -0.65, 0.9);
	glRotatef(45, 0.0, 1.0, 0.0);
	glScalef(0.6, 0.69, 1.5);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.8, -0.65, 0.9);
	glRotatef(315, 0.0, 1.0, 0.0);
	glScalef(0.6, 0.69, 1.5);
	drawCube(i);
	glPopMatrix();

	// Mouth cover
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();	//right
	glTranslatef(1.54, -0.49, 0.5);
	glRotatef(10, 1.0, 0.0, 0.0);
	glScalef(0.1, 1.05, 1.4);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.34, -0.78, 1.66);
	glRotatef(20, 1.0, 0.0, 0.0);
	glRotatef(-20, 0.0, 1.0, 0.0);
	glScalef(0.1, 1.03, 1.2);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	//left
	glTranslatef(-1.54, -0.49, 0.5);
	glRotatef(10, 1.0, 0.0, 0.0);
	glScalef(0.1, 1.05, 1.4);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.34, -0.78, 1.66);
	glRotatef(20, 1.0, 0.0, 0.0);
	glRotatef(20, 0.0, 1.0, 0.0);
	glScalef(0.1, 1.03, 1.2);
	drawCube(i);
	glPopMatrix();

	// Head Top Base

	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glTranslatef(0.5, 1.1, 0.6);
	glRotatef(270, 1.0, 0.0, 0.0);
	glScalef(1.0, 1.05, 0.2);
	drawRightTriangle(i);
	glPopMatrix();

	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glTranslatef(-0.5, 1.1, 0.6);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glScalef(1.0, 1.05, 0.2);
	drawRightTriangle(i);
	glPopMatrix();

	// Head Top Front
	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glTranslatef(0.0, 1.7, 0.33);
	glRotatef(65, 1.0, 0.0, 0.0);
	glScalef(2.7, 0.1, 1.3);
	drawCube(i);
	glPopMatrix();

	// Head Top Side
	glPushMatrix();	//left
	glTranslatef(1.43, 1.68, -0.3);
	glScalef(0.2, 1.35, 1.4);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	//right
	glTranslatef(-1.43, 1.68, -0.3);
	glScalef(0.2, 1.35, 1.4);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	//left
	glTranslatef(1.43, 1.0, 0.4);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glScalef(1.35, 0.3, 0.2);
	drawRightTriangle(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	if (i == 1)
	{
		glColor3f(1.0, 1.0, 0.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(1.43, 1.75, 0.52);
	glRotatef(-12.8, 1.0, 0.0, 0.0);
	glScalef(0.1, 1.1, 0.1);
	drawCube(i);
	glPopMatrix();

	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey), textures[0] = loadTexture("black1.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();	//right
	glTranslatef(-1.43, 1.0, 0.4);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glScalef(1.35, 0.3, 0.2);
	drawRightTriangle(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	if (i == 1)
	{
		glColor3f(1.0, 1.0, 0.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow), textures[0];
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-1.43, 1.75, 0.52);
	glRotatef(-12.8, 1.0, 0.0, 0.0);
	glScalef(0.1, 1.1, 0.1);
	drawCube(i);
	glPopMatrix();

	// Head Top Ceiliing
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, 2.31, -0.45);
	glScalef(2.66, 0.01, 1.09);
	drawCube(i);
	glPopMatrix();

	// Head Top Antenna
	glPushMatrix();	// right
	glTranslatef(0.8, 2.38, 0.0);
	glRotatef(10, 1.0, 0.0, 0.0);
	glRotatef(-4, 0.0, 1.0, 0.0);
	headAntenna(i, j);
	glPopMatrix();

	glPushMatrix();	// right
	glTranslatef(-0.8, 2.38, 0.0);
	glRotatef(10, 1.0, 0.0, 0.0);
	glRotatef(4, 0.0, 1.0, 0.0);
	headAntenna(i, j);
	glPopMatrix();

	// Head Core
	headTopCore(i, j);

	// Ear
	rightEar(i, j);
	leftEar(i, j);

	// Head Side Cover
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey), textures[0] = loadTexture("black1.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();	//right
	glTranslatef(1.5, 0.4, -0.7);
	glScalef(0.1, 1.4, 2.5);
	drawCube(i);
	glPopMatrix();

	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();	//left
	glTranslatef(-1.5, 0.4, -0.7);
	glScalef(0.1, 1.4, 2.5);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	// bottom right
	glTranslatef(1.5, -0.65, -0.27);
	glScalef(0.1, 0.7, 1.55);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();	// bottom left
	glTranslatef(-1.5, -0.65, -0.27);
	glScalef(0.1, 0.7, 1.55);
	drawCube(i);
	glPopMatrix();

	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();	// top
	glTranslatef(0.0, 1.1, -0.99);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	glScalef(0.98, 1.22, 3.05);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();	// bottom
	glTranslatef(0.0, -0.3, -1.05);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.93, 0.7, 3.1);
	drawRightTriangle(i);
	glPopMatrix();

	// Back cover
	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glTranslatef(0.0, 0.4, -1.87);
	glScalef(3.05, 1.4, 0.2);
	drawCube(i);
	glPopMatrix();

	glPopMatrix();

	// Neck
	glPushMatrix();
	glTranslatef(0.0, 0.2, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(1, 1, 8, 20, 2);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);
}

// Head End

/** Sword **/

void swordFan(int ii, int j)
{
	// Center
	glPushMatrix();
	drawSphere(0.15, 20, 20, 2);
	glPopMatrix();

	// Blade

	for (int i = 0; i < 4; i++)
	{
		if (ii == 1)
		{
			if (isTexture == 0)
				glColor3f(1.0, 0.0, 0.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red), textures[0] = loadTexture("darkmatter1.bmp");
			else if (isTexture == 1)
				glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("darkmatter1.bmp");
			else
				glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("darkmatter1.bmp");
		}
		else
			glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

		glPushMatrix();
		glRotatef(i * 90, 0.0, 0.0, 1.0);
		glScalef(0.7, 0.7, 0.1);
		twoSideTriangle(ii);
		glPopMatrix();
		glDeleteTextures(1, &textures[0]);
	}
}

void swordCover(int i, int j)
{
	// Sword Fan Cover Top
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.5, 0.5, 0.5), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.2);
	drawDonut(0.7, 0.9, 0.1, j);
	glPopMatrix();

	// Sword Fan Top Middle Cover Circle
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.1);
	drawDonut(0.1, 0.25, 0.15, j);
	glPopMatrix();

	if (i == 1)
	{
		glColor3f(0.5, 1.0, 0.5), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, swordGreen);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.1);
	drawDonut(0.0, 0.1, 0.1, j);
	glPopMatrix();

	// Sword Fan Top Middle Cover Line
	if (i == 1)
	{
		glColor3f(0.5, 0.5, 0.5), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-0.7, 0.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.08, 1.6, j);
	glPopMatrix();

}

void swordTransformer(int i, int j)
{
	// Sword Fan
	glPushMatrix();
	glRotatef(swordMachine, 0.0, 0.0, 1.0);
	swordFan(i, j);
	swordMachine += 4;
	if (swordMachine > 36000)
		swordMachine = 0;
	glPopMatrix();

	// Sword Fan Side Cover
	if (i == 1)
	{
		glColor3f(0.3, 0.3, 0.3), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.2);
	drawDonut(0.8, 1.0, 0.4, j);
	glPopMatrix();

	// Sword Fan Cover
	swordCover(i, j);
	glPushMatrix();
	glRotatef(180, 0.0, 1.0, 0.0);
	swordCover(i, j);
	glPopMatrix();
}

void swordBlade(int i, int j)
{
	// Sword Blade Base
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.082, 0.082, 0.082), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5), textures[0] = loadTexture("black5.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black5.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black5.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(3.23, 0.2, 0);
	glScalef(5, 0.9, 0.2);
	drawCube(i);
	glPopMatrix();

	// Sword Blade Tip
	glPushMatrix();
	glTranslatef(5.72, 0.65, 0);
	glScalef(1.1, 1.18, 0.2);
	drawRightTriangle(i);
	glPopMatrix();

	// Sword Blade Lower
	glPushMatrix();
	glTranslatef(3.63, -0.4, 0.0);
	glScalef(4.2, 0.3, 0.2);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Sword Blade Sharp Base
	if (i == 1)
	{
		glColor3f(0.773 + swordGlow, 0.082 + swordGlow, 0.0 + swordGlow), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, swordSharpBase);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	swordSharpBase[0] = 0.773 + swordGlow;
	swordSharpBase[1] = 0.082 + swordGlow;
	swordSharpBase[2] = 0.0 + swordGlow;

	glPushMatrix();
	glTranslatef(3.65, -0.59, 0.0);
	glScalef(4.24, 0.1, 0.17);
	drawCube(i);
	glPopMatrix();

	// Sword Blade Sharp Base Front
	glPushMatrix();
	glTranslatef(6.3, 0.0, 0.0);
	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(1.71, 0.1, 0.17);
	drawCube(i);
	glPopMatrix();

	// Sword Blade Sharp
	if (i == 1)
	{
		glColor3f(0.965 + swordGlow, 0.373 + swordGlow, 0.082 + swordGlow), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, swordSharp);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	swordSharp[0] = 0.965 + swordGlow;
	swordSharp[1] = 0.373 + swordGlow;
	swordSharp[2] = 0.082 + swordGlow;

	glPushMatrix();
	glTranslatef(3.66, -0.76, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.11, 0.12, 4.27);
	twoSideTriangle(i);
	glPopMatrix();

	// Sword Blade Sharp Front
	glPushMatrix();
	glTranslatef(6.42, -0.13, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(315, 1.0, 0.0, 0.0);
	glScalef(0.11, 0.12, 1.74);
	twoSideTriangle(i);
	glPopMatrix();

	// Sword Blade Below Corner
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.082, 0.082, 0.082), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, swordBase), textures[0] = loadTexture("black5.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black5.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black5.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(1.54, -0.25, 0.0);
	glScalef(0.76, 0.3, 0.2);
	glRotatef(180, 0.0, 1.0, 0.0);
	drawRightTriangle(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	if (swordGlow > 0.2)
		swordGlowSpeed = -swordGlowSpeed;
	else if (swordGlow < 0)
		swordGlowSpeed = -swordGlowSpeed;

	swordGlow += swordGlowSpeed;
}

void swordBladeTop(int i, int j)
{
	// Blade Top Base
	if (i == 1)
	{
		glColor3f(0.7, 0.7, 0.7), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.6, 0.6, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.18, 2.36, j);
	glPopMatrix();

	// Blade Top Deco Right
	if (i == 1)
	{
		glColor3f(0.5, 1.0, 0.5), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, swordGreen);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.58, 0.65, 0.13);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.06, 2.12, j);
	glPopMatrix();

	// Blade Top Deco Left
	glColor3f(0.5, 1.0, 0.5);
	glPushMatrix();
	glTranslatef(0.58, 0.65, -0.13);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.06, 2.12, j);
	glPopMatrix();

	// Blade Top Up Deco
	if (i == 1)
	{
		glColor3f(0.6, 0.6, 0.6), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.4, 0.82, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.13, 1.22, j);
	glPopMatrix();
}

void swordBelowAltBlade(int i, int j)
{
	// Below Base
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.082, 0.082, 0.082), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, swordBase), textures[0] = loadTexture("black5.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black5.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black5.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0);
	glScalef(0.6, 0.2, 0.2);
	drawCube(i);
	glPopMatrix();

	// Below Front
	glPushMatrix();
	glTranslatef(0.0, -1.1, 0.0);
	glScalef(2.0, 0.4, 0.2);
	drawRightTriangle(i);
	glPopMatrix();

	// Below Back
	glPushMatrix();
	glTranslatef(0.0, -1.1, 0.0);
	glScalef(1.2, 0.4, 0.2);
	glRotatef(180, 0.0, 1.0, 0.0);
	drawRightTriangle(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Below Front Sharp Base
	if (i == 1)
	{
		glColor3f(0.773 + swordGlow, 0.082 + swordGlow, 0.0 + swordGlow), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, swordSharpBase);

	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	swordSharpBase[0] = 0.773 + swordGlow;
	swordSharpBase[1] = 0.082 + swordGlow;
	swordSharpBase[2] = 0.0 + swordGlow;
	glPushMatrix();
	glTranslatef(1.0, -1.34, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(348, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.1, 1.94);
	drawCube(i);
	glPopMatrix();

	// Below Back Sharp Base
	glPushMatrix();
	glTranslatef(-0.56, -1.35, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(270, 0.0, 1.0, 0.0);
	glRotatef(343, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.1, 1.34);
	drawCube(i);
	glPopMatrix();

	// Below Front Sharp
	if (i == 1)
	{
		glColor3f(0.965 + swordGlow, 0.373 + swordGlow, 0.082 + swordGlow), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, swordSharp);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	swordSharp[0] = 0.965 + swordGlow;
	swordSharp[1] = 0.373 + swordGlow;
	swordSharp[2] = 0.082 + swordGlow;

	glPushMatrix();
	glTranslatef(1.02, -1.46, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(348, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.1, 1.94);
	twoSideTriangle(i);
	glPopMatrix();

	// Below Back Sharp Base
	glPushMatrix();
	glTranslatef(-0.58, -1.46, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(270, 0.0, 1.0, 0.0);
	glRotatef(343, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.1, 1.37);
	twoSideTriangle(i);
	glPopMatrix();

}

void swordHandle(int i, int j)
{
	// Handle Base
	if (i == 1)
	{
		glColor3f(0.5, 0.5, 0.5), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-1.56, 0.0, 0.0);
	glScalef(1.0, 1.5, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.18, 0.6, j);
	glPopMatrix();

	// Handle Main
	if (i == 1)
	{
		glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-3.46, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.15, 1.9, j);
	glPopMatrix();

	// Handle Back
	if (i == 1)
	{
		glColor3f(1.0, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-3.81, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.15, 0.4, j);
	glPopMatrix();

	// Handle End
	if (i == 1)
	{
		glColor3f(0.6, 0.6, 0.6), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-4.07, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.22, 0.4, j);
	glPopMatrix();
}

void swordHandleDecoration(int i, int j)
{
	// Sword Handle Metal Thingy
	if (i == 1)
	{
		glColor3f(0.6, 0.6, 0.6), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-1.1, -0.7, 0.0);
	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(0.7, 0.15, 0.2);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.14, -0.92, 0.0);
	glScalef(1.7, 0.15, 0.2);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.96, -0.85, 0.0);
	glRotatef(270, 0.0, 0.0, 1.0);
	glScalef(0.14, 0.37, 0.2);
	drawRightTriangle(i);
	glPopMatrix();

	// Sword Handle Top Cylinder Thingy
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-1.0, 1.1, 0.0);
	glRotatef(315, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawDonut(0.12, 0.2, 0.5, j);
	glPopMatrix();

	if (i == 1)
	{
		glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-0.7, 0.8, 0.0);
	glRotatef(315, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawDisk(0.0, 0.15, 20, 20, j);
	glPopMatrix();
}

void swordCircuit(int i, int j)
{
	// Circuits
	if (i == 1)
	{
		glColor3f(0.773 + swordGlow, 0.082 + swordGlow, 0.0 + swordGlow), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, swordSharpBase);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	swordSharpBase[0] = 0.773 + swordGlow;
	swordSharpBase[1] = 0.082 + swordGlow;
	swordSharpBase[2] = 0.0 + swordGlow;
	glPushMatrix();
	glTranslatef(1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 1.4, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -0.23, 0.0);
	glRotatef(51, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.28, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.94, -0.21, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.6, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.38, 0.0, 0.0);
	glRotatef(304, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.7, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.5, -0.2, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.8, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.27, -0.18, 0.0);
	glRotatef(290, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.43, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.93, -0.17, 0.0);
	glRotatef(82, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.39, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.95, 0.2, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 2.36, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.29, 0.23, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.82, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.35, 0.15, 0.0);
	glRotatef(80, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.35, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.39, 0.48, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 2.15, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.5, 0.5, 0.0);
	glRotatef(-81, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.5, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 1.3, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.0, 0.0, 0.0);
	glRotatef(262.8, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.6, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.3, 0.0, 0.0);
	glRotatef(270, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.6, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.5, -0.2, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.8, j);
	glPopMatrix();

	// Circle Circuit
	glPushMatrix();
	glTranslatef(4.3, -0.2, 0.0);
	drawDonut(0.13, 0.2, 0.04, j);
	glPopMatrix();
}

void swordAltCircuit(int i, int j)
{
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.773 + swordGlow, 0.082 + swordGlow, 0.0 + swordGlow), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, swordSharpBase), textures[0] = loadTexture("lava5.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("lava5.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("lava5.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.07, -0.95, 0.0);
	glRotatef(244, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.32, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.04, -1.2, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.7, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.31, -1.22, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.23, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.62, -1.19, 0.0);
	glRotatef(332.5, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.04, 0.3, j);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);
}

void sword(int i, int j)
{
	// Sword Transformer
	swordTransformer(i, j);

	// Sword Blade
	glPushMatrix();
	glTranslatef(-0.2, 0.0, 0.0);
	glScalef(1.2, 1.0, 1.0);
	swordBlade(i, j);
	swordBladeTop(i, j);
	glPopMatrix();

	// Sword Below Alt Blade
	swordBelowAltBlade(i, j);

	// Sword Handle
	swordHandle(i, j);
	swordHandleDecoration(i, j);

	// Sword Circuit
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.09);
	swordCircuit(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.12);
	swordCircuit(i, j);
	glPopMatrix();

	// Sword Alt Circuit
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.09);
	swordAltCircuit(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.09);
	swordAltCircuit(i, j);
	glPopMatrix();

}

/** Hand **/

void fingerRotator(int i, int j)
{
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(-0.3, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFullCylinder(0.26, 0.6, j);
	glPopMatrix();
}

void fingerJoint(float length, int i, int j)
{
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.0, length / 2, 0.0);
	glScalef(0.7, length + 0.5, 0.6);
	drawCube(i);
	glPopMatrix();
}

void fingerJointConnect(float length, int i, int j)
{
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	fingerRotator(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.49, 0.0);
	fingerJoint(length, i, j);
	glPopMatrix();
	glPopMatrix();
}

void finger(float bend, float jointBaseLen, float jointMidLen, int i, int j)
{
	if (bend < 10)
		bend = 10;
	else if (bend > 90)
		bend = 90;

	// Finger Base
	glPushMatrix();
	glRotatef(bend, 1.0, 0.0, 0.0);
	fingerJointConnect(0.0 + jointBaseLen, i, j);

	// Finger Middle
	glPushMatrix();
	glTranslatef(0.0, 1.0 + jointBaseLen, 0.0);
	glRotatef(bend, 1.0, 0.0, 0.0);
	fingerJointConnect(0.0 + jointMidLen, i, j);

	// Finger Tip
	glPushMatrix();
	glTranslatef(0.0, 1.0 + jointMidLen, 0.0);
	glRotatef(bend, 1.0, 0.0, 0.0);
	fingerJointConnect(0.0, i, j);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void indexFinger(float bend, int i, int j)
{
	finger(bend, 0.2, 0.0, i, j);
}

void middleFinger(float bend, int i, int j)
{
	finger(bend, 0.5, 0.0, i, j);
}

void ringFinger(float bend, int i, int j)
{
	finger(bend, 0.3, 0.0, i, j);
}

void pinkie(float bend, int i, int j)
{
	finger(bend, 0.0, 0.0, i, j);
}

void thumb(float bend, float bendX, boolean isRight, int i, int j)
{
	// Bend X is to put in negative value if you wish to
	// flip your second joint thumb up, make a "like"
	// On normal circumstances, bendX = bend, unless bendX
	// is provided with negative value

	float thumbsUp = 0;
	// Thumbsup adjust the thumb so closer to index (more up)
	float fingerTipBend = 0;
	float fingerBaseBend = 350;

	if (bend < 10)
		bend = 10;
	else if (bend > 90)
	{
		bend = 90;
		bendX = 90;
	}

	if (bendX < 0)
	{
		thumbsUp = bendX / 3.2;
	}

	if (bendX >= 0)
		bendX = bend;

	if (bendX < 0)
		fingerTipBend = 0;
	else
		fingerTipBend = bendX;

	if (!isRight)
	{
		bend = -bend;
		bendX = -bendX;
		fingerTipBend = -fingerTipBend;
		fingerBaseBend = 10;
	}

	// Finger Base
	glPushMatrix();
	glTranslatef(0.0, -0.1, 0.7);
	glRotatef(thumbsUp + 10 + bend / 6, 1.0, 0.0, 0.0);
	glRotatef(fingerBaseBend + bend / 20, 0.0, 0.0, 1.0);
	glRotatef(270, 0.0, 1.0, 0.0); // not this
	glScalef(1.3, 1.0, 1.0);
	fingerJointConnect(0.9, i, j);

	// Finger Middle
	glPushMatrix();
	glTranslatef(0.0, 1.9, 0.0);
	glRotatef(bendX, 1.0, 0.0, 0.0);
	fingerJointConnect(0.6, i, j);

	// Finger Tip
	glPushMatrix();
	glTranslatef(0.0, 1.6, 0.0);
	glRotatef(fingerTipBend, 1.0, 0.0, 0.0);
	fingerJointConnect(0.1, i, j);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	// Thumb Support
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	if (isRight)
	{
		glPushMatrix();
		glTranslatef(-1.9, 0.0, 0.8);
		glScalef(3.2, 1.0, 0.9);
		drawCube(i);
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glTranslatef(1.9, 0.0, 0.8);
		glScalef(3.2, 1.0, 0.9);
		drawCube(i);
		glPopMatrix();
	}
}

void rightHand(float index, float middle, float ring, float pinky, float thum, float thumX, int i, int j)
{
	glPushMatrix();
	glTranslatef(0.0, 2.4, 0.0);
	// Hand Rotator
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.0, -1.8, 0.0);
	drawSphere(0.6, 20, 20, 2);
	glPopMatrix();

	// Hand Base
	glPushMatrix();
	glScalef(3.0, 3.6, 0.7);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Index Finger
	glPushMatrix();
	glTranslatef(0.4, 2.0, 0.0);
	glPushMatrix();
	glTranslatef(0.8, 0.0, 0.0);
	indexFinger(index, i, j);
	glPopMatrix();

	// Middle Finger
	glPushMatrix();
	middleFinger(middle, i, j);
	glPopMatrix();

	// Ring Finger
	glPushMatrix();
	glTranslatef(-0.8, 0.0, 0.0);
	ringFinger(ring, i, j);
	glPopMatrix();

	// Pinkie
	glPushMatrix();
	glTranslatef(-1.6, 0.0, 0.0);
	pinkie(pinky, i, j);
	glPopMatrix();
	glPopMatrix();

	// Thumb
	glPushMatrix();
	glTranslatef(1.9, -1.3, 0.0);
	thumb(thum, thumX, true, i, j);
	glPopMatrix();

	// Hand Bottom Cover
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-1.4, 1.7, 0.44);
	glScalef(2.8, 2.4, 0.2);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.4, -1, 0.44);
	glScalef(2.8, 2.4, 0.2);
	glRotatef(180, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	// Hand Top Cover
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.0, -0.09, -0.45);
	glScalef(3.0, 3.4, 0.2);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Hand Left Cover
	glPushMatrix();
	glTranslatef(-1.67, -0.3, 0.0);
	glScalef(0.2, 2.7, 0.3);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(270, 1.0, 0.0, 0.0);
	twoSideTriangle(i);
	glPopMatrix();

	// Hand Top Deco
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(1.2, 1.32, -0.55);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 2.8, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.4, 1.31, -0.55);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 2.8, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4, 1.31, -0.55);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 2.8, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.12, 1.31, -0.55);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 2.8, 2);
	glPopMatrix();

	glPopMatrix();
}

void leftHand(float index, float middle, float ring, float pinky, float thum, float thumX, int i, int j)
{
	glPushMatrix();
	glTranslatef(0.0, 1.8, 0.0);

	// Hand Rotator
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.0, -1.8, 0.0);
	drawSphere(0.6, 20, 20, 2);
	glPopMatrix();

	// Hand Base
	glPushMatrix();
	glScalef(3.0, 3.6, 0.7);
	drawCube(i);
	glPopMatrix();

	// Index Finger
	glPushMatrix();
	glTranslatef(0.4, 2.0, 0.0);
	glPushMatrix();
	glTranslatef(-1.6, 0.0, 0.0);
	indexFinger(index, i, j);
	glPopMatrix();

	// Middle Finger
	glPushMatrix();
	glTranslatef(-0.8, 0.0, 0.0);
	middleFinger(middle, i, j);
	glPopMatrix();

	// Ring Finger
	glPushMatrix();
	ringFinger(ring, i, j);
	glPopMatrix();

	// Pinkie
	glPushMatrix();
	glTranslatef(0.8, 0.0, 0.0);
	pinkie(pinky, i, j);
	glPopMatrix();
	glPopMatrix();

	// Thumb
	glPushMatrix();
	glTranslatef(-1.9, -1.3, 0.0);
	thumb(thum, thumX, false, i, j);
	glPopMatrix();

	// Hand Bottom Cover
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-1.4, 1.7, 0.44);
	glScalef(2.8, 2.4, 0.2);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.4, -1, 0.44);
	glScalef(2.8, 2.4, 0.2);
	glRotatef(180, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	// Hand Top Cover
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.0, -0.09, -0.45);
	glScalef(3.0, 3.4, 0.2);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Hand Right Cover
	glPushMatrix();
	glTranslatef(1.67, -0.3, 0.0);
	glScalef(0.2, 2.7, 0.3);
	glRotatef(270, 0.0, 1.0, 0.0);
	glRotatef(270, 1.0, 0.0, 0.0);
	twoSideTriangle(i);
	glPopMatrix();

	// Hand Top Deco
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(1.2, 1.32, -0.55);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 2.8, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.4, 1.31, -0.55);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 2.8, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4, 1.31, -0.55);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 2.8, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.12, 1.31, -0.55);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 2.8, j);
	glPopMatrix();

	glPopMatrix();
}

/** Right Arm **/

void rightForeArmDeco(int i, int j)
{
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-1.97, -0.83, -0.86);
	glRotatef(61.5, 0.0, 1.0, 0.0);
	drawFullCylinder(0.1, 0.18, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.97, -4.83, -0.86);
	glRotatef(61.5, 0.0, 1.0, 0.0);
	drawFullCylinder(0.1, 0.18, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, -4.92, -0.84);
	glRotatef(-90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 4.19, j);
	glPopMatrix();
}

void foreArmTopDeco(int i, int j)
{
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.45, -2.89, -1.17);
	glScalef(0.88, 4.9, 0.2);
	drawCube(i);
	glPopMatrix();

	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4), textures[0] = loadTexture("black2.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black2.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black2.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.21, -2.9, -1.24);
	glScalef(0.5, 4.9, 0.6);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawRightTriangle(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.11, -2.9, -1.59);
	glScalef(0.2, 4.9, 1.0);
	drawCube(i);
	glPopMatrix();
}

void rightForeArm(int i, int j)
{
	glPushMatrix();
	glTranslatef(0.0, 5.7, 0.0);

	// Hand Rotator
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.0, -5.7, 0.0);
	drawSphere(0.6, 20, 20, j);
	glPopMatrix();

	// Fore arm Base
	glPushMatrix();
	glTranslatef(0.0, -2.9, 0.0);
	glScalef(3.3, 5.8, 2.2);
	drawCube(i);
	glPopMatrix();

	// Fore arm top
	glPushMatrix();
	glTranslatef(0.4, 0.0, 0.0);
	foreArmTopDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -5.8, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	foreArmTopDeco(i, j);
	glPopMatrix();

	// Fore arm right armor
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-1.95, -2.9, 0.6);
	glScalef(0.6, 5.8, 1.0);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.64, -2.9, 0.1);
	glScalef(0.63, 5.8, 1.2);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	drawRightTriangle(i);
	glPopMatrix();

	// Fore arm right armor deco
	rightForeArmDeco(i, j);
	glPushMatrix();
	glTranslatef(-0.24, 0.0, 0.5);
	rightForeArmDeco(i, j);
	glPopMatrix();

	// Fore arm right armor sword
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("lightblue1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-2.25, -4.3, 0.65);
	glScalef(1.3, 3.2, 0.15);
	glRotatef(180, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	// Fore arm left armor
	glPushMatrix();
	glTranslatef(1.7, -3.0, 0.0);
	glScalef(0.1, 5.2, 2.0);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Fore arm left decoration
	if (i == 1)
	{
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(1.7, -1.0, -0.5);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.2, 4.1, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.7, -1.0, 0.4);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.2, 4.1, j);
	glPopMatrix();

	// Fore arm bottom armor
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-2.1, -0.2, 1.2);
	glScalef(3.6, 5.5, 0.2);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.55, -5.73, 1.2);
	glScalef(3.6, 5.5, 0.2);
	glRotatef(180, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	// Fore arm connector support
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.6, 0.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(-180, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	glPopMatrix();
}

void rightUpperArmBottomDeco(int i, int j)
{
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.0, -5.5, 1.37);
	glScalef(1.0, 1.0, 0.5);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -5.0, 1.12);
	glRotatef(180, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.51, 3.98, 0.99);
	drawRightTriangle(i);
	glPopMatrix();
}

void rightUpperArmRightDeco(int i, int j)
{
	// Right Upper Right Deco
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("lightblue1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(1.87, -2.2, -0.7);
	glRotatef(45, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 0.9, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.87, -2.84, -0.2);
	glRotatef(315, 1.0, 0.0, 0.0);
	drawFullCylinder(0.1, 0.9, j);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);
}

void rightUpperArm(int i, int j)
{
	// Right Upper Arm Base
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey), textures[0] = loadTexture("black1.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.2, -3.5, 0.0);
	glScalef(4.1, 5.9, 2.2);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Right Upper Arm Bottom
	glPushMatrix();
	glTranslatef(0.9, 0.0, 0.0);
	rightUpperArmBottomDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.2, 0.0, 0.0);
	rightUpperArmBottomDeco(i, j);
	glPopMatrix();

	// Right Upper Right Deco
	glPushMatrix();
	glTranslatef(0.0, 1.0, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.2, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -0.6, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -1.4, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -2.2, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -3.0, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Right Upper Top Deco
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("lightblue1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.2, -3.5, -1.14);
	glScalef(3.9, 5.6, 0.1);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Fore arm connector support
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.6, -0.6, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -0.6, 0.0);
	glRotatef(-180, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

}

void rightArmRotator(int i, int j)
{
	glPushMatrix();
	glRotatef(180, 0.0, 1.0, 0.0);
	// Arm Rotator Base
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.1, -0.6, -1.4);
	drawFullCylinder(2.5, 2.7, j);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Arm Rotator Front Ring
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.1, -0.6, 1.3);
	drawDonut(2.0, 2.48, 0.3, j);
	glPopMatrix();

	// Arm Rotator Front Deco
	glPushMatrix();
	glTranslatef(-0.18, -1.9, 1.2);
	drawDonut(0.2, 0.4, 0.2, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.1, 0.06, 1.2);
	drawDonut(0.2, 0.4, 0.2, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.3, 0.06, 1.2);
	drawDonut(0.2, 0.4, 0.2, j);
	glPopMatrix();
	glPopMatrix();
}

void rightArmFull(float armLift, float armOpen, float armFlex, float armRotate, float armInOut, float handFlex, float handRotate, float handTilt, float iB, float mB, float rB, float pB, float tB, float tBX, boolean swordHold, int i, int j)
{
	glPushMatrix();

	glRotatef(armLift, 1.0, 0.0, 0.0);	// swing sword
	glRotatef(armOpen + 10, 0.0, 0.0, 1.0);	// fly
	glTranslatef(0.1, 0.6, 0.0);
	rightArmRotator(i, j);

	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 8.0, 0.0);
	rightUpperArm(i, j);

	glPushMatrix();
	// Insert foreaarm rotation here
	glRotatef(armFlex - 5, 0.0, 0.0, 1.0);	// flex arm
	glRotatef(armRotate, 0.0, 1.0, 0.0);	// drill hand
	glRotatef(armInOut, 1.0, 0.0, 0.0);
	rightForeArm(i, j);

	glPushMatrix();
	glTranslatef(0.0, 5.7, 0.0);
	// insert right hand rotation here
	glRotatef(handFlex + 5, 1.0, 0.0, 0.0);	// flex hand
	glRotatef(handRotate, 0.0, 1.0, 0.0);	// drill fist
	glRotatef(handTilt + 10, 0.0, 0.0, 1.0);	// hello

	rightHand(iB, mB, rB, pB, tB, tBX, i, j);

	if (swordHold)
	{
		glPushMatrix();
		glTranslatef(6.2, 3.8, 0.7);
		glScalef(2.4, 2.4, 2.4);
		glRotatef(180, 1.0, 0.0, 0.0);
		if(i == 1) sword(1, 2);
		glPopMatrix();
	}

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}

void rightArmAnimated(int i, int j)
{
	// Initial State
	switch (playAnimation)
	{
	case 0:	// Initial State
		armLift = armOpen = armFlex = armRotate = armInOut = handFlex = handRotate = handTilt = tBX = 0;
		iB = mB = rB = pB = tB = 90;
		rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
		break;
	case 1:	// Attack animation
		animSpeed = 4;
		switch (armAnimState)
		{
		case 1:	// lift sword
			armLift = armOpen = armFlex = armRotate = armInOut = handFlex = handRotate = handTilt = tBX = 0;
			iB = mB = rB = pB = tB = 90;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
			armAnimState = 2;
			break;
		case 2:	// first strike
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
			if (armLift > -80) armLift -= animSpeed;
			if (armFlex > -80) armFlex -= animSpeed;
			if (handRotate > -10) handRotate -= animSpeed;

			if (armLift <= -80 && armFlex <= -80 && handRotate <= -10) armAnimState = 3;
			break;
		case 3:	// second prepare
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
			if (armLift < -70) armLift += animSpeed;
			if (armInOut < 60) armInOut += animSpeed;

			if (armLift >= -70 && armInOut >= 60) armAnimState = 4;
			break;
		case 4:	// second strike
			animSpeed = 15;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
			if (armLift < 0) armLift += animSpeed;
			if (armOpen < 20) armOpen += animSpeed;
			if (armFlex < 10) armFlex += animSpeed;
			if (armInOut > -10) armInOut -= animSpeed;
			if (handRotate < 10) handRotate += animSpeed;

			if (armLift >= 0 && armOpen >= 20 && armFlex >= 10 && armInOut <= -10 && handRotate >= 10) armAnimState = 5;
			break;
		case 5:	// third prepare
			animSpeed = 4;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
			if (armLift > -110) armLift -= animSpeed;
			if (armFlex > -20) armFlex -= animSpeed;

			if (armLift <= -110 && armFlex <= -20) armAnimState = 6;
			break;
		case 6:	// third strike
			animSpeed = 15;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
			if (armLift < -40) armLift += animSpeed;
			if (armOpen > -10) armOpen -= animSpeed;
			if (armInOut < 40) armInOut += animSpeed;
			if (handTilt < 40) handTilt += animSpeed;

			if (armLift >= -40 && armOpen <= -10 && armInOut >= 40 && handTilt >= 40) armAnimState = 7;
			break;
		case 7:	// forth prepare
			animSpeed = 4;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
			if (armLift > -80) armLift -= animSpeed;
			if (armOpen > -30) armOpen -= animSpeed;
			if (armFlex < 0) armFlex += animSpeed;
			if (armRotate < 5) armRotate += animSpeed;
			if (handRotate > -100) handRotate -= animSpeed * 1.5;
			if (handTilt > 10) handTilt -= animSpeed;

			if (armLift <= -80 && armOpen <= -30 && armFlex >= 0 && armRotate >= 5 && handRotate <= -100 && handTilt <= 10) armAnimState = 8;
			break;
		case 8:	// forth strike
			animSpeed = 15;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
			if (armOpen < 60) armOpen += animSpeed;
			if (armFlex > -30) armFlex -= animSpeed;
			if (armRotate > -95) armRotate -= animSpeed;
			if (handRotate < 0) handRotate += animSpeed;
			if (handTilt < 60) handTilt += animSpeed;

			if (armOpen >= 60 && armFlex <= -30 && armRotate <= -95 && handRotate >= 0 && handTilt >= 60) armAnimState = 9;
			break;
		case 9:	// finishing pose
			animSpeed = 2.2;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
			if (armLift < -40) armLift += animSpeed;
			if (armOpen > 1) armOpen -= animSpeed;
			if (armFlex > -51) armFlex -= animSpeed;
			if (armRotate < 8) armRotate += animSpeed;
			if (armInOut < 84) armInOut += animSpeed;
			if (handFlex > -10) handFlex -= animSpeed;
			if (handRotate > 6) handRotate -= animSpeed;
			if (handTilt > -10) handTilt -= animSpeed;

			if (armLift >= -40 && armOpen <= 1 && armFlex <= -51 && armRotate >= 8 && armInOut >= 84 && handFlex <= -10 && handRotate <= 6 && handTilt <= -10)
			{
				animDelay++;
				if (animDelay > 20)
					armAnimState = 10;
			}
			break;
		case 10: // return and end
		{
			animDelay = 0;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, true, i, j);
			if (armLift < 0) armLift += animSpeed;
			if (armOpen > 0) armOpen -= animSpeed;
			if (armFlex < 0) armFlex += animSpeed;
			if (armRotate > 0) armRotate -= animSpeed;
			if (armInOut > 0) armInOut -= animSpeed * 2;
			if (handFlex < 0) handFlex += animSpeed;
			if (handRotate > 0) handRotate -= animSpeed;
			if (handTilt < 0) handTilt += animSpeed;

			if (armLift >= 0 && armOpen <= 0 && armFlex >= 0 && armRotate <= 0 && armInOut <= 0 && handFlex >= 0 && handRotate <= 0 && handTilt >= 0)
			{
				playAnimation = 0;
				armAnimState = 1;
			}
		}
		}
		break;
	case 2:	// Walking
		switch (armAnimState)
		{
		case 1:
			armLift = armOpen = armFlex = armRotate = armInOut = handFlex = handRotate = handTilt = tBX = 0;
			iB = mB = rB = pB = tB = 0;
			armAnimState = 2;
		case 2: // right lift
			animSpeed = 1;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, false, i, j);
			if (armLift > -20) armLift -= animSpeed;
			break;
		case 3:	// right down
			iB = mB = rB = pB = tB = 0;
			animSpeed = 1;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, false, i, j);
			if (armLift < 20) armLift += animSpeed;
			break;
		}
		break;
	case 3: // Cannon pose
		switch (armAnimState)
		{
		case 1:
			animSpeed = 1.2;
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, false, i, j);
			if (armOpen > -6) armOpen -= animSpeed;
			if (armLift > -30) armLift -= animSpeed;
			if (armFlex > -55) armFlex -= animSpeed;
			if (armRotate > -80) armRotate -= animSpeed * 1.5;
			if (handTilt > -10) handTilt -= animSpeed;
			break;
		case 2:
			rightArmFull(armLift, armOpen, armFlex, armRotate, armInOut, handFlex, handRotate, handTilt, iB, mB, rB, pB, tB, tBX, false, i, j);
			if (armOpen < 0) armOpen += animSpeed;
			if (armLift < 0) armLift += animSpeed;
			if (armFlex < 0) armFlex += animSpeed;
			if (armRotate < 0) armRotate += animSpeed * 1.5;
			if (handTilt < 0) handTilt == animSpeed;
			if (armLift >= 0 && armFlex >= 0 && armRotate >= 0 && handTilt >= 0) playAnimation = 0;
			break;
		}
	default:
		;
	}

}

/** Left Arm **/

void leftForeArm(int i, int j)
{
	glPushMatrix();
	glTranslatef(0.0, 5.7, 0.0);

	// Hand Rotator
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.0, -5.7, 0.0);
	drawSphere(0.6, 20, 20, j);
	glPopMatrix();

	// Fore arm Base
	glPushMatrix();
	glTranslatef(0.0, -2.9, 0.0);
	glScalef(3.3, 5.8, 2.2);
	drawCube(i);
	glPopMatrix();

	// Fore arm top
	glPushMatrix();
	glTranslatef(0.4, 0.0, 0.0);
	foreArmTopDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -5.8, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	foreArmTopDeco(i, j);
	glPopMatrix();

	// Fore arm right armor
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey), textures[0] = loadTexture("black1.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(1.95, -2.9, 0.6);
	glScalef(0.6, 5.8, 1.0);
	drawCube(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.65, -2.9, 0.1);
	glScalef(0.63, 5.8, 1.2);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawRightTriangle(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Fore arm right armor deco
	glPushMatrix();
	glTranslatef(0.0, -5.7, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	rightForeArmDeco(i, j);
	glPushMatrix();
	glTranslatef(-0.24, 0.0, 0.5);
	rightForeArmDeco(i, j);
	glPopMatrix();
	glPopMatrix();

	// Fore arm right armor sword
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("lightblue1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(2.25, -4.3, 0.65);
	glScalef(1.3, 3.2, 0.15);
	glRotatef(180, 0.0, 1.0, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	// Fore arm left armor
	glPushMatrix();
	glTranslatef(-1.7, -3.0, 0.0);
	glScalef(0.1, 5.2, 2.0);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Fore arm left decoration
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey), textures[0] = loadTexture("black1.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-1.7, -1.0, -0.5);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.2, 4.1, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.7, -1.0, 0.4);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFullCylinder(0.2, 4.1, j);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Fore arm bottom armor
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(2.1, -0.2, 1.2);
	glScalef(3.6, 5.5, 0.2);
	glRotatef(180, 0.0, 1.0, 0.0);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.55, -5.73, 1.2);
	glScalef(3.6, 5.5, 0.2);
	glRotatef(180, 0.0, 1.0, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	// Fore arm connector support
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.6, 0.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.0, 0.0);
	glRotatef(-180, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();
	glPopMatrix();
}

void leftUpperArm(int i, int j)
{
	// Left Upper Arm Base
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey), textures[0] = loadTexture("black1.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("black1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.2, -3.5, 0.0);
	glScalef(4.1, 5.9, 2.2);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Left Upper Arm Bottom
	glPushMatrix();
	glTranslatef(0.9, 0.0, 0.0);
	rightUpperArmBottomDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.2, 0.0, 0.0);
	rightUpperArmBottomDeco(i, j);
	glPopMatrix();

	// Left Upper Right Deco
	glPushMatrix();
	glTranslatef(-4.14, 1.0, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.14, 0.2, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.14, -0.6, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.14, -1.4, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.14, -2.2, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.14, -3.0, 0.1);
	rightUpperArmRightDeco(i, j);
	glPopMatrix();

	// Left Upper Top Deco
	if (i == 1)
	{
		if (isTexture == 0)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else if (isTexture == 1)
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("white3.bmp");
		else
			glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white), textures[0] = loadTexture("lightblue1.bmp");
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.2, -3.5, -1.14);
	glScalef(3.9, 5.6, 0.1);
	drawCube(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[0]);

	// Fore arm connector support
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.6, -0.6, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -0.6, 0.0);
	glRotatef(-180, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawRightTriangle(i);
	glPopMatrix();
}

void leftArmRotator(int i, int j)
{
	// Arm Rotator Base
	if (i == 1)
	{
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.1, -0.6, -1.4);
	drawFullCylinder(2.5, 2.7, j);
	glPopMatrix();

	// Arm Rotator Front Ring
	if (i == 1)
	{
		glColor3f(1.0, 1.0, 1.0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.1, -0.6, 1.3);
	drawDonut(2.0, 2.48, 0.3, j);
	glPopMatrix();

	// Arm Rotator Front Deco
	glPushMatrix();
	glTranslatef(-0.18, -1.9, 1.2);
	drawDonut(0.2, 0.4, 0.2, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.1, 0.06, 1.2);
	drawDonut(0.2, 0.4, 0.2, j);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.3, 0.06, 1.2);
	drawDonut(0.2, 0.4, 0.2, j);
	glPopMatrix();
}

void leftArmFull(float armLift, float armOpen, float armFlex, float armRotate, float armInOut, float handFlex, float handRotate, float handTilt, float iB, float mB, float rB, float pB, float tB, float tBX, int i, int j)
{
	glPushMatrix();

	glRotatef(armLift, 1.0, 0.0, 0.0);	// swing sword
	glRotatef(armOpen + 10, 0.0, 0.0, 1.0);	// fly
	glTranslatef(0.1, 0.6, 0.0);
	leftArmRotator(i, j);

	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 8.0, 0.0);
	leftUpperArm(i, j);

	glPushMatrix();

	// Forearm Rotation
	glRotatef(armFlex + 5, 0.0, 0.0, 1.0);	// flex arm
	glRotatef(armRotate, 0.0, 1.0, 0.0);		// drill hand
	glRotatef(armInOut, 1.0, 0.0, 0.0);
	leftForeArm(i, j);

	glPushMatrix();
	glTranslatef(0.0, 6.3, 0.0);

	// Left Hand Rotation
	glRotatef(handFlex, 1.0, 0.0, 0.0);		// flex hand
	glRotatef(handRotate, 0.0, 1.0, 0.0);		// drill fist
	glRotatef(handTilt - 10, 0.0, 0.0, 1.0);	// hello

	leftHand(iB, mB, rB, pB, tB, tBX, i, j);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void leftArmAnimated(int i, int j)
{
	switch (playAnimation)
	{
	case 0:	// Initial State
		lArmLift = lArmOpen = lArmFlex = lArmRotate = lArmInOut = lHandFlex = lHandRotate = lHandTilt = lTBX = 0;
		lIB = lMB = lRB = lPB = lTB = 0;
		leftArmFull(lArmLift, lArmOpen, lArmFlex, lArmRotate, lArmInOut, lHandFlex, lHandRotate, lHandTilt, lIB, lMB, lRB, lPB, lTB, lTBX, i, j);
		break;
	case 1:	// attack animation no change
		leftArmFull(lArmLift, lArmOpen, lArmFlex, lArmRotate, lArmInOut, lHandFlex, lHandRotate, lHandTilt, lIB, lMB, lRB, lPB, lTB, lTBX, i, j);
		break;
	case 2:	// walking
		switch (armAnimState)
		{
		case 1:
			lArmLift = lArmOpen = lArmFlex = lArmRotate = lArmInOut = lHandFlex = lHandRotate = lHandTilt = lTBX = 0;
			lIB = lMB = lRB = lPB = lTB = 0;
			armAnimState = 2;
			break;
		case 2: // right lift
			leftArmFull(lArmLift, lArmOpen, lArmFlex, lArmRotate, lArmInOut, lHandFlex, lHandRotate, lHandTilt, lIB, lMB, lRB, lPB, lTB, lTBX, i, j);
			if (lArmLift > -20) lArmLift -= animSpeed;
			break;
		case 3:	// right down
			leftArmFull(lArmLift, lArmOpen, lArmFlex, lArmRotate, lArmInOut, lHandFlex, lHandRotate, lHandTilt, lIB, lMB, lRB, lPB, lTB, lTBX, i, j);
			if (lArmLift < 20) lArmLift += animSpeed;
			break;
		}
		break;
	case 3:  //canon
		switch (armAnimState)
		{
		case 1:
			animSpeed = 1.6;
			leftArmFull(lArmLift, lArmOpen, lArmFlex, lArmRotate, lArmInOut, lHandFlex, lHandRotate, lHandTilt, lIB, lMB, lRB, lPB, lTB, lTBX, i, j);
			if (lArmLift < 40) lArmLift += animSpeed;
			if (lArmOpen > -23) lArmOpen -= animSpeed;
			if (lArmFlex < 120) lArmFlex += animSpeed * 1.5;
			if (lArmRotate < 30) lArmRotate += animSpeed;
			if (lArmInOut < 97) lArmInOut += animSpeed * 1.5;
			if (lIB < 90) lIB += animSpeed * 1.2;
			if (lMB < 90) lMB += animSpeed * 1.2;
			if (lRB < 90) lRB += animSpeed * 1.2;
			if (lPB < 90) lPB += animSpeed * 1.2;
			if (lTB < 90) lTB += animSpeed * 1.2;
			break;
		case 2:
			animSpeed = 1.6;
			leftArmFull(lArmLift, lArmOpen, lArmFlex, lArmRotate, lArmInOut, lHandFlex, lHandRotate, lHandTilt, lIB, lMB, lRB, lPB, lTB, lTBX, i, j);
			if (lArmLift > 0) lArmLift -= animSpeed;
			if (lArmOpen < 0) lArmOpen += animSpeed;
			if (lArmFlex > 0) lArmFlex -= animSpeed * 1.5;
			if (lArmRotate > 0) lArmRotate -= animSpeed;
			if (lArmInOut > 0) lArmInOut -= animSpeed * 1.5;
			if (lIB > 0) lIB -= animSpeed * 1.2;
			if (lMB > 0) lMB -= animSpeed * 1.2;
			if (lRB > 0) lRB -= animSpeed * 1.2;
			if (lPB > 0) lPB -= animSpeed * 1.2;
			if (lTB > 0) lTB -= animSpeed * 1.2;
			break;
		}
		break;
	default:
		;
	}
}




// body -------------------------------------------------------------------------------------




//upper body--------------------------------------------------------------------------------------------------------------------
void bodyFront() {
	glPushMatrix();

	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//f1.l
	if (isTexture == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);	glColor3f(0.6, 0.6, 0.6);
	}
	else {

		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.55, 0.7);
	}
	glVertex3f(-0.5, 4.66, -1);		//1
	if (isTexture == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);	glColor3f(1, 1, 1);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.7, 0.7);
	}
	glVertex3f(0, 4.66, -1);		//2
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.7, 0.3);
	}
	glVertex3f(0, 3.33, -1);		//3
	if (isTexture == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);	glColor3f(0.6, 0.6, 0.6);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.45, 0.2);
	}
	glVertex3f(-0.84, 3, -1);		//4
	if (isTexture == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);	glColor3f(0.3, 0.3, 0.3);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.1, 0.35);
	}
	glVertex3f(-1.17, 2.66, -1);		//5
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.45, 0.2);
	}
	glVertex3f(-1.17, 3.66, -1);		//6
	if (isTexture == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);	glColor3f(0.6, 0.6, 0.6);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.4, 0.45);
	}
	glVertex3f(-1, 3.84, -1);		//7
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.4, 0.55);
	}
	glVertex3f(-1, 4.17, -1);		//8
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.525, 0.65);
	}
	glVertex3f(-0.58, 4.5, -1);		//9
	glEnd();
	glDeleteTextures(1, &textures[0]);






	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//f1.r
	if (isTexture == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);	glColor3f(0.6, 0.6, 0.6);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.55, 0.7);
	}
	glVertex3f(0.5, 4.66, -1);		//1
	if (isTexture == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);	glColor3f(1, 1, 1);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.7, 0.7);
	}
	glVertex3f(0, 4.66, -1);		//2
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.7, 0.3);
	}
	glVertex3f(0, 3.33, -1);		//3
	if (isTexture == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);	glColor3f(0.6, 0.6, 0.6);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.45, 0.2);
	}
	glVertex3f(0.84, 3, -1);		//4
	if (isTexture == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);	glColor3f(0.3, 0.3, 0.3);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.1, 0.35);
	}
	glVertex3f(1.17, 2.66, -1);		//5
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.45, 0.2);
	}
	glVertex3f(1.17, 3.66, -1);		//6
	if (isTexture == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);	glColor3f(0.6, 0.6, 0.6);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.4, 0.45);
	}
	glVertex3f(1, 3.84, -1);		//7
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.4, 0.55);
	}
	glVertex3f(1, 4.17, -1);		//8
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.525, 0.65);
	}
	glVertex3f(0.58, 4.5, -1);		//9
	glEnd();
	glDeleteTextures(1, &textures[0]);


	pc++;  	glBegin(GL_POLYGON);		//f2.r
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(1.17, 3.33, -0.87);
	glVertex3f(0, 3.33, -0.87);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 2, -0.87);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0.84, 2.33, -0.87);
	glVertex3f(1.17, 2, -0.87);
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//f2.l
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-1.17, 3.33, -0.87);
	glVertex3f(0, 3.33, -0.87);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 2, -0.87);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-0.84, 2.33, -0.87);
	glVertex3f(-1.17, 2, -0.87);
	glEnd();



	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//f3.r
	if (isTexture == 0)
	{
		glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	}
	else {

		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.15, 0.4);
	}
	glVertex3f(1.17, 2, -0.66);		//1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.5);
	}
	glVertex3f(0.84, 2.33, -0.66);	//2
	if (isTexture == 0)
	{
		glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.4);
	}
	glVertex3f(0, 2, -0.66);		//3
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0);
	}
	glVertex3f(0, 0.66, -0.66);		//4
	if (isTexture == 0)
	{
		glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.1);
	}
	glVertex3f(0.84, 1, -0.66);		//5
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.15, 0.3);
	}
	glVertex3f(1.17, 1.66, -0.66);	//6
	glEnd();
	glDeleteTextures(1, &textures[0]);




	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//f3.l
	if (isTexture == 0)
	{
		glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	}
	else {

		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.15, 0.4);
	}
	glVertex3f(-1.17, 2, -0.66);		//1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.5);
	}
	glVertex3f(-0.84, 2.33, -0.66);	//2
	if (isTexture == 0)
	{
		glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.4);
	}
	glVertex3f(0, 2, -0.66);		//3
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0);
	}
	glVertex3f(0, 0.66, -0.66);		//4
	if (isTexture == 0)
	{
		glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.1);
	}
	glVertex3f(-0.84, 1, -0.66);		//5
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.15, 0.3);
	}
	glVertex3f(-1.17, 1.66, -0.66);	//6
	glEnd();
	glDeleteTextures(1, &textures[0]);






	quadC++;    glBegin(GL_QUADS);		//f4.r
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0.84, 1, -0.66);
	glVertex3f(0, 0.66, -0.66);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0, 0.5, -0.66);
	glVertex3f(0.84, 0.5, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//f4.l
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(-0.84, 1, -0.66);
	glVertex3f(0, 0.66, -0.66);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0, 0.5, -0.66);
	glVertex3f(-0.84, 0.5, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//f5.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1, 0.84, -0.5);
	glVertex3f(0, 0.84, -0.5);
	glVertex3f(0, 0, -1);
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(1.5, 0, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//f5.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1, 0.84, -0.5);
	glVertex3f(0, 0.84, -0.5);
	glVertex3f(0, 0, -1);
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-1.5, 0, -1);
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//f6.r
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0.5, 1.84, -0.84);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 1.66, -0.84);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(0, 1.33, -0.84);
	glVertex3f(0.5, 1.5, -0.84);
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0.66, 1.66, -0.84);
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//f6.l
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(-0.5, 1.84, -0.84);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0, 1.66, -0.84);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-0, 1.33, -0.84);
	glVertex3f(-0.5, 1.5, -0.84);
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(-0.66, 1.66, -0.84);
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//f7.r
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0.5, 1.5, -0.84);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 1.33, -0.84);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(0, 1, -0.84);
	glVertex3f(0.5, 1.17, -0.84);
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0.66, 1.33, -0.84);
	glEnd();


	pc++;  	glBegin(GL_POLYGON);		//f7.l
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(-0.5, 1.5, -0.84);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0, 1.33, -0.84);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-0, 1, -0.84);
	glVertex3f(-0.5, 1.17, -0.84);
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(-0.66, 1.33, -0.84);
	glEnd();




	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//f8.r
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.35, 0.5);
	}
	glVertex3f(1.17, 4.66, -0.66);		//1
	if (isTexture == 0)
	{
		glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.5);
	}
	glVertex3f(0, 4.66, -0.66);			//2
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.2);
	}
	glVertex3f(0, 3.66, -0.66);			//3
	if (isTexture == 0)
	{
		glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.05, 0.2);
	}
	glVertex3f(1.5, 3.66, -0.66);		//4
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.05, 0.45);
	}
	glVertex3f(1.5, 4.5, -0.66);		//5
	glEnd();
	glDeleteTextures(1, &textures[0]);


	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//f8.r
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.35, 0.5);
	}
	glVertex3f(-1.17, 4.66, -0.66);		//1
	if (isTexture == 0)
	{
		glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.5);
	}
	glVertex3f(0, 4.66, -0.66);			//2
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.2);
	}
	glVertex3f(0, 3.66, -0.66);			//3
	if (isTexture == 0)
	{
		glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.05, 0.2);
	}
	glVertex3f(-1.5, 3.66, -0.66);		//4
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.05, 0.45);
	}
	glVertex3f(-1.5, 4.5, -0.66);		//5
	glEnd();
	glDeleteTextures(1, &textures[0]);





	quadC++;    glBegin(GL_QUADS);		//f9.r
	glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	glVertex3f(0.66, 5, -0.33);
	glVertex3f(1, 5, -0.33);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1, 4.66, -0.66);
	glVertex3f(0.66, 4.66, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//f9.l
	glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	glVertex3f(-0.66, 5, -0.33);
	glVertex3f(-1, 5, -0.33);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1, 4.66, -0.66);
	glVertex3f(-0.66, 4.66, -0.66);
	glEnd();

	glPopMatrix();
}

void bodySide() {

	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//s1.r
	if (isTexture == 0)
	{
		glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0.55);
	}
	glVertex3f(1.5, 4.5, -0.66);	//1
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.55);
	}
	glVertex3f(1.5, 4.5, 1);		//2
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.1);
	}
	glVertex3f(1.5, 3, 1);			//3
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.45, 0);
	}
	glVertex3f(1.5, 2.66, 0.84);	//4
	if (isTexture == 0)
	{
		glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0);
	}
	glVertex3f(1.5, 2.66, -0.66);	//5
	glEnd();
	glDeleteTextures(1, &textures[0]);





	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//s1.l
	if (isTexture == 0)
	{
		glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0.55);
	}
	glVertex3f(-1.5, 4.5, -0.66);	//1
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.55);
	}
	glVertex3f(-1.5, 4.5, 1);		//2
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.1);
	}
	glVertex3f(-1.5, 3, 1);			//3
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.45, 0);
	}
	glVertex3f(-1.5, 2.66, 0.84);	//4
	if (isTexture == 0)
	{
		glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0);
	}
	glVertex3f(-1.5, 2.66, -0.66);	//5
	glEnd();
	glDeleteTextures(1, &textures[0]);







	pc++;  	glBegin(GL_POLYGON);		//s2.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1.17, 2.33, -0.66);
	glVertex3f(1.17, 2.33, 0);
	glVertex3f(1.17, 2.17, 0.5);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(1.17, 1.66, 0.5);
	glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	glVertex3f(1.17, 1.66, -0.66);
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//s2.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1.17, 2.33, -0.66);
	glVertex3f(-1.17, 2.33, 0);
	glVertex3f(-1.17, 2.17, 0.5);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-1.17, 1.66, 0.5);
	glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	glVertex3f(-1.17, 1.66, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s3.r
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(1.17, 1.66, -0.66);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1.17, 1.66, 0.5);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(0.84, 1, 0.5);
	glColor3f(0.9, 0.9, 0.9); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey9);
	glVertex3f(0.84, 1, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s3.l
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-1.17, 1.66, -0.66);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1.17, 1.66, 0.5);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-0.84, 1, 0.5);
	glColor3f(0.9, 0.9, 0.9); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey9);
	glVertex3f(-0.84, 1, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s4.r
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(0.84, 1, -0.66);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.84, 1, 0.5);
	glVertex3f(0.84, 0.5, 0.5);
	glVertex3f(0.84, 0.5, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s4.l
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-0.84, 1, -0.66);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.84, 1, 0.5);
	glVertex3f(-0.84, 0.5, 0.5);
	glVertex3f(-0.84, 0.5, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s5.r
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(1, 0.84, -0.5);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1, 0.84, 0.66);
	glVertex3f(1.5, 0, 1);
	glVertex3f(1.5, 0, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s5.l
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-1, 0.84, -0.5);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1, 0.84, 0.66);
	glVertex3f(-1.5, 0, 1);
	glVertex3f(-1.5, 0, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s6.r
	glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	glVertex3f(1.17, 4.66, -0.66);
	glVertex3f(1.17, 4.66, 0.84);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1.5, 4.5, 1);
	glVertex3f(1.5, 4.5, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s6.l
	glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	glVertex3f(-1.17, 4.66, -0.66);
	glVertex3f(-1.17, 4.66, 0.84);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1.5, 4.5, 1);
	glVertex3f(-1.5, 4.5, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s7.r
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(1, 5, -0.33);
	glVertex3f(1, 5, 0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1.17, 4.66, 0.33);
	glVertex3f(1.17, 4.66, -0.33);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s7.l
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-1, 5, -0.33);
	glVertex3f(-1, 5, 0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1.17, 4.66, 0.33);
	glVertex3f(-1.17, 4.66, -0.33);
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//s8.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1, 5, -0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1.17, 4.66, -0.33);
	glVertex3f(1, 4.66, -0.66);
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//s8.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1, 5, -0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1.17, 4.66, -0.33);
	glVertex3f(-1, 4.66, -0.66);
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//s9.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1, 5, 0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1.17, 4.66, 0.33);
	glVertex3f(1, 4.66, 0.66);
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//s9.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1, 5, 0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1.17, 4.66, 0.33);
	glVertex3f(-1, 4.66, 0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s10.r
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0.5, 4.66, -1);
	glVertex3f(0.58, 4.84, -0.66);
	glVertex3f(0.66, 4.66, -0.66);
	glVertex3f(0.58, 4.5, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s10.l
	glVertex3f(-0.5, 4.66, -1);
	glVertex3f(-0.58, 4.84, -0.66);
	glVertex3f(-0.66, 4.66, -0.66);
	glVertex3f(-0.58, 4.5, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s11.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.58, 4.5, -1);
	glVertex3f(0.66, 4.66, -0.66);
	glVertex3f(1.17, 4.33, -0.66);
	glVertex3f(1, 4.17, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s11.l
	glVertex3f(-0.58, 4.5, -1);
	glVertex3f(-0.66, 4.66, -0.66);
	glVertex3f(-1.17, 4.33, -0.66);
	glVertex3f(-1, 4.17, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s12.r
	glVertex3f(1, 4.17, -1);
	glVertex3f(1.17, 4.33, -0.66);
	glVertex3f(1.17, 4, -0.66);
	glVertex3f(1, 3.84, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s12.l
	glVertex3f(-1, 4.17, -1);
	glVertex3f(-1.17, 4.33, -0.66);
	glVertex3f(-1.17, 4, -0.66);
	glVertex3f(-1, 3.84, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s13.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1, 3.84, -1);
	glVertex3f(1.17, 4, -0.66);
	glVertex3f(1.5, 3.66, -0.66);
	glVertex3f(1.17, 3.66, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s13.l
	glVertex3f(-1, 3.84, -1);
	glVertex3f(-1.17, 4, -0.66);
	glVertex3f(-1.5, 3.66, -0.66);
	glVertex3f(-1.17, 3.66, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s14.r
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(1.17, 3.66, -1);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1.5, 3.66, -0.66);
	glVertex3f(1.5, 2.66, -0.66);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(1.17, 2.66, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s14.l
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-1.17, 3.66, -1);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1.5, 3.66, -0.66);
	glVertex3f(-1.5, 2.66, -0.66);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-1.17, 2.66, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s15.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1.17, 2.66, -1);
	glVertex3f(1.5, 2.66, -0.66);
	glVertex3f(1.17, 2.33, -0.66);
	glVertex3f(1, 2.33, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s15.l
	glVertex3f(-1.17, 2.66, -1);
	glVertex3f(-1.5, 2.66, -0.66);
	glVertex3f(-1.17, 2.33, -0.66);
	glVertex3f(-1, 2.33, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s16.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0.5, 1.84, -0.84);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(0.5, 2, -0.66);
	glVertex3f(0.84, 1.66, -0.66);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0.66, 1.66, -0.84);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s16.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-0.5, 1.84, -0.84);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-0.5, 2, -0.66);
	glVertex3f(-0.84, 1.66, -0.66);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-0.66, 1.66, -0.84);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s17.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0.66, 1.66, -0.84);
	glVertex3f(0.84, 1.66, -0.66);
	glVertex3f(0.66, 1.5, -0.66);
	glVertex3f(0.5, 1.5, -0.84);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s17.l
	glVertex3f(-0.66, 1.66, -0.84);
	glVertex3f(-0.84, 1.66, -0.66);
	glVertex3f(-0.66, 1.5, -0.66);
	glVertex3f(-0.5, 1.5, -0.84);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s18.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0.5, 1.5, -0.84);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(0.66, 1.5, -0.66);
	glVertex3f(0.84, 1.33, -0.66);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0.66, 1.33, -0.84);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s18.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-0.5, 1.5, -0.84);
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-0.66, 1.5, -0.66);
	glVertex3f(-0.84, 1.33, -0.66);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-0.66, 1.33, -0.84);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s19.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0.66, 1.33, -0.84);
	glVertex3f(0.84, 1.33, -0.66);
	glVertex3f(0.5, 1, -0.66);
	glVertex3f(0.5, 1.17, -0.84);
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//s19.l
	glVertex3f(-0.66, 1.33, -0.84);
	glVertex3f(-0.84, 1.33, -0.66);
	glVertex3f(-0.5, 1, -0.66);
	glVertex3f(-0.5, 1.17, -0.84);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s20.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0, 1, -0.84);
	glVertex3f(0.5, 1.17, -0.84);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.5, 1, -0.66);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0, 0.84, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s20.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0, 1, -0.84);
	glVertex3f(-0.5, 1.17, -0.84);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.5, 1, -0.66);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0, 0.84, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s21.r
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(0.66, 2.17, 0.5);
	glVertex3f(0.66, 2.66, 0.84);
	glVertex3f(0.66, 2, 0.84);
	glVertex3f(0.66, 1.66, 0.5);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s21.l
	glVertex3f(-0.66, 2.17, 0.5);
	glVertex3f(-0.66, 2.66, 0.84);
	glVertex3f(-0.66, 2, 0.84);
	glVertex3f(-0.66, 1.66, 0.5);
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//s21.5.r
	glVertex3f(0.17, 2.17, 0.5);
	glVertex3f(0.17, 2.66, 0.84);
	glVertex3f(0.17, 2, 0.84);
	glVertex3f(0.17, 1.66, 0.5);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s21.5l
	glVertex3f(-0.17, 2.17, 0.5);
	glVertex3f(-0.17, 2.66, 0.84);
	glVertex3f(-0.17, 2, 0.84);
	glVertex3f(-0.17, 1.66, 0.5);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s22.r
	glVertex3f(0.66, 1.66, 0.5);
	glVertex3f(0.66, 1.84, 0.66);
	glVertex3f(0.66, 1.33, 0.66);
	glVertex3f(0.66, 1, 0.5);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s22.l
	glVertex3f(-0.66, 1.66, 0.5);
	glVertex3f(-0.66, 1.84, 0.66);
	glVertex3f(-0.66, 1.33, 0.66);
	glVertex3f(-0.66, 1, 0.5);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s22.5.r
	glVertex3f(0.17, 1.66, 0.5);
	glVertex3f(0.17, 1.84, 0.66);
	glVertex3f(0.17, 1.33, 0.66);
	glVertex3f(0.17, 1, 0.5);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s22.5.l
	glVertex3f(-0.17, 1.66, 0.5);
	glVertex3f(-0.17, 1.84, 0.66);
	glVertex3f(-0.17, 1.33, 0.66);
	glVertex3f(-0.17, 1, 0.5);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s23.r
	glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	glVertex3f(0, 1.84, -0.66);
	glVertex3f(0.5, 2, -0.66);
	glVertex3f(0.5, 1.84, -0.84);
	glVertex3f(0, 1.66, -0.84);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s23.l
	glVertex3f(0, 1.84, -0.66);
	glVertex3f(-0.5, 2, -0.66);
	glVertex3f(-0.5, 1.84, -0.84);
	glVertex3f(0, 1.66, -0.84);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s24.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0, 4.84, -0.66);
	glVertex3f(0.58, 4.84, -0.66);
	glVertex3f(0.5, 4.66, -1);
	glVertex3f(0, 4.66, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s24.l
	glVertex3f(0, 4.84, -0.66);
	glVertex3f(-0.58, 4.84, -0.66);
	glVertex3f(-0.5, 4.66, -1);
	glVertex3f(0, 4.66, -1);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s25.r
	glVertex3f(0, 3.33, -1);
	glVertex3f(0.84, 3, -1);
	glVertex3f(0.84, 2.66, -0.66);
	glVertex3f(0, 3, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s25.l
	glVertex3f(0, 3.33, -1);
	glVertex3f(-0.84, 3, -1);
	glVertex3f(-0.84, 2.66, -0.66);
	glVertex3f(0, 3, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s26.r
	glVertex3f(0.84, 3, -1);
	glVertex3f(1.17, 2.66, -1);
	glVertex3f(1, 2.33, -0.66);
	glVertex3f(0.84, 2.66, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s26.l
	glVertex3f(-0.84, 3, -1);
	glVertex3f(-1.17, 2.66, -1);
	glVertex3f(-1, 2.33, -0.66);
	glVertex3f(-0.84, 2.66, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s27.r
	glVertex3f(1.5, 2.66, -0.66);
	glVertex3f(1.5, 2.66, 0.84);
	glVertex3f(1.17, 2.33, 0.66);
	glVertex3f(1.17, 2.33, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s27.l
	glVertex3f(-1.5, 2.66, -0.66);
	glVertex3f(-1.5, 2.66, 0.84);
	glVertex3f(-1.17, 2.33, 0.66);
	glVertex3f(-1.17, 2.33, -0.66);
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//s28.r
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(1.17, 2.33, 0);
	glVertex3f(1.17, 2.33, 0.66);
	glVertex3f(1.17, 2.17, 0.5);
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//s28.l
	glVertex3f(-1.17, 2.33, 0);
	glVertex3f(-1.17, 2.33, 0.66);
	glVertex3f(-1.17, 2.17, 0.5);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s29
	glVertex3f(1, 0.84, -0.5);
	glVertex3f(1, 0.84, 0.66);
	glVertex3f(-1, 0.84, 0.66);
	glVertex3f(-1, 0.84, -0.5);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s30.r
	glColor3f(0.9, 0.9, 0.9); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey9);
	glVertex3f(0.66, 5, -0.33);
	glVertex3f(0.66, 5, 0.33);
	glVertex3f(1, 5, 0.33);
	glVertex3f(1, 5, -0.33);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s30.l
	glVertex3f(-0.66, 5, -0.33);
	glVertex3f(-0.66, 5, 0.33);
	glVertex3f(-1, 5, 0.33);
	glVertex3f(-1, 5, -0.33);
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//s31.r
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(0.66, 5, -0.33);
	glVertex3f(0.66, 5, 0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.5, 4.66, 0.33);
	glVertex3f(0.5, 4.66, -0.33);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s31.l
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-0.66, 5, -0.33);
	glVertex3f(-0.66, 5, 0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.5, 4.66, 0.33);
	glVertex3f(-0.5, 4.66, -0.33);
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//s32.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0.66, 5, -0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.5, 4.66, -0.33);
	glVertex3f(0.66, 4.66, -0.66);
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//s32.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-0.66, 5, -0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.5, 4.66, -0.33);
	glVertex3f(-0.66, 4.66, -0.66);
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//s33.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0.66, 5, 0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.5, 4.66, 0.33);
	glVertex3f(0.66, 4.66, 0.66);
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//s33.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-0.66, 5, 0.33);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.5, 4.66, 0.33);
	glVertex3f(-0.66, 4.66, 0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s34.r
	glVertex3f(0, 4.66, -0.66);
	glVertex3f(0, 4.66, 0.84);
	glVertex3f(1.17, 4.66, 0.84);
	glVertex3f(1.17, 4.66, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s34.l
	glVertex3f(0, 4.66, -0.66);
	glVertex3f(0, 4.66, 0.84);
	glVertex3f(-1.17, 4.66, 0.84);
	glVertex3f(-1.17, 4.66, -0.66);
	glEnd();



	quadC++;    glBegin(GL_QUADS);		//s35.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1.17, 1.84, -0.66);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(1.17, 2, -0.87);
	glVertex3f(1.17, 3.33, -0.87);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1.17, 3.33, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s35.l
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1.17, 1.84, -0.66);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-1.17, 2, -0.87);
	glVertex3f(-1.17, 3.33, -0.87);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1.17, 3.33, -0.66);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s36.l
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.84, 2.33, -0.87);
	glVertex3f(0.84, 2.17, -0.66);
	glVertex3f(1.17, 1.84, -0.66);
	glVertex3f(1.17, 2, -0.87);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s36.r
	glVertex3f(-0.84, 2.33, -0.87);
	glVertex3f(-0.84, 2.17, -0.66);
	glVertex3f(-1.17, 1.84, -0.66);
	glVertex3f(-1.17, 2, -0.87);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s37.r
	glVertex3f(0, 2, -0.87);
	glVertex3f(0, 1.84, -0.66);
	glVertex3f(0.84, 2.17, -0.66);
	glVertex3f(0.84, 2.33, -0.87);
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//s37.l
	glVertex3f(0, 2, -0.87);
	glVertex3f(0, 1.84, -0.66);
	glVertex3f(-0.84, 2.17, -0.66);
	glVertex3f(-0.84, 2.33, -0.87);
	glEnd();
}


void bodyBack() {
	glBegin(GL_POLYGON);		//b1.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1.5, 4.5, 1);
	glVertex3f(1.5, 3, 1);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 3, 1);
	glVertex3f(0, 4.5, 1);
	glEnd();

	glBegin(GL_POLYGON);		//b1.l
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1.5, 4.5, 1);
	glVertex3f(-1.5, 3, 1);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 3, 1);
	glVertex3f(0, 4.5, 1);
	glEnd();

	glBegin(GL_POLYGON);		//b2.r
	glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	glVertex3f(0.66, 5, 0.33);
	glVertex3f(1, 5, 0.33);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1, 4.66, 0.66);
	glVertex3f(0.66, 4.66, 0.66);
	glEnd();

	glBegin(GL_POLYGON);		//b2.l
	glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	glVertex3f(-0.66, 5, 0.33);
	glVertex3f(-1, 5, 0.33);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1, 4.66, 0.66);
	glVertex3f(-0.66, 4.66, 0.66);
	glEnd();

	glBegin(GL_POLYGON);		//b3.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1.17, 4.66, 0.84);
	glVertex3f(1.5, 4.5, 1);
	glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	glVertex3f(0, 4.5, 1);
	glVertex3f(0, 4.66, 0.84);
	glEnd();

	glBegin(GL_POLYGON);		//b3.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1.17, 4.66, 0.84);
	glVertex3f(-1.5, 4.5, 1);
	glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	glVertex3f(-0, 4.5, 1);
	glVertex3f(-0, 4.66, 0.84);
	glEnd();

	glBegin(GL_POLYGON);		//b4.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1.5, 3, 1);
	glVertex3f(1.5, 2.66, 0.84);
	glVertex3f(1.5, 2.66, 0.84);
	glVertex3f(1.17, 2.33, 0.66);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0, 2.33, 0.66);
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0, 3, 1);
	glEnd();

	glBegin(GL_POLYGON);		//b4.l
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1.5, 3, 1);
	glVertex3f(-1.5, 2.66, 0.84);
	glVertex3f(-1.17, 2.33, 0.66);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0, 2.33, 0.66);
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0, 3, 1);
	glEnd();

	glBegin(GL_POLYGON);		//b5.r
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(1.17, 2.33, 0.66);
	glVertex3f(1.17, 2.17, 0.5);
	glVertex3f(0, 2.17, 0.5);
	glVertex3f(0, 2.33, 0.66);
	glEnd();

	glBegin(GL_POLYGON);		//b5.l
	glVertex3f(-1.17, 2.33, 0.66);
	glVertex3f(-1.17, 2.17, 0.5);
	glVertex3f(0, 2.17, 0.5);
	glVertex3f(0, 2.33, 0.66);
	glEnd();

	glBegin(GL_POLYGON);		//b6.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1.17, 2.17, 0.5);
	glVertex3f(1.17, 1.66, 0.5);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 1.66, 0.5);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0, 2.17, 0.5);
	glEnd();

	glBegin(GL_POLYGON);		//b6.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1.17, 2.17, 0.5);
	glVertex3f(-1.17, 1.66, 0.5);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 1.66, 0.5);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0, 2.17, 0.5);
	glEnd();

	glBegin(GL_POLYGON);		//b7.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1.17, 1.66, 0.5);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0.84, 1, 0.5);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0, 1, 0.5);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 1.66, 0.5);
	glEnd();

	glBegin(GL_POLYGON);		//b7.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1.17, 1.66, 0.5);
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-0.84, 1, 0.5);
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(0, 1, 0.5);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 1.66, 0.5);
	glEnd();

	glBegin(GL_POLYGON);		//b8.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.84, 1, 0.5);
	glVertex3f(0.84, 0.5, 0.5);
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0, 0.5, 0.5);
	glVertex3f(0, 1, 0.5);
	glEnd();

	glBegin(GL_POLYGON);		//b8.l
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.84, 1, 0.5);
	glVertex3f(-0.84, 0.5, 0.5);
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0, 0.5, 0.5);
	glVertex3f(0, 1, 0.5);
	glEnd();

	glBegin(GL_POLYGON);		//b9.r
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(1, 0.84, 0.66);
	glVertex3f(0, 0.84, 0.66);
	glVertex3f(0, 0, 1);
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(1.5, 0, 1);
	glEnd();

	glBegin(GL_POLYGON);		//b9.l
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glVertex3f(-1, 0.84, 0.66);
	glVertex3f(-0, 0.84, 0.66);
	glVertex3f(-0, 0, 1);
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-1.5, 0, 1);
	glEnd();

	glBegin(GL_POLYGON);		//b10.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.66, 2.66, 0.84);
	glVertex3f(0.66, 2, 0.84);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 2, 0.84);
	glVertex3f(0, 2.66, 0.84);
	glEnd();

	glBegin(GL_POLYGON);		//b10.l
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.66, 2.66, 0.84);
	glVertex3f(-0.66, 2, 0.84);
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, 2, 0.84);
	glVertex3f(0, 2.66, 0.84);
	glEnd();

	glBegin(GL_POLYGON);		//b11.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.66, 2, 0.84);
	glVertex3f(0.66, 1.66, 0.5);
	glVertex3f(0, 1.66, 0.5);
	glVertex3f(0, 2, 0.84);
	glEnd();

	glBegin(GL_POLYGON);		//b11.r
	glVertex3f(-0.66, 2, 0.84);
	glVertex3f(-0.66, 1.66, 0.5);
	glVertex3f(0, 1.66, 0.5);
	glVertex3f(0, 2, 0.84);
	glEnd();

	glBegin(GL_POLYGON);		//b12.r
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0.66, 1.84, 0.66);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.66, 1.33, 0.66);
	glVertex3f(0.17, 1.33, 0.66);
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(0.17, 1.84, 0.66);
	glEnd();

	glBegin(GL_POLYGON);		//b12.l
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(-0.66, 1.84, 0.66);
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.66, 1.33, 0.66);
	glVertex3f(-0.17, 1.33, 0.66);
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(-0.17, 1.84, 0.66);
	glEnd();

	glBegin(GL_POLYGON);		//b13.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.66, 1.33, 0.66);
	glVertex3f(0.66, 1, 0.5);
	glVertex3f(0.17, 1, 0.5);
	glVertex3f(0.17, 1.33, 0.66);
	glEnd();


	glBegin(GL_POLYGON);		//b13.l
	glVertex3f(-0.66, 1.33, 0.66);
	glVertex3f(-0.66, 1, 0.5);
	glVertex3f(-0.17, 1, 0.5);
	glVertex3f(-0.17, 1.33, 0.66);
	glEnd();
}

void bodyHeart() {

	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glTranslatef(0, 2.5, -0.95);
	drawDonut(0.2, 0.3, 0.17, 2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glTranslatef(0, 2.5, -0.85);
	heartRotate += 0.1;
	glRotatef(heartRotate, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	drawSphere(0.2, 15, 15, 1);
	glPopMatrix();

	/*
	glPushMatrix();
	glTranslatef(0, 1.5, -0.33);

	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	glTranslatef(0, 2.5, -0.77);
	drawDonut(0.2, 0.3, 0.17, 2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glTranslatef(0, 2.5, -0.66);
	heartRotate += 0.1;
	glRotatef(heartRotate, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	drawSphere(0.2, 20, 20, 1);
	glPopMatrix();

	glPopMatrix();
	*/

}

void upperBody() {

	glPushMatrix();
	glScalef(1, 0.72, 1);
	glRotatef(180, 0, 1, 0);

	bodyFront();
	bodySide();
	bodyBack();
	bodyHeart();

	quadC++;    glBegin(GL_QUADS);
	glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glVertex3f(-1.5, 0, -1); //bottom connect face
	glVertex3f(1.5, 0, -1);
	glVertex3f(1.5, 0, 1);
	glVertex3f(-1.5, 0, 1);
	glEnd();

	glPopMatrix();
}


//body end-----------------------------------------------------------------------------------

// wing -----------------------------------------------------------------------------------

void wing1LeftSide() {
	glPushMatrix();

	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	quadC++;    glBegin(GL_QUADS);		//w.s1
	glVertex3f(-0.25, 0.25, -0.25);		//a.r
	glVertex3f(-0.25, 0.25, 0.25);		//a.l
	glVertex3f(-0.25, -0.25, 0.25);		//e.l
	glVertex3f(-0.25, -0.25, -0.25);	//e.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s2
	glVertex3f(-0.25, 0.25, -0.25);		//a.r
	glVertex3f(-0.25, 0.25, 0.25);		//a.l
	glVertex3f(0.25, 0.25, 0.25);		//b.l
	glVertex3f(0.25, 0.25, -0.25);		//b.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s3
	glVertex3f(0.25, 0.25, -0.25);			//b.r
	glVertex3f(0.25, 0.25, 0.25);			//b.l
	glVertex3f(0.5, 0, 0.25);			//c1.l
	glVertex3f(0.5, 0, -0.25);			//c1.r
	glEnd();

	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	quadC++;    glBegin(GL_QUADS);		//w.s4
	glVertex3f(0.5, 0, -0.125);			//c2.r
	glVertex3f(0.5, 0, 0.125);			//c2.l
	glVertex3f(2.5, -1, 0.125);			//j1.l
	glVertex3f(2.5, -1, -0.125);		//j1.r
	glEnd();

	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	quadC++;    glBegin(GL_QUADS);		//w.s5
	glVertex3f(2.5, -1, -0.06);			//j2.r
	glVertex3f(2.5, -1, 0.06);			//j2.l
	glVertex3f(2.625, -1.25, 0.06);	//o.l
	glVertex3f(2.625, -1.25, -0.06);	//o.r
	glEnd();

	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	quadC++;    glBegin(GL_QUADS);		//w.s6
	glVertex3f(2.625, -1.25, -0.06);	//o.r
	glVertex3f(2.625, -1.25, 0.06);		//o.l
	glVertex3f(3, -2, 0.06);			//p.l
	glVertex3f(3, -2, -0.06);			//p.r
	glEnd();

	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	quadC++;    glBegin(GL_QUADS);		//w.s7
	glVertex3f(3, -2, -0.06);			//p.r
	glVertex3f(3, -2, 0.06);			//p.l
	glVertex3f(2, -2, 0.06);			//q.l
	glVertex3f(2, -2, -0.06);			//q.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s8
	glVertex3f(1.5, -1.5, -0.06);		//r.r
	glVertex3f(1.5, -1.5, 0.06);		//r.l
	glVertex3f(2, -2, 0.06);			//q.l
	glVertex3f(2, -2, -0.06);			//q.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s9
	glVertex3f(1.25, -1.5, -0.06);		//s.r
	glVertex3f(1.25, -1.5, 0.06);		//s.l
	glVertex3f(1.5, -1.5, 0.06);		//r.l
	glVertex3f(1.5, -1.5, -0.06);		//r.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s10
	glVertex3f(0.75, -1, -0.06);		//t.r
	glVertex3f(0.75, -1, 0.06);			//t.l
	glVertex3f(1.25, -1.5, 0.06);		//s.l
	glVertex3f(1.25, -1.5, -0.06);		//s.r
	glEnd();

	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	quadC++;    glBegin(GL_QUADS);		//w.s11
	glVertex3f(0.5, -1, -0.125);		//v2.r
	glVertex3f(0.5, -1, 0.125);			//v2.l
	glVertex3f(0.75, -1, 0.125);		//t2.l
	glVertex3f(0.75, -1, -0.125);		//t2.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s12
	glVertex3f(0.375, -0.75, -0.125);	//w2.r
	glVertex3f(0.375, -0.75, 0.125);	//w2.l
	glVertex3f(0.5, -1, 0.125);			//v2.l
	glVertex3f(0.5, -1, -0.125);		//v2.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s13
	glVertex3f(0.375, -0.5, -0.125);	//x2.r
	glVertex3f(0.375, -0.5, 0.125);		//x2.l
	glVertex3f(0.375, -0.75, 0.125);	//w2.l
	glVertex3f(0.375, -0.75, -0.125);	//w2.r
	glEnd();

	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	quadC++;    glBegin(GL_QUADS);		//w.s14
	glVertex3f(0, -0.25, -0.125);		//y2.r
	glVertex3f(0, -0.25, 0.125);		//y2.l
	glVertex3f(0.375, -0.5, 0.06);		//x1.l
	glVertex3f(0.375, -0.5, -0.06);		//x1.r
	glEnd();

	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	quadC++;    glBegin(GL_QUADS);		//w.s15
	glVertex3f(-0.25, -0.25, -0.25);	//e1.r
	glVertex3f(-0.25, -0.25, 0.25);		//e1.l
	glVertex3f(0.25, -0.25, 0.25);		//d1.l
	glVertex3f(0.25, -0.25, -0.25);		//d1.r	
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s17
	glVertex3f(1.5, -0.75, -0.125);		//i1.r
	glVertex3f(1.5, -0.75, 0.125);		//i1.l
	glVertex3f(2.5, -1, 0.125);			//j1.l
	glVertex3f(2.5, -1, -0.125);		//j1.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s18
	glVertex3f(1, -0.5, -0.125);		//h1.r
	glVertex3f(1, -0.5, 0.125);			//h1.l
	glVertex3f(1.5, -0.75, 0.125);		//i1.l
	glVertex3f(1.5, -0.75, -0.125);		//i1.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s19
	glVertex3f(1, -0.5, -0.125);		//g1.r
	glVertex3f(1, -0.5, 0.125);			//g1.l
	glVertex3f(1, -0.5, 0.125);			//h1.l
	glVertex3f(1, -0.5, -0.125);		//h1.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s21
	glVertex3f(0.25, -0.25, -0.125);	//d2.r	
	glVertex3f(0.25, -0.25, 0.125);		//d2.l
	glVertex3f(1, -0.5, 0.125);			//g1.l
	glVertex3f(1, -0.5, -0.125);		//g1.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s22
	glVertex3f(0.25, -0.25, -0.25);		//d1.r	
	glVertex3f(0.25, -0.25, 0.25);		//d1.l
	glVertex3f(0.5, 0, 0.25);			//c1.l
	glVertex3f(0.5, 0, -0.25);			//c1.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s23
	glVertex3f(0.75, -1, -0.125);		//t2.r
	glVertex3f(0.75, -1, 0.125);		//t2.l
	glVertex3f(0.75, -0.75, 0.125);		//u2.l
	glVertex3f(0.75, -0.75, -0.125);	//u2.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s24
	glVertex3f(0.375, -0.75, -0.125);	//x2.r
	glVertex3f(0.375, -0.75, -0.125);	//x2.l
	glVertex3f(0.75, -0.75, -0.125);	//u2.r
	glVertex3f(0.75, -0.75, 0.125);		//u2.l
	glEnd();

	glPopMatrix();
}

void wing1RightSide() {
	glPushMatrix();

	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	quadC++;    glBegin(GL_QUADS);		//w.s1
	glVertex3f(0.25, 0.25, -0.25);		//a.r
	glVertex3f(0.25, 0.25, 0.25);		//a.l
	glVertex3f(0.25, -0.25, 0.25);		//e.l
	glVertex3f(0.25, -0.25, -0.25);	//e.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s2
	glVertex3f(0.25, 0.25, -0.25);		//a.r
	glVertex3f(0.25, 0.25, 0.25);		//a.l
	glVertex3f(-0.25, 0.25, 0.25);		//b.l
	glVertex3f(-0.25, 0.25, -0.25);		//b.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s3
	glVertex3f(-0.25, 0.25, -0.25);			//b.r
	glVertex3f(-0.25, 0.25, 0.25);			//b.l
	glVertex3f(-0.5, 0, 0.25);			//c1.l
	glVertex3f(-0.5, 0, -0.25);			//c1.r
	glEnd();

	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	quadC++;    glBegin(GL_QUADS);		//w.s4
	glVertex3f(-0.5, 0, -0.125);			//c2.r
	glVertex3f(-0.5, 0, 0.125);			//c2.l
	glVertex3f(-2.5, -1, 0.125);			//j1.l
	glVertex3f(-2.5, -1, -0.125);		//j1.r
	glEnd();

	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	quadC++;    glBegin(GL_QUADS);		//w.s5
	glVertex3f(-2.5, -1, -0.06);			//j2.r
	glVertex3f(-2.5, -1, 0.06);			//j2.l
	glVertex3f(-2.625, -1.25, 0.06);	//o.l
	glVertex3f(-2.625, -1.25, -0.06);	//o.r
	glEnd();

	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	quadC++;    glBegin(GL_QUADS);		//w.s6
	glVertex3f(-2.625, -1.25, -0.06);	//o.r
	glVertex3f(-2.625, -1.25, 0.06);		//o.l
	glVertex3f(-3, -2, 0.06);			//p.l
	glVertex3f(-3, -2, -0.06);			//p.r
	glEnd();

	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	quadC++;    glBegin(GL_QUADS);		//w.s7
	glVertex3f(-3, -2, -0.06);			//p.r
	glVertex3f(-3, -2, 0.06);			//p.l
	glVertex3f(-2, -2, 0.06);			//q.l
	glVertex3f(-2, -2, -0.06);			//q.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s8
	glVertex3f(-1.5, -1.5, -0.06);		//r.r
	glVertex3f(-1.5, -1.5, 0.06);		//r.l
	glVertex3f(-2, -2, 0.06);			//q.l
	glVertex3f(-2, -2, -0.06);			//q.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s9
	glVertex3f(-1.25, -1.5, -0.06);		//s.r
	glVertex3f(-1.25, -1.5, 0.06);		//s.l
	glVertex3f(-1.5, -1.5, 0.06);		//r.l
	glVertex3f(-1.5, -1.5, -0.06);		//r.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s10
	glVertex3f(-0.75, -1, -0.06);		//t.r
	glVertex3f(-0.75, -1, 0.06);			//t.l
	glVertex3f(-1.25, -1.5, 0.06);		//s.l
	glVertex3f(-1.25, -1.5, -0.06);		//s.r
	glEnd();

	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	quadC++;    glBegin(GL_QUADS);		//w.s11
	glVertex3f(-0.5, -1, -0.125);		//v2.r
	glVertex3f(-0.5, -1, 0.125);			//v2.l
	glVertex3f(-0.75, -1, 0.125);		//t2.l
	glVertex3f(-0.75, -1, -0.125);		//t2.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s12
	glVertex3f(-0.375, -0.75, -0.125);	//w2.r
	glVertex3f(-0.375, -0.75, 0.125);	//w2.l
	glVertex3f(-0.5, -1, 0.125);			//v2.l
	glVertex3f(-0.5, -1, -0.125);		//v2.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s13
	glVertex3f(-0.375, -0.5, -0.125);	//x2.r
	glVertex3f(-0.375, -0.5, 0.125);		//x2.l
	glVertex3f(-0.375, -0.75, 0.125);	//w2.l
	glVertex3f(-0.375, -0.75, -0.125);	//w2.r
	glEnd();

	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	quadC++;    glBegin(GL_QUADS);		//w.s14
	glVertex3f(0, -0.25, -0.125);		//y2.r
	glVertex3f(0, -0.25, 0.125);		//y2.l
	glVertex3f(-0.375, -0.5, 0.06);		//x1.l
	glVertex3f(-0.375, -0.5, -0.06);		//x1.r
	glEnd();

	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	quadC++;    glBegin(GL_QUADS);		//w.s15
	glVertex3f(0.25, -0.25, -0.25);	//e1.r
	glVertex3f(0.25, -0.25, 0.25);		//e1.l
	glVertex3f(-0.25, -0.25, 0.25);		//d1.l
	glVertex3f(-0.25, -0.25, -0.25);		//d1.r	
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s17
	glVertex3f(-1.5, -0.75, -0.125);		//i1.r
	glVertex3f(-1.5, -0.75, 0.125);		//i1.l
	glVertex3f(-2.5, -1, 0.125);			//j1.l
	glVertex3f(-2.5, -1, -0.125);		//j1.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s18
	glVertex3f(-1, -0.5, -0.125);		//h1.r
	glVertex3f(-1, -0.5, 0.125);			//h1.l
	glVertex3f(-1.5, -0.75, 0.125);		//i1.l
	glVertex3f(-1.5, -0.75, -0.125);		//i1.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s19
	glVertex3f(-1, -0.5, -0.125);		//g1.r
	glVertex3f(-1, -0.5, 0.125);			//g1.l
	glVertex3f(-1, -0.5, 0.125);			//h1.l
	glVertex3f(-1, -0.5, -0.125);		//h1.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s21
	glVertex3f(-0.25, -0.25, -0.125);	//d2.r	
	glVertex3f(-0.25, -0.25, 0.125);		//d2.l
	glVertex3f(-1, -0.5, 0.125);			//g1.l
	glVertex3f(-1, -0.5, -0.125);		//g1.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s22
	glVertex3f(-0.25, -0.25, -0.25);		//d1.r	
	glVertex3f(-0.25, -0.25, 0.25);		//d1.l
	glVertex3f(-0.5, 0, 0.25);			//c1.l
	glVertex3f(-0.5, 0, -0.25);			//c1.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s23
	glVertex3f(-0.75, -1, -0.125);		//t2.r
	glVertex3f(-0.75, -1, 0.125);		//t2.l
	glVertex3f(-0.75, -0.75, 0.125);		//u2.l
	glVertex3f(-0.75, -0.75, -0.125);	//u2.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.s24
	glVertex3f(-0.375, -0.75, -0.125);	//x2.r
	glVertex3f(-0.375, -0.75, -0.125);	//x2.l
	glVertex3f(-0.75, -0.75, -0.125);	//u2.r
	glVertex3f(-0.75, -0.75, 0.125);		//u2.l
	glEnd();

	glPopMatrix();
}

void wing1Left() {
	glPushMatrix();

	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//w.f1.r
	if (isTexture == 0)
	{
		glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.75);
	}
	glVertex3f(-0.25, 0.25, 0.25);		//a
	if (isTexture == 0)
	{
		glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.75, 0.75);
	}
	glVertex3f(0.25, 0.25, 0.25);		//b
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 0.5);
	}
	glVertex3f(0.5, 0, 0.25);			//c1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.75, 0.25);
	}
	glVertex3f(0.25, -0.25, 0.25);		//d1
	if (isTexture == 0)
	{
		glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.25);
	}
	glVertex3f(-0.25, -0.25, 0.25);	//e
	glEnd();
	glDeleteTextures(1, &textures[0]);




	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//w.f1.r
	if (isTexture == 0)
	{
		glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.75);
	}
	glVertex3f(-0.25, 0.25, -0.25);		//a
	if (isTexture == 0)
	{
		glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.75, 0.75);
	}
	glVertex3f(0.25, 0.25, -0.25);		//b
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 0.5);
	}
	glVertex3f(0.5, 0, -0.25);			//c1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.75, 0.25);
	}
	glVertex3f(0.25, -0.25, -0.25);		//d1
	if (isTexture == 0)
	{
		glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.25);
	}
	glVertex3f(-0.25, -0.25, -0.25);	//e
	glEnd();
	glDeleteTextures(1, &textures[0]);





	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//w.f2.r
	if (isTexture == 0)
	{
		glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else {

		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0.5);
	}
	glVertex3f(0, 0, 0.125);			//f1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.1, 0.5);
	}
	glVertex3f(0.5, 0, 0.125);			//c2
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.3);
	}
	glVertex3f(2.5, -1, 0.125);		//j1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.3, 0.35);
	}
	glVertex3f(1.5, -0.75, 0.125);		//i
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.2, 0.4);
	}
	glVertex3f(1, -0.5, 0.125);		//h
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.1, 0.4);
	}
	glVertex3f(0.5, -0.5, 0.125);		//g1
	glEnd();
	glDeleteTextures(1, &textures[0]);




	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//w.f2.l
	if (isTexture == 0)
	{
		glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else {

		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0.5);
	}
	glVertex3f(0, 0, -0.125);			//f1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.1, 0.5);
	}
	glVertex3f(0.5, 0, -0.125);			//c2
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.3);
	}
	glVertex3f(2.5, -1, -0.125);		//j1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.3, 0.35);
	}
	glVertex3f(1.5, -0.75, -0.125);		//i
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.2, 0.4);
	}
	glVertex3f(1, -0.5, -0.125);		//h
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.1, 0.4);
	}
	glVertex3f(0.5, -0.5, -0.125);		//g1
	glEnd();
	glDeleteTextures(1, &textures[0]);






	quadC++;    glBegin(GL_QUADS);		//w.f3.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(1.75, -1, 0.06);		//m
	glVertex3f(2.5, -1, 0.06);			//j2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(2.625, -1.25, 0.06);	//o
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(2, -1.25, 0.06);		//n
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f3.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(1.75, -1, -0.06);			//m
	glVertex3f(2.5, -1, -0.06);			//j2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(2.625, -1.25, -0.06);		//o
	glVertex3f(2, -1.25, -0.06);			//n
	glEnd();


	triangleC++;    glBegin(GL_TRIANGLES);		//w.f3.1.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0.5, -0.5, 0.06);		//g
	glVertex3f(1, -0.5, 0.06);			//h2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(1, -0.625, 0.06);		//k		
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//w.f3.1.l
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(0.5, -0.5, -0.06);		//g
	glVertex3f(1, -0.5, -0.06);			//h2
	glVertex3f(1, -0.625, -0.06);		//k		
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f3.2.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(1, -0.5, 0.06);			//h2
	glVertex3f(1, -0.5, 0.06);			//i2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(1.75, -1, 0.06);		//m
	glVertex3f(1.5, -1, 0.06);			//l
	glVertex3f(1, -0.625, 0.06);		//k
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f3.2.l
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(1, -0.5, -0.06);			//h2
	glVertex3f(1, -0.5, -0.06);			//i2
	glVertex3f(1.75, -1, -0.06);			//m
	glVertex3f(1.5, -1, -0.06);			//l
	glVertex3f(1, -0.625, -0.06);		//k
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//w.f3.3.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(1, -0.5, 0.06);			//i2
	glVertex3f(2.5, -1, 0.06);			//j2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(1.75, -1, 0.06);		//m
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//w.f3.3.l
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(1, -0.5, -0.06);			//i2
	glVertex3f(2.5, -1, -0.06);			//j2
	glVertex3f(1.75, -1, -0.06);		//m
	glEnd();






	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	quadC++;    glBegin(GL_QUADS);		//w.f4.r
	if (isTexture == 0)
	{
		glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	}
	else {

		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0.75);
	}
	glVertex3f(2, -1.25, 0.06);		//n
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.625, 0.75);
	}
	glVertex3f(2.625, -1.25, 0.06);	//o
	if (isTexture == 0)
	{
		glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 0);
	}
	glVertex3f(3, -2, 0.06);			//p
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0);
	}
	glVertex3f(2, -2, 0.06);			//q
	glEnd();
	glDeleteTextures(1, &textures[0]);





	quadC++;    glBegin(GL_QUADS);		//w.f4.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(2, -1.25, -0.06);			//n
	glVertex3f(2.625, -1.25, -0.06);		//o
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(3, -2, -0.06);			//p
	glVertex3f(2, -2, -0.06);			//q
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f4.1.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(1.5, -1, 0.06);			//l
	glVertex3f(1.75, -1, 0.06);		//m
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(2, -1.25, 0.06);		//n
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(2, -2, 0.06);			//q
	glVertex3f(1.5, -1.5, 0.06);		//r
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f4.1.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(1.5, -1, -0.06);			//l
	glVertex3f(1.75, -1, -0.06);		//m
	glVertex3f(2, -1.25, -0.06);		//n
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(2, -2, -0.06);			//q
	glVertex3f(1.5, -1.5, -0.06);		//r
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f5.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(1, -0.625, 0.06);		//k
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(1.5, -1, 0.06);			//l
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(1.5, -1.5, 0.06);		//r
	glVertex3f(1.25, -1.5, 0.06);		//s
	glVertex3f(0.75, -1, 0.06);		//t
	glVertex3f(0.75, -0.75, 0.06);		//u
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f5.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(1, -0.625, -0.06);		//k
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(1.5, -1, -0.06);			//l
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(1.5, -1.5, -0.06);		//r
	glVertex3f(1.25, -1.5, -0.06);		//s
	glVertex3f(0.75, -1, -0.06);			//t
	glVertex3f(0.75, -0.75, -0.06);		//u
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f6.r
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(0.375, -0.5, 0.125);		//x
	glVertex3f(0.75, -0.75, 0.125);		//u
	glVertex3f(0.75, -1, 0.125);		//t
	glVertex3f(0.5, -1, 0.125);			//v
	glVertex3f(0.375, -0.75, 0.125);	//w
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f6.l
	glVertex3f(0.375, -0.5, -0.125);		//x
	glVertex3f(0.75, -0.75, -0.125);		//u
	glVertex3f(0.75, -1, -0.125);		//t
	glVertex3f(0.5, -1, -0.125);			//v
	glVertex3f(0.375, -0.75, -0.125);	//w
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f7.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0.5, -0.5, 0.06);		//g
	glVertex3f(1, -0.625, 0.06);		//k
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.75, -0.75, 0.06);		//u
	glVertex3f(0.375, -0.5, 0.06);		//x
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f7.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0.5, -0.5, -0.06);		//g
	glVertex3f(1, -0.625, -0.06);		//k
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0.75, -0.75, -0.06);		//u
	glVertex3f(0.375, -0.5, -0.06);		//x
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f7.1.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, -0.25, 0.06);		//y
	glVertex3f(0.25, -0.25, 0.06);		//d2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0.5, -0.5, 0.06);		//g
	glVertex3f(0.375, -0.5, 0.06);		//x
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f7.1.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, -0.25, -0.06);			//y
	glVertex3f(0.25, -0.25, -0.06);		//d2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0.5, -0.5, -0.06);		//g
	glVertex3f(0.375, -0.5, -0.06);		//x
	glEnd();

	glPopMatrix();
}

void wing1Right() {
	glPushMatrix();
	//glRotatef(180, 0, 1, 0);
	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//w.f1.r
	if (isTexture == 0)
	{
		glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.75);
	}
	glVertex3f(0.25, 0.25, 0.25);		//a
	if (isTexture == 0)
	{
		glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.75, 0.75);
	}
	glVertex3f(-0.25, 0.25, 0.25);		//b
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 0.5);
	}
	glVertex3f(-0.5, 0, 0.25);			//c1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.75, 0.25);
	}
	glVertex3f(-0.25, -0.25, 0.25);		//d1
	if (isTexture == 0)
	{
		glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.25);
	}
	glVertex3f(0.25, -0.25, 0.25);	//e
	glEnd();
	glDeleteTextures(1, &textures[0]);




	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//w.f1.r
	if (isTexture == 0)
	{
		glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.75);
	}
	glVertex3f(0.25, 0.25, -0.25);		//a
	if (isTexture == 0)
	{
		glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.75, 0.75);
	}
	glVertex3f(-0.25, 0.25, -0.25);		//b
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 0.5);
	}
	glVertex3f(-0.5, 0, -0.25);			//c1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.75, 0.25);
	}
	glVertex3f(-0.25, -0.25, -0.25);		//d1
	if (isTexture == 0)
	{
		glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.25, 0.25);
	}
	glVertex3f(0.25, -0.25, -0.25);	//e
	glEnd();
	glDeleteTextures(1, &textures[0]);





	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//w.f2.r
	if (isTexture == 0)
	{
		glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0.5);
	}
	glVertex3f(0, 0, 0.125);			//f1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.1, 0.5);
	}
	glVertex3f(-0.5, 0, 0.125);			//c2
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.3);
	}
	glVertex3f(-2.5, -1, 0.125);		//j1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.3, 0.35);
	}
	glVertex3f(-1.5, -0.75, 0.125);		//i
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.2, 0.4);
	}
	glVertex3f(-1, -0.5, 0.125);		//h
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.1, 0.4);
	}
	glVertex3f(-0.5, -0.5, 0.125);		//g1
	glEnd();
	glDeleteTextures(1, &textures[0]);




	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	pc++;  	glBegin(GL_POLYGON);		//w.f2.l
	if (isTexture == 0)
	{
		glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0.5);
	}
	glVertex3f(0, 0, -0.125);			//f1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.1, 0.5);
	}
	glVertex3f(-0.5, 0, -0.125);			//c2
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.5, 0.3);
	}
	glVertex3f(-2.5, -1, -0.125);		//j1
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.3, 0.35);
	}
	glVertex3f(-1.5, -0.75, -0.125);		//i
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.2, 0.4);
	}
	glVertex3f(-1, -0.5, -0.125);		//h
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.1, 0.4);
	}
	glVertex3f(-0.5, -0.5, -0.125);		//g1
	glEnd();
	glDeleteTextures(1, &textures[0]);


	quadC++;    glBegin(GL_QUADS);		//w.f3.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-1.75, -1, 0.06);		//m
	glVertex3f(-2.5, -1, 0.06);			//j2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-2.625, -1.25, 0.06);	//o
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-2, -1.25, 0.06);		//n
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f3.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-1.75, -1, -0.06);			//m
	glVertex3f(-2.5, -1, -0.06);			//j2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-2.625, -1.25, -0.06);		//o
	glVertex3f(-2, -1.25, -0.06);			//n
	glEnd();


	triangleC++;    glBegin(GL_TRIANGLES);		//w.f3.1.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0.5, -0.5, 0.06);		//g
	glVertex3f(-1, -0.5, 0.06);			//h2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-1, -0.625, 0.06);		//k		
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//w.f3.1.l
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-0.5, -0.5, -0.06);		//g
	glVertex3f(-1, -0.5, -0.06);			//h2
	glVertex3f(-1, -0.625, -0.06);		//k		
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f3.2.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-1, -0.5, 0.06);			//h2
	glVertex3f(-1, -0.5, 0.06);			//i2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-1.75, -1, 0.06);		//m
	glVertex3f(-1.5, -1, 0.06);			//l
	glVertex3f(-1, -0.625, 0.06);		//k
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f3.2.l
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-1, -0.5, -0.06);			//h2
	glVertex3f(-1, -0.5, -0.06);			//i2
	glVertex3f(-1.75, -1, -0.06);			//m
	glVertex3f(-1.5, -1, -0.06);			//l
	glVertex3f(-1, -0.625, -0.06);		//k
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//w.f3.3.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-1, -0.5, 0.06);			//i2
	glVertex3f(-2.5, -1, 0.06);			//j2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-1.75, -1, 0.06);		//m
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//w.f3.3.l
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-1, -0.5, -0.06);			//i2
	glVertex3f(-2.5, -1, -0.06);			//j2
	glVertex3f(-1.75, -1, -0.06);		//m
	glEnd();




	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	quadC++;    glBegin(GL_QUADS);		//w.f4.r
	if (isTexture == 0)
	{
		glColor3f(0.3, 0.3, 0.3); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey3);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0.75);
	}
	glVertex3f(-2, -1.25, 0.06);		//n
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0.625, 0.75);
	}
	glVertex3f(-2.625, -1.25, 0.06);	//o
	if (isTexture == 0)
	{
		glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 0);
	}
	glVertex3f(-3, -2, 0.06);			//p
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0);
	}
	glVertex3f(-2, -2, 0.06);			//q
	glEnd();
	glDeleteTextures(1, &textures[0]);





	quadC++;    glBegin(GL_QUADS);		//w.f4.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-2, -1.25, -0.06);			//n
	glVertex3f(-2.625, -1.25, -0.06);		//o
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-3, -2, -0.06);			//p
	glVertex3f(-2, -2, -0.06);			//q
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f4.1.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-1.5, -1, 0.06);			//l
	glVertex3f(-1.75, -1, 0.06);		//m
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-2, -1.25, 0.06);		//n
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-2, -2, 0.06);			//q
	glVertex3f(-1.5, -1.5, 0.06);		//r
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f4.1.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-1.5, -1, -0.06);			//l
	glVertex3f(-1.75, -1, -0.06);		//m
	glVertex3f(-2, -1.25, -0.06);		//n
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-2, -2, -0.06);			//q
	glVertex3f(-1.5, -1.5, -0.06);		//r
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f5.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-1, -0.625, 0.06);		//k
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(-1.5, -1, 0.06);			//l
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-1.5, -1.5, 0.06);		//r
	glVertex3f(-1.25, -1.5, 0.06);		//s
	glVertex3f(-0.75, -1, 0.06);		//t
	glVertex3f(-0.75, -0.75, 0.06);		//u
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f5.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-1, -0.625, -0.06);		//k
	glColor3f(0.8, 0.8, 0.8); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey8);
	glVertex3f(-1.5, -1, -0.06);			//l
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-1.5, -1.5, -0.06);		//r
	glVertex3f(-1.25, -1.5, -0.06);		//s
	glVertex3f(-0.75, -1, -0.06);			//t
	glVertex3f(-0.75, -0.75, -0.06);		//u
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w.f6.r
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-0.375, -0.5, 0.125);		//x
	glVertex3f(-0.75, -0.75, 0.125);		//u
	glVertex3f(-0.75, -1, 0.125);		//t
	glVertex3f(-0.5, -1, 0.125);			//v
	glVertex3f(-0.375, -0.75, 0.125);	//w
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f6.l
	glVertex3f(-0.375, -0.5, -0.125);		//x
	glVertex3f(-0.75, -0.75, -0.125);		//u
	glVertex3f(-0.75, -1, -0.125);		//t
	glVertex3f(-0.5, -1, -0.125);			//v
	glVertex3f(-0.375, -0.75, -0.125);	//w
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f7.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0.5, -0.5, 0.06);		//g
	glVertex3f(-1, -0.625, 0.06);		//k
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.75, -0.75, 0.06);		//u
	glVertex3f(-0.375, -0.5, 0.06);		//x
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f7.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0.5, -0.5, -0.06);		//g
	glVertex3f(-1, -0.625, -0.06);		//k
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-0.75, -0.75, -0.06);		//u
	glVertex3f(-0.375, -0.5, -0.06);		//x
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f7.1.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0, -0.25, 0.06);		//y
	glVertex3f(-0.25, -0.25, 0.06);		//d2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-0.5, -0.5, 0.06);		//g
	glVertex3f(-0.375, -0.5, 0.06);		//x
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w.f7.1.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0, -0.25, -0.06);			//y
	glVertex3f(-0.25, -0.25, -0.06);		//d2
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-0.5, -0.5, -0.06);		//g
	glVertex3f(-0.375, -0.5, -0.06);		//x
	glEnd();

	glPopMatrix();
}

void wing2Left() {
	glPushMatrix();

	pc++;  	glBegin(GL_POLYGON);		//w2.f1.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0.5, 0, -0.125);		//c
	glVertex3f(-0.25, 0.25, -0.125);	//a
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(0.25, 0.25, -0.125);		//b
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0.25, -0.25, -0.125);	//e
	glVertex3f(-0.25, -0.25, -0.125);	//d
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w2.f1.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0.5, 0, 0.125);			//c
	glVertex3f(-0.25, 0.25, 0.125);		//a
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(0.25, 0.25, 0.125);		//b
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0.25, -0.25, 0.125);		//e
	glVertex3f(-0.25, -0.25, 0.125);	//d
	glEnd();

	quadC++;    glBegin(GL_QUADS);			//w2.f2.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.25, 0, -0.06);			//f
	glVertex3f(0.625, -0.25, -0.06);	//g
	glVertex3f(0.625, -0.5, -0.06);		//h
	glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	glVertex3f(0.25, -0.25, -0.06);		//e
	glEnd();

	quadC++;    glBegin(GL_QUADS);			//w2.f2.l
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.25, 0, 0.06);			//f
	glVertex3f(0.625, -0.25, 0.06);		//g
	glVertex3f(0.625, -0.5, 0.06);		//h
	glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	glVertex3f(0.25, -0.25, 0.06);		//e
	glEnd();





	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	quadC++;    glBegin(GL_QUADS);			//w2.f3.r
	if (isTexture == 0)
	{
		glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 1);
	}
	glVertex3f(0.25, -0.25, -0.06);		//e
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 1);
	}
	glVertex3f(0.625, -0.5, -0.06);		//h
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 0);
	}
	glVertex3f(1.125, -1.5, -0.06);		//i
	if (isTexture == 0)
	{
		glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0);
	}
	glVertex3f(0.75, -2, -0.06);		//k
	glEnd();
	glDeleteTextures(1, &textures[0]);



	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	quadC++;    glBegin(GL_QUADS);			//w2.f3.l
	if (isTexture == 0)
	{
		glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 1);
	}
	glVertex3f(0.25, -0.25, 0.06);		//e
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 1);
	}
	glVertex3f(0.625, -0.5, 0.06);		//h
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 0);
	}
	glVertex3f(1.125, -1.5, 0.06);		//i
	if (isTexture == 0)
	{
		glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0);
	}
	glVertex3f(0.75, -2, 0.06);		//k
	glEnd();
	glDeleteTextures(1, &textures[0]);





	triangleC++;    glBegin(GL_TRIANGLES);		//w2.f4.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1.125, -1.5, -0.06);		//i
	glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	glVertex3f(0.75, -2, -0.06);		//k
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(1.625, -2.5, -0.06);		//j
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//w2.f4.l
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(1.125, -1.5, 0.06);		//i
	glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	glVertex3f(0.75, -2, 0.06);			//k
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(1.625, -2.5, 0.06);		//j
	glEnd();

	quadC++;    glBegin(GL_QUADS);			//w2.f5.r

	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, -0.25, -0.06);		//n
	glVertex3f(0.25, -0.25, -0.06);		//e
	glVertex3f(0.5, -1.25, -0.06);		//l
	glVertex3f(0.25, -1.25, -0.06);		//m
	glEnd();

	quadC++;    glBegin(GL_QUADS);			//w2.f5.l
	glVertex3f(0, -0.25, 0.06);			//n
	glVertex3f(0.25, -0.25, 0.06);		//e
	glVertex3f(0.5, -1.25, 0.06);		//l
	glVertex3f(0.25, -1.25, 0.06);		//m
	glEnd();

	glPopMatrix();
}

void wing2LeftSide() {
	glPushMatrix();


	quadC++;    glBegin(GL_QUADS);		//w2.s1
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-0.5, 0, -0.125);		//c.r
	glVertex3f(-0.5, 0, 0.125);			//c.r
	glVertex3f(-0.25, 0.25, 0.125);		//a.l
	glVertex3f(-0.25, 0.25, -0.125);	//a.l
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s2
	glVertex3f(-0.25, 0.25, -0.125);	//a.r
	glVertex3f(-0.25, 0.25, 0.125);		//a.l
	glVertex3f(0.25, 0.25, 0.125);		//b.l
	glVertex3f(0.25, 0.25, -0.125);		//b.r
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//w2.s3
	glVertex3f(0.25, 0.25, -0.125);		//b.r
	glVertex3f(0.25, 0.25, 0.125);		//b.l
	glVertex3f(0.25, -0.25, 0.125);		//e.l
	glVertex3f(0.25, -0.25, -0.125);	//e.r
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//w2.s4
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.25, 0, -0.06);			//f.r
	glVertex3f(0.25, 0, 0.06);			//f.l
	glVertex3f(0.625, -0.25, 0.06);		//g.l
	glVertex3f(0.625, -0.25, -0.06);	//g.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s5
	glVertex3f(0.625, -0.25, -0.06);	//g.r
	glVertex3f(0.625, -0.25, 0.06);		//g.l
	glVertex3f(0.625, -0.5, 0.06);		//h.l
	glVertex3f(0.625, -0.5, -0.06);		//h.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s6
	glVertex3f(0.625, -0.5, -0.06);		//h.r
	glVertex3f(0.625, -0.5, 0.06);		//h.l
	glVertex3f(1.625, -2.5, 0.06);		//j.l
	glVertex3f(1.625, -2.5, -0.06);		//j.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s7
	glVertex3f(0.75, -2, -0.06);		//k.r
	glVertex3f(0.75, -2, 0.06);			//k.l
	glVertex3f(1.625, -2.5, 0.06);		//j.l
	glVertex3f(1.625, -2.5, -0.06);		//j.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s8
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0.25, -0.25, -0.06);		//e.r
	glVertex3f(0.25, -0.25, 0.06);		//e.l
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(0.75, -2, 0.06);			//k.l
	glVertex3f(0.75, -2, -0.06);		//k.r
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//w2.s9
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0.5, -1.25, -0.06);		//l.r
	glVertex3f(0.5, -1.25, 0.06);		//l.l
	glVertex3f(0.25, -1.25, 0.06);		//m.l
	glVertex3f(0.25, -1.25, -0.06);		//m.r
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//w2.s10
	glVertex3f(0, -0.25, -0.06);			//n.r
	glVertex3f(0, -0.25, 0.06);			//n.l
	glVertex3f(0.25, -1.25, 0.06);		//m.l
	glVertex3f(0.25, -1.25, -0.06);		//m.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s11
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-0.25, -0.25, -0.125);	//d.r
	glVertex3f(-0.25, -0.25, 0.125);	//d.l
	glVertex3f(0.25, -0.25, 0.125);		//e.l
	glVertex3f(0.25, -0.25, -0.125);	//e.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s12
	glVertex3f(-0.5, 0, -0.125);		//c.r
	glVertex3f(-0.5, 0, 0.125);			//c.l
	glVertex3f(-0.25, -0.25, 0.125);	//d.l
	glVertex3f(-0.25, -0.25, -0.125);	//d.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s13
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0.25, -0.25, -0.06);		//e.r
	glVertex3f(0.25, -0.25, 0.06);		//e.l
	glVertex3f(0.5, -1.25, 0.06);		//l.l
	glVertex3f(0.5, -1.25, -0.06);		//l.r
	glEnd();

	glPopMatrix();
}

void wing2Right() {
	glPushMatrix();
	//glRotatef(180, 0, 1, 0);

	pc++;  	glBegin(GL_POLYGON);		//w2.f1.r
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0.5, 0, -0.125);		//c
	glVertex3f(0.25, 0.25, -0.125);	//a
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-0.25, 0.25, -0.125);		//b
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-0.25, -0.25, -0.125);	//e
	glVertex3f(0.25, -0.25, -0.125);	//d
	glEnd();

	pc++;  	glBegin(GL_POLYGON);		//w2.f1.l
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0.5, 0, 0.125);			//c
	glVertex3f(0.25, 0.25, 0.125);		//a
	glColor3f(0.5, 0.5, 0.5); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
	glVertex3f(-0.25, 0.25, 0.125);		//b
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-0.25, -0.25, 0.125);		//e
	glVertex3f(0.25, -0.25, 0.125);	//d
	glEnd();

	quadC++;    glBegin(GL_QUADS);			//w2.f2.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.25, 0, -0.06);			//f
	glVertex3f(-0.625, -0.25, -0.06);	//g
	glVertex3f(-0.625, -0.5, -0.06);		//h
	glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	glVertex3f(-0.25, -0.25, -0.06);		//e
	glEnd();

	quadC++;    glBegin(GL_QUADS);			//w2.f2.l
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.25, 0, 0.06);			//f
	glVertex3f(-0.625, -0.25, 0.06);		//g
	glVertex3f(-0.625, -0.5, 0.06);		//h
	glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	glVertex3f(-0.25, -0.25, 0.06);		//e
	glEnd();






	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	quadC++;    glBegin(GL_QUADS);			//w2.f3.r
	if (isTexture == 0)
	{
		glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 1);
	}
	glVertex3f(-0.25, -0.25, -0.06);		//e
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 1);
	}
	glVertex3f(-0.625, -0.5, -0.06);		//h
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 0);
	}
	glVertex3f(-1.125, -1.5, -0.06);		//i
	if (isTexture == 0)
	{
		glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0);
	}
	glVertex3f(-0.75, -2, -0.06);		//k
	glEnd();
	glDeleteTextures(1, &textures[0]);



	if (isTexture == 1)
		textures[0] = loadTexture("black1.bmp");
	else if (isTexture == 2)
		textures[0] = loadTexture("black2.bmp");
	quadC++;    glBegin(GL_QUADS);			//w2.f3.l
	if (isTexture == 0)
	{
		glColor3f(0.6, 0.6, 0.6); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey6);
	}
	else {
		glColor3f(1, 1, 1);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 1);
	}
	glVertex3f(-0.25, -0.25, 0.06);		//e
	if (isTexture == 0)
	{
		glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 1);
	}
	glVertex3f(-0.625, -0.5, 0.06);		//h
	if (isTexture != 0) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(1, 0);
	}
	glVertex3f(-1.125, -1.5, 0.06);		//i
	if (isTexture == 0)
	{
		glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glTexCoord2f(0, 0);
	}
	glVertex3f(-0.75, -2, 0.06);		//k
	glEnd();
	glDeleteTextures(1, &textures[0]);






	triangleC++;    glBegin(GL_TRIANGLES);		//w2.f4.r
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1.125, -1.5, -0.06);		//i
	glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	glVertex3f(-0.75, -2, -0.06);		//k
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-1.625, -2.5, -0.06);		//j
	glEnd();

	triangleC++;    glBegin(GL_TRIANGLES);		//w2.f4.l
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-1.125, -1.5, 0.06);		//i
	glColor3f(0.7, 0.7, 0.7); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey7);
	glVertex3f(-0.75, -2, 0.06);			//k
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-1.625, -2.5, 0.06);		//j
	glEnd();

	quadC++;    glBegin(GL_QUADS);			//w2.f5.r

	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(0, -0.25, -0.06);		//n
	glVertex3f(-0.25, -0.25, -0.06);		//e
	glVertex3f(-0.5, -1.25, -0.06);		//l
	glVertex3f(-0.25, -1.25, -0.06);		//m
	glEnd();

	quadC++;    glBegin(GL_QUADS);			//w2.f5.l
	glVertex3f(0, -0.25, 0.06);			//n
	glVertex3f(-0.25, -0.25, 0.06);		//e
	glVertex3f(-0.5, -1.25, 0.06);		//l
	glVertex3f(-0.25, -1.25, 0.06);		//m
	glEnd();

	glPopMatrix();
}

void wing2RightSide() {

	glPushMatrix();
	//glRotatef(180, 0, 1, 0);


	quadC++;    glBegin(GL_QUADS);		//w2.s1
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(0.5, 0, -0.125);		//c.r
	glVertex3f(0.5, 0, 0.125);			//c.r
	glVertex3f(0.25, 0.25, 0.125);		//a.l
	glVertex3f(0.25, 0.25, -0.125);	//a.l
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s2
	glVertex3f(0.25, 0.25, -0.125);	//a.r
	glVertex3f(0.25, 0.25, 0.125);		//a.l
	glVertex3f(-0.25, 0.25, 0.125);		//b.l
	glVertex3f(-0.25, 0.25, -0.125);		//b.r
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//w2.s3
	glVertex3f(-0.25, 0.25, -0.125);		//b.r
	glVertex3f(-0.25, 0.25, 0.125);		//b.l
	glVertex3f(-0.25, -0.25, 0.125);		//e.l
	glVertex3f(-0.25, -0.25, -0.125);	//e.r
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//w2.s4
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.25, 0, -0.06);			//f.r
	glVertex3f(-0.25, 0, 0.06);			//f.l
	glVertex3f(-0.625, -0.25, 0.06);		//g.l
	glVertex3f(-0.625, -0.25, -0.06);	//g.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s5
	glVertex3f(-0.625, -0.25, -0.06);	//g.r
	glVertex3f(-0.625, -0.25, 0.06);		//g.l
	glVertex3f(-0.625, -0.5, 0.06);		//h.l
	glVertex3f(-0.625, -0.5, -0.06);		//h.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s6
	glVertex3f(-0.625, -0.5, -0.06);		//h.r
	glVertex3f(-0.625, -0.5, 0.06);		//h.l
	glVertex3f(-1.625, -2.5, 0.06);		//j.l
	glVertex3f(-1.625, -2.5, -0.06);		//j.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s7
	glVertex3f(-0.75, -2, -0.06);		//k.r
	glVertex3f(-0.75, -2, 0.06);			//k.l
	glVertex3f(-1.625, -2.5, 0.06);		//j.l
	glVertex3f(-1.625, -2.5, -0.06);		//j.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s8
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0.25, -0.25, -0.06);		//e.r
	glVertex3f(-0.25, -0.25, 0.06);		//e.l
	glColor3f(0.4, 0.4, 0.4); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
	glVertex3f(-0.75, -2, 0.06);			//k.l
	glVertex3f(-0.75, -2, -0.06);		//k.r
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//w2.s9
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0.5, -1.25, -0.06);		//l.r
	glVertex3f(-0.5, -1.25, 0.06);		//l.l
	glVertex3f(-0.25, -1.25, 0.06);		//m.l
	glVertex3f(-0.25, -1.25, -0.06);		//m.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s10
	glVertex3f(0, -0.25, -0.06);			//n.r
	glVertex3f(0, -0.25, 0.06);			//n.l
	glVertex3f(-0.25, -1.25, 0.06);		//m.l
	glVertex3f(-0.25, -1.25, -0.06);		//m.r
	glEnd();


	quadC++;    glBegin(GL_QUADS);		//w2.s11
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(0.25, -0.25, -0.125);	//d.r
	glVertex3f(0.25, -0.25, 0.125);	//d.l
	glVertex3f(-0.25, -0.25, 0.125);		//e.l
	glVertex3f(-0.25, -0.25, -0.125);	//e.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s12
	glVertex3f(0.5, 0, -0.125);		//c.r
	glVertex3f(0.5, 0, 0.125);			//c.l
	glVertex3f(0.25, -0.25, 0.125);	//d.l
	glVertex3f(0.25, -0.25, -0.125);	//d.r
	glEnd();

	quadC++;    glBegin(GL_QUADS);		//w2.s13
	glColor3f(1, 1, 1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glVertex3f(-0.25, -0.25, -0.06);		//e.r
	glVertex3f(-0.25, -0.25, 0.06);		//e.l
	glVertex3f(-0.5, -1.25, 0.06);		//l.l
	glVertex3f(-0.5, -1.25, -0.06);		//l.r
	glEnd();

	glPopMatrix();
}

void wingleftStick() {
	glPushMatrix();
	//frontView----------------------------------------------------------------------------


	pc++;  	glBegin(GL_POLYGON);			//wls.f1.b
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(-0.5, 0, -0.12);			//a
	glVertex3f(-0.25, 0, -0.12);		//b
	glVertex3f(-0.25, -0.25, -0.12);	//c
	glVertex3f(-1.075, -0.825, -0.12);	//d
	glVertex3f(-1.325, -0.825, -0.12);	//e
	glVertex3f(-1.325, -0.575, -0.12);	//f
	glEnd();

	pc++;  	glBegin(GL_POLYGON);			//wls.f1.f
	glVertex3f(-0.5, 0, 0.12);			//a
	glVertex3f(-0.25, 0, 0.12);			//b
	glVertex3f(-0.25, -0.25, 0.12);		//c
	glVertex3f(-1.075, -0.825, 0.12);	//d
	glVertex3f(-1.325, -0.825, 0.12);	//e
	glVertex3f(-1.325, -0.575, 0.12);	//f
	glEnd();

	//sideView-----------------------------------------------------------------------


	quadC++;    glBegin(GL_QUADS);				//wls.s1
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(-0.5, 0, -0.12);			//a
	glVertex3f(-0.5, 0, 0.12);			//a
	glVertex3f(-0.25, 0, 0.12);			//b
	glVertex3f(-0.25, 0, -0.12);		//b
	glEnd();

	quadC++;    glBegin(GL_QUADS);				//wls.s2
	glVertex3f(-0.25, 0, -0.12);		//b
	glVertex3f(-0.25, 0, 0.12);		//b
	glVertex3f(-0.25, -0.25, 0.12);	//c
	glVertex3f(-0.25, -0.25, -0.12);	//c
	glEnd();

	quadC++;    glBegin(GL_QUADS);				//wls.s3
	glVertex3f(-0.25, -0.25, -0.12);	//c
	glVertex3f(-0.25, -0.25, 0.12);	//c
	glVertex3f(-1.075, -0.825, 0.12);	//d
	glVertex3f(-1.075, -0.825, -0.12);	//d
	glEnd();

	quadC++;    glBegin(GL_QUADS);				//wls.s4
	glVertex3f(-1.075, -0.825, -0.12);	//d
	glVertex3f(-1.075, -0.825, 0.12);	//d
	glVertex3f(-1.325, -0.825, 0.12);	//e
	glVertex3f(-1.325, -0.825, -0.12);	//e
	glEnd();

	quadC++;    glBegin(GL_QUADS);				//wls.s5
	glVertex3f(-1.325, -0.825, -0.12);	//e
	glVertex3f(-1.325, -0.825, 0.12);	//e
	glVertex3f(-1.325, -0.575, 0.12);	//f
	glVertex3f(-1.325, -0.575, -0.12);	//f
	glEnd();

	quadC++;    glBegin(GL_QUADS);				//wls.s6
	glVertex3f(-0.5, 0, -0.12);			//a
	glVertex3f(-0.5, 0, 0.12);			//a
	glVertex3f(-1.325, -0.575, 0.12);	//f
	glVertex3f(-1.325, -0.575, -0.12);	//f
	glEnd();

	glPopMatrix();
}

void wingRightStick() {
	glPushMatrix();
	//glRotatef(180, 0, 1, 0);

	//frontView----------------------------------------------------------------------------


	pc++;  	glBegin(GL_POLYGON);			//wls.f1.b
	glColor3f(0.2, 0.2, 0.2); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
	glVertex3f(0.5, 0, -0.12);			//a
	glVertex3f(0.25, 0, -0.12);		//b
	glVertex3f(0.25, -0.25, -0.12);	//c
	glVertex3f(1.075, -0.825, -0.12);	//d
	glVertex3f(1.325, -0.825, -0.12);	//e
	glVertex3f(1.325, -0.575, -0.12);	//f
	glEnd();

	pc++;  	glBegin(GL_POLYGON);			//wls.f1.f
	glVertex3f(0.5, 0, 0.12);			//a
	glVertex3f(0.25, 0, 0.12);			//b
	glVertex3f(0.25, -0.25, 0.12);		//c
	glVertex3f(1.075, -0.825, 0.12);	//d
	glVertex3f(1.325, -0.825, 0.12);	//e
	glVertex3f(1.325, -0.575, 0.12);	//f
	glEnd();

	//sideView-----------------------------------------------------------------------


	quadC++;    glBegin(GL_QUADS);				//wls.s1
	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(0.5, 0, -0.12);			//a
	glVertex3f(0.5, 0, 0.12);			//a
	glVertex3f(0.25, 0, 0.12);			//b
	glVertex3f(0.25, 0, -0.12);		//b
	glEnd();

	quadC++;    glBegin(GL_QUADS);				//wls.s2
	glVertex3f(0.25, 0, -0.12);		//b
	glVertex3f(0.25, 0, 0.12);		//b
	glVertex3f(0.25, -0.25, 0.12);	//c
	glVertex3f(0.25, -0.25, -0.12);	//c
	glEnd();

	quadC++;    glBegin(GL_QUADS);				//wls.s3
	glVertex3f(0.25, -0.25, -0.12);	//c
	glVertex3f(0.25, -0.25, 0.12);	//c
	glVertex3f(1.075, -0.825, 0.12);	//d
	glVertex3f(1.075, -0.825, -0.12);	//d
	glEnd();

	quadC++;    glBegin(GL_QUADS);				//wls.s4
	glVertex3f(1.075, -0.825, -0.12);	//d
	glVertex3f(1.075, -0.825, 0.12);	//d
	glVertex3f(1.325, -0.825, 0.12);	//e
	glVertex3f(1.325, -0.825, -0.12);	//e
	glEnd();

	quadC++;    glBegin(GL_QUADS);				//wls.s5
	glVertex3f(1.325, -0.825, -0.12);	//e
	glVertex3f(1.325, -0.825, 0.12);	//e
	glVertex3f(1.325, -0.575, 0.12);	//f
	glVertex3f(1.325, -0.575, -0.12);	//f
	glEnd();

	quadC++;    glBegin(GL_QUADS);				//wls.s6
	glVertex3f(0.5, 0, -0.12);			//a
	glVertex3f(0.5, 0, 0.12);			//a
	glVertex3f(1.325, -0.575, 0.12);	//f
	glVertex3f(1.325, -0.575, -0.12);	//f
	glEnd();

	glPopMatrix();
}

void balls() {
	glPushMatrix();


	glColor3f(0.1, 0.1, 0.1); glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);

	glPushMatrix();
	narutoRotate += 1;
	glTranslatef(0, 3, 0);
	glRotatef(narutoRotate, 0, 0, 1);

	glPushMatrix();
	glTranslatef(2, 3.6, -5);
	drawSphere(0.75, 15, 15, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4, 0, -5);
	glRotatef(-90, 1, 0, 0);
	drawSphere(0.75, 15, 15, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, -3.6, -5);
	drawSphere(0.75, 15, 15, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, -3.6, -5);
	drawSphere(0.75, 15, 15, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4, 0, -5);
	drawSphere(0.75, 15, 15, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 3.6, -5);
	drawSphere(0.75, 15, 15, 2);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void wing() {

	if (wingRightRotate < 0)
		balls();

	glPushMatrix();
	glTranslatef(0.7, 4.4, -1.2);
	glScalef(2, 2, 2);

	if (wingOn) {
		if (wingLeftRotateY > 0)
			wingLeftRotateY -= wingRotateSpeedY;
	}
	else {
		if (wingLeftRotateY < 45)
			wingLeftRotateY += wingRotateSpeedY;
	}
	glRotatef(wingLeftRotateY, 0, 1, 0);

	//left wing-----------------------------------------------------------
	glPushMatrix();
	glTranslatef(1.25, 0.25, -0.375);

	if (wingOn) {
		if (wingLeftRotate < 0)
			wingLeftRotate += wingRotateSpeed;
	}
	else {
		if (wingLeftRotate > -50)
			wingLeftRotate -= wingRotateSpeed;
	}

	glRotatef(wingLeftRotate, 0, 0, 1);

	wing1Left();
	wing1LeftSide();
	glPopMatrix();

	// left wing2-------------------------------------------------------
	glPushMatrix();
	glTranslatef(1, 0, 0);
	wingleftStick();

	if (wingOn) {
		if (wing2LeftRotate < 0)
			wing2LeftRotate += wingRotateSpeed;
	}
	else {
		if (wing2LeftRotate > -20)
			wing2LeftRotate -= wingRotateSpeed;
	}

	glRotatef(wing2LeftRotate, 0, 0, 1);

	wing2Left();
	wing2LeftSide();

	glPopMatrix();
	glPopMatrix();


	//-----------------------------------------------------------------------------
	glPushMatrix();
	glTranslatef(-0.7, 4.4, -1.2);
	glScalef(2, 2, 2);

	if (wingOn) {
		if (wingRightRotateY < 0)
			wingRightRotateY += wingRotateSpeedY;
	}
	else {
		if (wingRightRotateY > -45)
			wingRightRotateY -= wingRotateSpeedY;
	}
	glRotatef(wingRightRotateY, 0, 1, 0);


	// right wing------------------------------------------------------
	glPushMatrix();
	glTranslatef(-1.25, 0.25, -0.375);

	if (wingOn) {
		if (wingRightRotate > 0)
			wingRightRotate -= wingRotateSpeed;
	}
	else {
		if (wingRightRotate < 50)
			wingRightRotate += wingRotateSpeed;
	}

	glRotatef(wingRightRotate, 0, 0, 1);

	wing1Right();
	wing1RightSide();
	glPopMatrix();

	//right wing2-------------------------------------------------------------------------
	glPushMatrix();
	glTranslatef(-1, 0, 0);
	wingRightStick();

	if (wingOn) {
		if (wing2RightRotate > 0)
			wing2RightRotate -= wingRotateSpeed;
	}
	else {
		if (wing2RightRotate < 20)
			wing2RightRotate += wingRotateSpeed;
	}

	glRotatef(wing2RightRotate, 0, 0, 1);

	wing2Right();
	wing2RightSide();
	glPopMatrix();
	glPopMatrix();

}
// wing end---------------------------------------------------------------------------------



//lower body--------------------------------------------------------------------------------

void drawCylinder2(float top, float bot, float h, int sl, int st, int t)
{
	pc++;
	cylinderC++;
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	if (t == 1)
		gluQuadricDrawStyle(cylinder, GLU_LINE);
	else
		gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, true);
	gluCylinder(cylinder, top, bot, h, sl, st);
	gluDeleteQuadric(cylinder);					// prevent memory burst
}

void drawFoot(int i)
{
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	// Front

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.1);
	glVertex3f(0.4, 0.2, 0.0);
	glTexCoord2f(0.1, 0.0);
	glVertex3f(0.4, 0.0, -0.2);
	glTexCoord2f(0.45, 0.0);
	glVertex3f(0.3, 0.0, -1.0);
	glTexCoord2f(0.0, 0.4);
	glVertex3f(0.3, 1.0, 0.0);
	glEnd();

	// Back
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.1);
	glVertex3f(-0.4, 0.2, 0.0);
	glTexCoord2f(0.1, 0.0);
	glVertex3f(-0.4, 0.0, -0.2);
	glTexCoord2f(0.45, 0.0);
	glVertex3f(-0.3, 0.0, -1.0);
	glTexCoord2f(0.0, 0.4);
	glVertex3f(-0.3, 1.0, 0.0);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.4, 0.2, 0.0);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(0.4, 0.0, -0.2);
	glTexCoord2f(0.5, 0.25);
	glVertex3f(-0.4, 0.0, -0.2);
	glTexCoord2f(0.0, 0.25);
	glVertex3f(-0.4, 0.2, 0.0);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.4, 0.0, -0.2);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(0.3, 0.0, -1.0);
	glTexCoord2f(0.5, 0.25);
	glVertex3f(-0.3, 0.0, -1.0);
	glTexCoord2f(0.0, 0.25);
	glVertex3f(-0.4, 0.0, -0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.3, 0.0, -1.0);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(0.3, 1.0, 0.0);
	glTexCoord2f(0.5, 0.25);
	glVertex3f(-0.3, 1.0, 0.0);
	glTexCoord2f(0.0, 0.25);
	glVertex3f(-0.3, 0.0, -1.0);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.4, 0.2, 0.0);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(0.3, 1.0, 0.0);
	glTexCoord2f(0.5, 0.25);
	glVertex3f(-0.3, 1.0, 0.0);
	glTexCoord2f(0.0, 0.25);
	glVertex3f(-0.4, 0.2, 0.0);
	glEnd();

	glPopMatrix();
}

void footArmamanet(int i) {
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); //o
	glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(0.5, -0.5, -0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); // o
	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); //i
	glVertex3f(0.4, 0.4, -0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(0.4, 0.4, 0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(0.4, -0.5, 0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(0.4, -0.5, -0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); //i
	glVertex3f(-0.4, 0.4, -0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(-0.4, 0.4, 0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(-0.4, -0.5, 0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(-0.4, -0.5, -0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); //o top
	glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(0.3, 0.0);
	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(0.3, 0.4);
	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(0.0, 0.4);
	glVertex3f(-0.5, 0.5, -0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); //i top
	glVertex3f(0.4, 0.4, -0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(0.4, 0.4, 0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(-0.4, 0.4, 0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(-0.4, 0.4, -0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); //right bot
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(0.4, -0.5, -0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(0.4, -0.5, 0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);  //left bot
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(-0.4, -0.5, -0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(-0.4, -0.5, 0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); // right front
	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(0.4, -0.5, 0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(0.4, 0.4, 0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);  //left front
	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(-0.4, -0.5, 0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(-0.4, 0.4, 0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); //front top
	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(-0.4, 0.4, 0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(0.4, 0.4, 0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); //right rear
	glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(0.4, -0.5, -0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(0.4, 0.4, -0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); //left rear
	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(-0.4, -0.5, -0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(-0.4, 0.4, -0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0); //rear top
	glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(0.4, 0.0);
	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(0.4, 0.3);
	glVertex3f(-0.4, 0.4, -0.5);
	glTexCoord2f(0.0, 0.3);
	glVertex3f(0.4, 0.4, -0.5);
	glEnd();
}

void rightFrontMiniSkirt(int i) {
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.45);
	glVertex3f(0.0, 0.0, -0.2);
	glTexCoord2f(0.2, 0.45);
	glVertex3f(1.2, 0.0, -0.2);
	glTexCoord2f(0.2, 0.2);
	glVertex3f(1.2, -1, -0.2);
	glTexCoord2f(0.1, 0.05);
	glVertex3f(0.7, -1.7, -0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -2, -0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.45);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(0.2, 0.45);
	glVertex3f(1.2, 0.0, 0.2);
	glTexCoord2f(0.2, 0.2);
	glVertex3f(1.2, -1, 0.2);
	glTexCoord2f(0.1, 0.05);
	glVertex3f(0.7, -1.7, 0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -2, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.2, 0.0, 0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.2, 0.0, -0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.2, 0.0, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.2, -1, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.2, -1, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.2, 0.0, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.2, -1, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.7, -1.7, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.7, -1.7, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.2, -1, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.7, -1.7, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, -2, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, -2, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.7, -1.7, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, -2, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, -2, -0.2);
	glEnd();
}

void leftFrontMiniSkirt(int i) {

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.45);
	glVertex3f(0.0, 0.0, -0.2);
	glTexCoord2f(0.2, 0.45);
	glVertex3f(-1.2, 0.0, -0.2);
	glTexCoord2f(0.2, 0.2);
	glVertex3f(-1.2, -1, -0.2);
	glTexCoord2f(0.1, 0.05);
	glVertex3f(-0.7, -1.7, -0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.0, -2, -0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.45);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(0.2, 0.45);
	glVertex3f(-1.2, 0.0, 0.2);
	glTexCoord2f(0.2, 0.2);
	glVertex3f(-1.2, -1, 0.2);
	glTexCoord2f(0.1, 0.05);
	glVertex3f(-0.7, -1.7, 0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -2, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.2, 0.0, 0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.2, 0.0, -0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.2, 0.0, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.2, -1, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.2, -1, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.2, 0.0, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.2, -1, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.7, -1.7, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.7, -1.7, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.2, -1, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.7, -1.7, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.0, -2, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.0, -2, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.7, -1.7, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, -2, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, -2, -0.2);
	glEnd();

}

void rightFrontSkirt(int i) {
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.2);
	glTexCoord2f(0.45, 1.0);
	glVertex3f(1, 0.0, -0.2);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(1.2, -1, -0.2);
	glTexCoord2f(0.3, 0.2);
	glVertex3f(0.7, -1.7, -0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -2, -0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(0.9, 1.0);
	glVertex3f(1, 0.0, 0.2);
	glTexCoord2f(1.0, 0.5);
	glVertex3f(1.2, -1, 0.2);
	glTexCoord2f(0.6, 0.2);
	glVertex3f(0.7, -1.7, 0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -2, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1, 0.0, 0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1, 0.0, -0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1, 0.0, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.2, -1, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.2, -1, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1, 0.0, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.2, -1, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.7, -1.7, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.7, -1.7, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.2, -1, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.7, -1.7, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, -2, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, -2, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.7, -1.7, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, -2, 0.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, -2, -0.2);
	glEnd();
}

void leftFrontSkirt(int i) {
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.2);
	glTexCoord2f(0.45, 1.0);
	glVertex3f(-1, 0.0, -0.2);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(-1.2, -1, -0.2);
	glTexCoord2f(0.3, 0.2);
	glVertex3f(-0.7, -1.7, -0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -2, -0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(0.9, 1.0);
	glVertex3f(-1, 0.0, 0.2);
	glTexCoord2f(1.0, 0.5);
	glVertex3f(-1.2, -1, 0.2);
	glTexCoord2f(0.6, 0.2);
	glVertex3f(-0.7, -1.7, 0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -2, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1, 0.0, 0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1, 0.0, -0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1, 0.0, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.2, -1, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.2, -1, 0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1, 0.0, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.2, -1, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.7, -1.7, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.7, -1.7, 0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.2, -1, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.7, -1.7, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, -2, -0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, -2, 0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.7, -1.7, 0.2);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, 0.0, 0.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, -2, 0.2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -2, -0.2);
	glEnd();

	glDeleteTextures(1, &textures[1]);
}

void rearSkirt(int i) {
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_STRIP);
	}
	glTexCoord2f(0.35, 0.55);
	glVertex3f(-0.4, -0.9, -0.1);
	glTexCoord2f(0.0, 0.65);
	glVertex3f(-1.2, -0.7, -0.1);
	glTexCoord2f(0.1, 1);
	glVertex3f(-1, 0, -0.1);
	glTexCoord2f(0.9, 1);
	glVertex3f(1, 0, -0.1);
	glTexCoord2f(1.0, 0.65);
	glVertex3f(1.2, -0.7, -0.1);
	glTexCoord2f(0.65, 0.55);
	glVertex3f(0.4, -0.9, -0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_STRIP);
	}
	glTexCoord2f(0.67, 0.55);
	glVertex3f(0.4, -0.9, -0.1);
	glTexCoord2f(0.595, 0.0);
	glVertex3f(0.2, -2, -0.1);
	glTexCoord2f(0.445, 0.0);
	glVertex3f(-0.2, -2, -0.1);
	glTexCoord2f(0.35, 0.55);
	glVertex3f(-0.4, -0.9, -0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_STRIP);
	}
	glTexCoord2f(0.35, 0.55);
	glVertex3f(-0.4, -0.9, 0.1);
	glTexCoord2f(0.0, 0.65);
	glVertex3f(-1.2, -0.7, 0.1);
	glTexCoord2f(0.1, 1);
	glVertex3f(-1, 0, 0.1);
	glTexCoord2f(0.9, 1);
	glVertex3f(1, 0, 0.1);
	glTexCoord2f(1.0, 0.65);
	glVertex3f(1.2, -0.7, 0.1);
	glTexCoord2f(0.65, 0.55);
	glVertex3f(0.4, -0.9, 0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_STRIP);
	}
	glTexCoord2f(0.67, 0.55);
	glVertex3f(0.4, -0.9, 0.1);
	glTexCoord2f(0.595, 0.0);
	glVertex3f(0.2, -2, 0.1);
	glTexCoord2f(0.445, 0.0);
	glVertex3f(-0.2, -2, 0.1);
	glTexCoord2f(0.35, 0.55);
	glVertex3f(-0.4, -0.9, 0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1, 0, -0.1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1, 0, -0.1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1, 0, 0.1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1, 0, 0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1, 0, 0.1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.2, -0.7, 0.1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.2, -0.7, -0.1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1, 0, -0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.2, -0.7, 0.1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.4, -0.9, 0.1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.4, -0.9, -0.1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.2, -0.7, -0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.4, -0.9, 0.1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.2, -2, 0.1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.2, -2, -0.1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.4, -0.9, -0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.2, -2, 0.1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.2, -2, 0.1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.2, -2, -0.1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.2, -2, -0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.2, -2, 0.1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.4, -0.9, 0.1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.4, -0.9, -0.1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.2, -2, -0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.4, -0.9, 0.1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.2, -0.7, 0.1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.2, -0.7, -0.1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.4, -0.9, -0.1);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1, 0, 0.1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.2, -0.7, 0.1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.2, -0.7, -0.1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1, 0, -0.1);
	glEnd();
}

void leftSideSkirt(int i) {
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.075);
	glTexCoord2f(0.42, 1.0);
	glVertex3f(1, 0.0, -0.075);
	glTexCoord2f(0.47, 0.67);
	glVertex3f(1.1, -1, -0.075);
	glTexCoord2f(0.3, 0.6);
	glVertex3f(0.7, -1.2, -0.075);
	glTexCoord2f(0.18, 0.1);
	glVertex3f(0.4, -2.7, -0.075);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -3, -0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.075);
	glTexCoord2f(0.42, 1.0);
	glVertex3f(1, 0.0, 0.075);
	glTexCoord2f(0.47, 0.67);
	glVertex3f(1.1, -1, 0.075);
	glTexCoord2f(0.3, 0.6);
	glVertex3f(0.7, -1.2, 0.075);
	glTexCoord2f(0.18, 0.1);
	glVertex3f(0.4, -2.7, 0.075);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -3, 0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1, 0.0, 0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1, 0.0, -0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1, 0.0, -0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.1, -1, -0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.1, -1, 0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1, 0.0, 0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.1, -1, -0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.7, -1.2, -0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.7, -1.2, 0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.1, -1, 0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.7, -1.2, -0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.4, -2.7, -0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.4, -2.7, 0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.7, -1.2, 0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.4, -2.7, -0.075);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -3, -0.075);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -3, 0.075);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.4, -2.7, 0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, -3, 0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, -3, -0.075);
	glEnd();
}

void rightSideSkirt(int i) {
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.075);
	glTexCoord2f(0.42, 1.0);
	glVertex3f(-1, 0.0, -0.075);
	glTexCoord2f(0.47, 0.67);
	glVertex3f(-1.1, -1, -0.075);
	glTexCoord2f(0.3, 0.6);
	glVertex3f(-0.7, -1.2, -0.075);
	glTexCoord2f(0.18, 0.1);
	glVertex3f(-0.4, -2.7, -0.075);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -3, -0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.075);
	glTexCoord2f(0.42, 1.0);
	glVertex3f(-1, 0.0, 0.075);
	glTexCoord2f(0.47, 0.67);
	glVertex3f(-1.1, -1, 0.075);
	glTexCoord2f(0.3, 0.6);
	glVertex3f(-0.7, -1.2, 0.075);
	glTexCoord2f(0.18, 0.1);
	glVertex3f(-0.4, -2.7, 0.075);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -3, 0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1, 0.0, 0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1, 0.0, -0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, -0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1, 0.0, -0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.1, -1, -0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.1, -1, 0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1, 0.0, 0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.1, -1, -0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.7, -1.2, -0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.7, -1.2, 0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.1, -1, 0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.7, -1.2, -0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.4, -2.7, -0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.4, -2.7, 0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.7, -1.2, 0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.4, -2.7, -0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, -3, -0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, -3, 0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.4, -2.7, 0.075);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.075);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.075);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, -3, 0.075);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, -3, -0.075);
	glEnd();

	glPopMatrix();
}

void thirdLeg(int i) {
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.5, 1.0);
	glVertex3f(-0.25, 0, -0.4);
	glTexCoord2f(0.9, 1.0);
	glVertex3f(-0.25, 0, 0);
	glTexCoord2f(0.9, 0.0);
	glVertex3f(-0.25, -1, 0);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(-0.25, -1, -0.4);
	glTexCoord2f(0.3, 0.2);
	glVertex3f(-0.25, -0.8, -0.7);
	glTexCoord2f(0.3, 0.8);
	glVertex3f(-0.25, -0.2, -0.7);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.5, 1.0);
	glVertex3f(0.25, 0, -0.4);
	glTexCoord2f(0.9, 1.0);
	glVertex3f(0.25, 0, 0);
	glTexCoord2f(0.9, 0.0);
	glVertex3f(0.25, -1, 0);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(0.25, -1, -0.4);
	glTexCoord2f(0.3, 0.2);
	glVertex3f(0.25, -0.8, -0.7);
	glTexCoord2f(0.3, 0.8);
	glVertex3f(0.25, -0.2, -0.7);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.25, 0, -0.4);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.25, 0, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.25, 0, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.25, 0, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.25, 0, 0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.25, -1, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.25, -1, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.25, 0, 0);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.25, -1, 0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.25, -1, -0.4);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.25, -1, -0.4);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.25, -1, 0);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.25, -1, -0.4);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.25, -0.8, -0.7);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.25, -0.8, -0.7);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.25, -1, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.25, -0.8, -0.7);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.25, -0.2, -0.7);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.25, -0.2, -0.7);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.25, -0.8, -0.7);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.25, 0, -0.4);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.25, -0.2, -0.7);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.25, -0.2, -0.7);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.25, 0, -0.4);
	glEnd();

	if (i == 1)
		glColor3f(1, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	else
		glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0.001, -0.32, -0.44);
	glScalef(0.4, 0.58, 0.43);
	glTranslatef(0, 0, 0);
	drawCube(i);
	glPopMatrix();
}

void connector(int t) {
	glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glPushMatrix();
	drawFullCylinder(0.2, 0.6, t);
	glPopMatrix();

	glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	glPushMatrix();
	glTranslatef(0, 0, 0.3);
	drawFullCylinder(0.18, 2.0, t);
	glPopMatrix();

	glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glPushMatrix();
	glTranslatef(0, 0, 2);
	glTranslatef(0, 0, 0);
	drawFullCylinder(0.2, 0.6, t);
	glPopMatrix();
}

void canon(int i, int t) {

	glPushMatrix();
	glTranslatef(-1.6, 0, -0.4);
	glRotatef(180, 1, 0, 0);
	glScalef(1.8, 1, 1);
	//front side
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_STRIP);
	}
	glVertex3f(-0.8, 0.45, 0.4);
	glVertex3f(-4, 0.35, 0.4);
	glVertex3f(-4, -0.35, 0.4);
	glVertex3f(-0.8, -0.45, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_STRIP);
	}
	glVertex3f(-0.8, 0.45, 0.4);
	glVertex3f(-0.5, 0.65, 0.4);
	glVertex3f(0.5, 0.65, 0.4);
	glVertex3f(0.8, 0.45, 0.4);
	glVertex3f(1.3, 0.32, 0.4);
	glVertex3f(1.3, -0.32, 0.4);
	glVertex3f(0.8, -0.45, 0.4);
	glVertex3f(0.5, -0.65, 0.4);
	glVertex3f(-0.5, -0.65, 0.4);
	glVertex3f(-0.8, -0.45, 0.4);
	glEnd();

	// back side
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_STRIP);
	}
	glShadeModel(GL_FLAT);
	glVertex3f(-0.8, 0.45, -0.4);
	glVertex3f(-4, 0.35, -0.4);
	glVertex3f(-4, -0.35, -0.4);
	glVertex3f(-0.8, -0.45, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_STRIP);
	}
	glVertex3f(-0.8, 0.45, -0.4);
	glVertex3f(-0.5, 0.65, -0.4);
	glVertex3f(0.5, 0.65, -0.4);
	glVertex3f(0.8, 0.45, -0.4);
	glVertex3f(1.3, 0.32, -0.4);
	glVertex3f(1.3, -0.32, -0.4);
	glVertex3f(0.8, -0.45, -0.4);
	glVertex3f(0.5, -0.65, -0.4);
	glVertex3f(-0.5, -0.65, -0.4);
	glVertex3f(-0.8, -0.45, -0.4);
	glEnd();

	//----------------------------------------

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, 0.4, 0.3);
	glVertex3f(-0.8, 0.5, 0.3);
	glVertex3f(-0.8, 0.5, -0.3);
	glVertex3f(-4, 0.4, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.8, 0.5, 0.3);
	glVertex3f(-0.5, 0.7, 0.3);
	glVertex3f(-0.5, 0.7, -0.3);
	glVertex3f(-0.8, 0.5, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.5, 0.7, 0.3);
	glVertex3f(0.5, 0.7, 0.3);
	glVertex3f(0.5, 0.7, -0.3);
	glVertex3f(-0.5, 0.7, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.5, 0.7, 0.3);
	glVertex3f(0.8, 0.5, 0.3);
	glVertex3f(0.8, 0.5, -0.3);
	glVertex3f(0.5, 0.7, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.8, 0.5, 0.3);
	glVertex3f(1.3, 0.37, 0.3);
	glVertex3f(1.3, 0.37, -0.3);
	glVertex3f(0.8, 0.5, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glVertex3f(1.3, 0.37, 0.3);
	glVertex3f(1.3, 0.32, 0.4);
	glVertex3f(1.3, -0.32, 0.4);
	glVertex3f(1.3, -0.37, 0.3);
	glVertex3f(1.3, -0.37, -0.3);
	glVertex3f(1.3, -0.32, -0.4);
	glVertex3f(1.3, 0.32, -0.4);
	glVertex3f(1.3, 0.37, -0.3);
	glEnd();

	if (i == 1)
		glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	else
		glColor3f(0, 0, 0);

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(1.3, -0.37, 0.3);
	glVertex3f(0.8, -0.5, 0.3);
	glVertex3f(0.8, -0.5, -0.3);
	glVertex3f(1.3, -0.37, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.8, -0.5, 0.3);
	glVertex3f(0.5, -0.7, 0.3);
	glVertex3f(0.5, -0.7, -0.3);
	glVertex3f(0.8, -0.5, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.5, -0.7, 0.3);
	glVertex3f(-0.5, -0.7, 0.3);
	glVertex3f(-0.5, -0.7, -0.3);
	glVertex3f(0.5, -0.7, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.5, -0.7, 0.3);
	glVertex3f(-0.8, -0.5, 0.3);
	glVertex3f(-0.8, -0.5, -0.3);
	glVertex3f(-0.5, -0.7, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.8, -0.5, 0.3);
	glVertex3f(-4, -0.4, 0.3);
	glVertex3f(-4, -0.4, -0.3);
	glVertex3f(-0.8, -0.5, -0.3);
	glEnd();

	//----------------------------------------
	//front
	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, 0.4, 0.3);
	glVertex3f(-0.8, 0.5, 0.3);
	glVertex3f(-0.8, 0.45, 0.4);
	glVertex3f(-4, 0.35, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.8, 0.5, 0.3);
	glVertex3f(-0.5, 0.7, 0.3);
	glVertex3f(-0.5, 0.65, 0.4);
	glVertex3f(-0.8, 0.45, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.5, 0.7, 0.3);
	glVertex3f(0.5, 0.7, 0.3);
	glVertex3f(0.5, 0.65, 0.4);
	glVertex3f(-0.5, 0.65, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.5, 0.7, 0.3);
	glVertex3f(0.8, 0.5, 0.3);
	glVertex3f(0.8, 0.45, 0.4);
	glVertex3f(0.5, 0.65, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.8, 0.5, 0.3);
	glVertex3f(1.3, 0.37, 0.3);
	glVertex3f(1.3, 0.32, 0.4);
	glVertex3f(0.8, 0.45, 0.4);
	glEnd();


	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(1.3, -0.37, 0.3);
	glVertex3f(0.8, -0.5, 0.3);
	glVertex3f(0.8, -0.45, 0.4);
	glVertex3f(1.3, -0.32, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.8, -0.5, 0.3);
	glVertex3f(0.5, -0.7, 0.3);
	glVertex3f(0.5, -0.65, 0.4);
	glVertex3f(0.8, -0.45, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.5, -0.7, 0.3);
	glVertex3f(-0.5, -0.7, 0.3);
	glVertex3f(-0.5, -0.65, 0.4);
	glVertex3f(0.5, -0.65, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.5, -0.7, 0.3);
	glVertex3f(-0.8, -0.5, 0.3);
	glVertex3f(-0.8, -0.45, 0.4);
	glVertex3f(-0.5, -0.65, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.8, -0.5, 0.3);
	glVertex3f(-4, -0.4, 0.3);
	glVertex3f(-4, -0.35, 0.4);
	glVertex3f(-0.8, -0.45, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, 0.35, 0.4);
	glVertex3f(-4, -0.35, 0.4);
	glVertex3f(-4.05, -0.35, 0.3);
	glVertex3f(-4.05, 0.35, 0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, 0.4, 0.3);
	glVertex3f(-4, 0.4, -0.3);
	glVertex3f(-4.05, 0.35, -0.3);
	glVertex3f(-4.05, 0.35, 0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, 0.4, 0.3);
	glVertex3f(-4.05, 0.35, 0.3);
	glVertex3f(-4, 0.35, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, 0.4, -0.3);
	glVertex3f(-4.05, 0.35, -0.3);
	glVertex3f(-4, 0.35, -0.4);
	glEnd();

	//--------------------------------------
	//back

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, 0.4, -0.3);
	glVertex3f(-0.8, 0.5, -0.3);
	glVertex3f(-0.8, 0.45, -0.4);
	glVertex3f(-4, 0.35, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.8, 0.5, -0.3);
	glVertex3f(-0.5, 0.7, -0.3);
	glVertex3f(-0.5, 0.65, -0.4);
	glVertex3f(-0.8, 0.45, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.5, 0.7, -0.3);
	glVertex3f(0.5, 0.7, -0.3);
	glVertex3f(0.5, 0.65, -0.4);
	glVertex3f(-0.5, 0.65, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.5, 0.7, -0.3);
	glVertex3f(0.8, 0.5, -0.3);
	glVertex3f(0.8, 0.45, -0.4);
	glVertex3f(0.5, 0.65, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.8, 0.5, -0.3);
	glVertex3f(1.3, 0.37, -0.3);
	glVertex3f(1.3, 0.32, -0.4);
	glVertex3f(0.8, 0.45, -0.4);
	glEnd();


	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(1.3, -0.37, -0.3);
	glVertex3f(0.8, -0.5, -0.3);
	glVertex3f(0.8, -0.45, -0.4);
	glVertex3f(1.3, -0.32, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.8, -0.5, -0.3);
	glVertex3f(0.5, -0.7, -0.3);
	glVertex3f(0.5, -0.65, -0.4);
	glVertex3f(0.8, -0.45, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.5, -0.7, -0.3);
	glVertex3f(-0.5, -0.7, -0.3);
	glVertex3f(-0.5, -0.65, -0.4);
	glVertex3f(0.5, -0.65, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.5, -0.7, -0.3);
	glVertex3f(-0.8, -0.5, -0.3);
	glVertex3f(-0.8, -0.45, -0.4);
	glVertex3f(-0.5, -0.65, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-0.8, -0.5, -0.3);
	glVertex3f(-4, -0.4, -0.3);
	glVertex3f(-4, -0.35, -0.4);
	glVertex3f(-0.8, -0.45, -0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, 0.35, -0.4);
	glVertex3f(-4, -0.35, -0.4);
	glVertex3f(-4.05, -0.35, -0.3);
	glVertex3f(-4.05, 0.35, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, -0.4, 0.3);
	glVertex3f(-4, -0.4, -0.3);
	glVertex3f(-4.05, -0.35, -0.3);
	glVertex3f(-4.05, -0.35, 0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, -0.4, 0.3);
	glVertex3f(-4.05, -0.35, 0.3);
	glVertex3f(-4, -0.35, 0.4);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(-4, -0.4, -0.3);
	glVertex3f(-4.05, -0.35, -0.3);
	glVertex3f(-4, -0.35, -0.4);
	glEnd();

	if (i == 1)
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	glTranslatef(0, 0, -1.2);
	drawDonut(0.2, 0.28, 5.5, t);
	glPopMatrix();

	if (i == 1)
		glColor3f(1, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	else
		glColor3f(1, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	glTranslatef(0, 0, -0.2 + bulletz);
	drawCone(0.19, 0.5, 15, 15, t);
	glPopMatrix();

	glPopMatrix();
}

void heel(int i, int t) {

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.21, 0.4);
	glVertex3f(0.3, 0, -0.3);
	glTexCoord2f(0.5, 0.4);
	glVertex3f(0.3, 0, 0.3);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(0.3, -1, 0.3);
	glTexCoord2f(0.225, 0.0);
	glVertex3f(0.1, -1, -0.3);
	glTexCoord2f(0.135, 0.25);
	glVertex3f(0.3, -0.4, -0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.21, 0.4);
	glVertex3f(-0.3, 0, -0.3);
	glTexCoord2f(0.5, 0.4);
	glVertex3f(-0.3, 0, 0.3);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(-0.3, -1, 0.3);
	glTexCoord2f(0.225, 0.0);
	glVertex3f(-0.1, -1, -0.3);
	glTexCoord2f(0.135, 0.25);
	glVertex3f(-0.3, -0.4, -0.5);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.3, 0, -0.3);
	glVertex3f(0.3, 0, 0.3);
	glVertex3f(-0.3, 0, 0.3);
	glVertex3f(-0.3, 0, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.3, 0, 0.3);
	glVertex3f(0.3, -1, 0.3);
	glVertex3f(-0.3, -1, 0.3);
	glVertex3f(-0.3, 0, 0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glVertex3f(0.3, -1, 0.3);
	glVertex3f(0.1, -1, -0.3);
	glVertex3f(-0.1, -1, -0.3);
	glVertex3f(-0.3, -1, 0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.31, 0.0);
	glVertex3f(0.1, -1, -0.3);
	glTexCoord2f(0.5, 0.3);
	glVertex3f(0.3, -0.4, -0.5);
	glTexCoord2f(0, 0.3);
	glVertex3f(-0.3, -0.4, -0.5);
	glTexCoord2f(0.15, 0.0);
	glVertex3f(-0.1, -1, -0.3);
	glEnd();

	if (i == 1) {
		pc++;
		glBegin(GL_POLYGON);
	}
	else {
		lineC++;
		glBegin(GL_LINE_LOOP);
	}
	glTexCoord2f(0.5, 0.0);
	glVertex3f(0.3, 0, -0.3);
	glTexCoord2f(0.5, 0.2);
	glVertex3f(0.3, -0.4, -0.5);
	glTexCoord2f(0, 0.2);
	glVertex3f(-0.3, -0.4, -0.5);
	glTexCoord2f(0, 0.0);
	glVertex3f(-0.3, 0, -0.3);
	glEnd();
}

void waist(int i, int t) {

	if (i == 1)
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix(); //waist
	glScalef(3.0, 0.5, 2);
	glTranslatef(0, -0.5, 0);
	drawCube(i);
	glPopMatrix();

	glPushMatrix(); //waist
	glTranslatef(0, -1, 0);
	glScalef(0.4, 1, 1);
	glTranslatef(0, 0, 0);
	drawCube(i);
	glPopMatrix();

	glPushMatrix(); // left thigh joint
	glTranslatef(0.55, -1.2, 0);
	glScalef(0.8, 0.8, 0.8);
	drawSphere(0.5, 15, 15, t);
	glPopMatrix();

	glPushMatrix(); // right thigh joint
	glTranslatef(-0.55, -1.2, 0);
	glScalef(0.8, 0.8, 0.8);
	drawSphere(0.5, 15, 15, t);
	glPopMatrix();
}

void leftUpperThigh(int i, int t) {
	if (i == 1)
		glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix(); //big thigh
	glScalef(1, 1, 1.1);
	glRotatef(45, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	drawCylinder2(0.7, 0.59, 3, 8, 1, t);
	glPopMatrix();

}

void rightUpperThigh(int i, int t) {
	if (i == 1)
		glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix(); //big thigh
	glScalef(1, 1, 1.1);
	glRotatef(45, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	drawCylinder2(0.7, 0.59, 3, 8, 1, t);
	glPopMatrix();

}

void leftLowerThigh(int i, int t) {
	// knee	
	glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	drawFullCylinder(0.5, 1.2, t);
	glPopMatrix();

	//----------------------------------------------------------------------------
	// small thigh
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black1.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.6, 0, 0.0);
	glScalef(1, 1, 1.1);
	glRotatef(45, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 0);
	drawCylinder2(0.7, 0.9, 4, 4, 1, t);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//----------------------------------------------------------------------------

	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black2.bmp");
				glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black4.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.6, 0, 0.0);
	glScalef(1, 1, 1.1);
	glRotatef(45, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	drawCylinder2(0.8, 1.1, 0.3, 4, 1, t);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -0.3, 0.0);
	glScalef(1, 1, 1.1);
	glRotatef(45, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	drawCylinder2(1.1, 1.1, 1.4, 4, 1, t);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -1.7, 0.0);
	glScalef(1, 1, 1.1);
	glRotatef(45, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	drawCylinder2(1.1, 0.81, 0.3, 4, 1, t);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//----------------------------------------------------------------------------
	// foot armament
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black2.bmp");
				glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	if (!isWalking) {
		glRotatef(-10, 0, 1, 0);
		glRotatef(5, 0, 0, 1), glTranslatef(-0.35, 0, 0);
	}
	glPushMatrix(); // foot armament
	glTranslatef(-0.60, -4.4, 0.9);
	glScalef(1.7, 1, 1);
	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 0, 0);
	footArmamanet(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);
	//--------------------------------------------------------------------
	// ankle
	glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	glPushMatrix();
	glTranslatef(-0.6, -4.2, 0);
	drawSphere(0.4, 15, 15, t);
	glPopMatrix();

	//--------------------------------------------------------------------
	//foot middle

	glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glPushMatrix();
	glTranslatef(-0.6, -4.7, 0);
	glScalef(0.9, 0.9, 1.29);
	glTranslatef(0, 0, 0);
	drawCube(i);
	glPopMatrix();

	//----------------------------------------------------------------------------
	//foot front

	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black1.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.6, -5.8, 0.7);
	glScalef(1.7, 1.7, 1.7);
	glTranslatef(0, 0, 0);
	drawFoot(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//----------------------------------------------------------------------------
	//foot heel
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black2.bmp");
				glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.6, -4.2, -0.5);
	glScalef(2.3, 1.6, 1.4);
	glTranslatef(0, 0, 0);
	heel(i, t);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	glPopMatrix();
}

void rightLowerThigh(int i, int t) {

	glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	glPushMatrix(); // knee	
	glRotatef(-90, 0, 1, 0);
	drawFullCylinder(0.5, 1.2, t);
	glPopMatrix();

	//----------------------------------------------------------------------------
	// small thigh
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black1.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.6, 0, 0.0);
	glScalef(1, 1, 1.1);
	glRotatef(45, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 0);
	drawCylinder2(0.7, 0.9, 4, 4, 1, t);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//----------------------------------------------------------------------------

	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black2.bmp");
				glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey4);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black4.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.6, 0, 0.0);
	glScalef(1, 1, 1.1);
	glRotatef(45, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	drawCylinder2(0.8, 1.1, 0.3, 4, 1, t);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -0.3, 0.0);
	glScalef(1, 1, 1.1);
	glRotatef(45, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	drawCylinder2(1.1, 1.1, 1.4, 4, 1, t);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -1.7, 0.0);
	glScalef(1, 1, 1.1);
	glRotatef(45, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	drawCylinder2(1.1, 0.81, 0.3, 4, 1, t);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//----------------------------------------------------------------------------
	// foot armament
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black2.bmp");
				glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	if (!isWalking) {
		glRotatef(10, 0, 1, 0);
		glRotatef(-5, 0, 0, 1), glTranslatef(0.35, -0.1, 0);
	}
	glPushMatrix(); // foot armament
	glTranslatef(-0.60, -4.4, 0.9);
	glScalef(1.7, 1, 1);
	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 0, 0);
	footArmamanet(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//--------------------------------------------------------------------
	// ankle
	glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	glPushMatrix();
	glTranslatef(-0.6, -4.2, 0);
	drawSphere(0.4, 15, 15, t);
	glPopMatrix();

	//--------------------------------------------------------------------
	//foot middle

	glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glPushMatrix();
	glTranslatef(-0.6, -4.7, 0);
	glScalef(0.9, 0.9, 1.29);
	drawCube(i);
	glPopMatrix();

	//----------------------------------------------------------------------------
	//foot front

	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black1.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.6, -5.8, 0.7);
	glScalef(1.7, 1.7, 1.7);
	glTranslatef(0, 0, 0);
	drawFoot(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//----------------------------------------------------------------------------
	//foot heel
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black2.bmp");
				glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.6, -4.2, -0.5);
	glScalef(2.3, 1.6, 1.4);
	glTranslatef(0, 0, 0);
	heel(i, t);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	glPopMatrix();
}

void leftLeg(int i, int t) {

	glPushMatrix(); //left thigh joint
	leftUpperThigh(i, t);
	glPopMatrix();

	glPushMatrix(); //left knee
	glTranslatef(0.7, -3.2, 0.0);
	if (walkRotate > 0)
		if (walkRotate > 25)
			glRotatef(25, walkx, 0, 0);
		else
			glRotatef(walkRotate, walkx, 0, 0);
	else
		if (-walkRotate > 25)
			glRotatef(25, walkx, 0, 0);
		else
			glRotatef(-walkRotate, walkx, 0, 0);
	glScalef(1.2, 1, 1.2);
	glTranslatef(0, 0, 0);
	leftLowerThigh(i, t);
	glPopMatrix();
}

void rightLeg(int i, int t) {

	glPushMatrix(); //right thigh joint
	rightUpperThigh(i, t);
	glPopMatrix();

	glPushMatrix(); //right knee
	glTranslatef(0.7, -3.2, 0.0);
	if (walkRotate > 0)
		if (walkRotate > 25)
			glRotatef(25, walkx, 0, 0);
		else
			glRotatef(walkRotate, walkx, 0, 0);
	else
		if (-walkRotate > 25)
			glRotatef(25, walkx, 0, 0);
		else
			glRotatef(-walkRotate, walkx, 0, 0);
	glScalef(1.2, 1, 1.2);
	glTranslatef(0, 0, 0);
	rightLowerThigh(i, t);
	glPopMatrix();
}

void backCannon(int i, int t) {

	glPushMatrix();
	glTranslatef(1.8, 0, -0.2 + cannontz);
	glRotatef(-cannonRotate / 2, 0, 1, 0);
	canon(i, t);
	glPopMatrix();

	if (i == 1)
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0.8, 0, 0);
	glScalef(1.5, 0.3, 0.3);
	glTranslatef(0, 0, 0);
	drawCube(i);
	glPopMatrix();

	glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	glPushMatrix();
	glTranslatef(0, 0.2, 0);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 0);
	drawFullCylinder(0.2, 0.4, t);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.6, 0.2, 0);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 0);
	drawFullCylinder(0.2, 0.4, t);
	glPopMatrix();


}

void lowerBody(int i, int t) {

	glPushMatrix();
	waist(i, t);
	glPopMatrix();

	//--------------------------------------------------
	//left frontskirt
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black1.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.3, -0.1, 1);
	glScalef(1.1, 1.3, 0.6);
	glRotatef(180, 0, 1, 0);
	if (-walkRotate > 0 && -walkRotate <= 25)
		glRotatef(-walkRotate, walkx, 0, 0);
	else if (-walkRotate > 25) {
		walkRotate = -25;
		glRotatef(-walkRotate, walkx, 0, 0);
	}
	else
		glRotatef(0, walkx, 0, 0);
	glTranslatef(0, 0, 0);
	leftFrontSkirt(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//--------------------------------------------------------
	//right frontskirt
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black1.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.3, -0.1, 1);
	glScalef(1.1, 1.3, 0.6);
	glRotatef(180, 0, 1, 0);
	if (walkRotate > 0 && walkRotate <= 25)
		glRotatef(walkRotate, walkx, 0, 0);
	else if (walkRotate > 25) {
		walkRotate = 25;
		glRotatef(walkRotate, walkx, 0, 0);
	}
	else
		glRotatef(0, walkx, 0, 0);
	glTranslatef(0, 0, 0);
	rightFrontSkirt(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//--------------------------------------------------
	//left mini frontskirt
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black2.bmp");
				glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0.3, -0.1, 1.22);
	glRotatef(180, 0, 1, 0);
	glScalef(0.4, 0.6, 0.4);
	if (-walkRotate > 0 && -walkRotate <= 25)
		glRotatef(-walkRotate, walkx, 0, 0);
	else if (-walkRotate > 25) {
		walkRotate = -25;
		glRotatef(-walkRotate, walkx, 0, 0);
	}
	else
		glRotatef(0, walkx, 0, 0);
	glTranslatef(0, 0, 0);
	leftFrontMiniSkirt(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//-----------------------------------------------------
	//right mini frontskirt
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black2.bmp");
				glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey2);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.3, -0.1, 1.22);
	glRotatef(180, 0, 1, 0);
	glScalef(0.4, 0.6, 0.4);
	if (walkRotate > 0 && walkRotate <= 25)
		glRotatef(walkRotate, walkx, 0, 0);
	else if (walkRotate > 25) {
		walkRotate = 25;
		glRotatef(walkRotate, walkx, 0, 0);
	}
	else
		glRotatef(0, walkx, 0, 0);
	glTranslatef(0, 0, 0);
	rightFrontMiniSkirt(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//--------------------------------------------------
	 //middle pole
	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("white1.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("white1.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(0, 0, 0.7);
	glRotatef(180, 0, 1, 0);
	glScalef(1, 1.9, 1);
	glTranslatef(0, 0, 0);
	thirdLeg(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//--------------------------------------------------
	//rear skirt

	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black1.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslatef(0, -0.2, -1);
	glRotatef(7, 1, 0, 0);
	glScalef(1.4, 1.8, 1);
	glTranslatef(0, 0, 0);
	rearSkirt(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//--------------------------------------------------
	//left sideskirt

	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black2.bmp");
				glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-1.6, -0.32, 1);
	glRotatef(-10, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glScalef(2, 1.7, 1.3);
	glTranslatef(0, 0, 0);
	leftSideSkirt(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//--------------------------------------------------
	//right sideskirt

	if (i == 1) {
		if (isTexture == 1 || isTexture == 2) {
			if (isTexture == 1) {
				textures[1] = loadTexture("black2.bmp");
				glColor3f(0.4, 0.4, 0.4), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey5);
			}
			else {
				textures[1] = NULL;
				textures[1] = loadTexture("black3.bmp");
				glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			}
		}
		else
			glColor3f(0.1, 0.1, 0.1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey1);
	}
	else
		glColor3f(0, 0, 0), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(1.6, -0.32, 1);
	glRotatef(10, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glScalef(2, 1.7, 1.3);
	glTranslatef(0, 0, 0);
	rightSideSkirt(i);
	glPopMatrix();
	glDeleteTextures(1, &textures[1]);

	//--------------------------------------------------
	//left connector
	if (i == 1)
		glColor3f(0.2, 0.2, 0.2), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
	else
		glColor3f(0, 0, 0);

	glPushMatrix();
	glTranslatef(-1.6, -0.1, -1.2);
	glScalef(1, 1, 0.9);
	glTranslatef(0, 0, 0);
	connector(t);
	glPopMatrix();

	//--------------------------------------------------
	//right connector
	glPushMatrix();
	glTranslatef(1.6, -0.1, -1.2);
	glScalef(1, 1, 0.9);
	glTranslatef(0, 0, 0);
	connector(t);
	glPopMatrix();

	//--------------------------------------------------
	//canon
	if (i == 1)
		glColor3f(1, 1, 1), glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	else
		glColor3f(0, 0, 0);

	glPushMatrix();
	glTranslatef(0, 0, -1.3);
	glRotatef(cannonRotate / 4, 0, 0, 1);
	glRotatef(cannonRotate, 0, 1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, 0, 0);
	backCannon(i, t);
	glPopMatrix();

	//--------------------------------------------------
	//left leg
	glPushMatrix();
	glTranslatef(-0.95, -0.7, 0);
	if (!isWalking)
		glRotatef(-5 - legRotatey, 0, 0, 1);
	glScalef(0.8, 0.8, 0.8);
	if (-walkRotate > 10)
		glRotatef(10, walkx, 0, 0);
	else if (-walkRotate < -25)
		glRotatef(-25, walkx, 0, 0);
	else
		glRotatef(-walkRotate, walkx, 0, 0);
	glTranslatef(0, 0, 0);
	leftLeg(i, t);
	glPopMatrix();

	//--------------------------------------------------
	//right leg
	glPushMatrix();
	glTranslatef(0.95, -0.7, 0);
	if (!isWalking)
		glRotatef(5 + legRotatey, 0, 0, 1);
	glScalef(0.8, 0.8, 0.8);
	if (walkRotate > 10)
		glRotatef(10, walkx, 0, 0);
	else if (walkRotate < -25)
		glRotatef(-25, walkx, 0, 0);
	else
		glRotatef(walkRotate, walkx, 0, 0);
	glTranslatef(0, 0, 0);
	rightLeg(i, t);
	glPopMatrix();

}

void animation() {

	if (isWalking) {
		legRotatey = 0;
		upperBodyRotate = 0;
		walkx = 1;
		if (walktx < 7)
			walktx += 0.1;
		if (walkRotate >= 25 || isTop) {
			walkRotate -= 1.5;
			if (walkRotate <= -25) isTop = !isTop;
			if (playAnimation != 3) armAnimState = 2;
		}
		else {
			walkRotate += 1.5;
			if (walkRotate >= 25) isTop = !isTop;
			if (playAnimation != 3) armAnimState = 3;
		}
	}
	else {
		walkx = walkRotate = walktx = 0;
	}

	if (isCannon) {
		legRotatey = 0;
		upperBodyRotate = 0;
		if (cannonRotate < 180) {
			cannonRotate += 6;
			if (cannonRotate > 135)
				cannontz += 0.03;
		}
	}
	else {
		if (cannonRotate > 0) {
			cannonRotate -= 6;
			if (cannonRotate > 135)
				cannontz -= 0.03;
			if (cannonRotate <= 135)
				cannontz = 0;
		}
	}

	if (isShooting) {
		legRotatey = 0;
		upperBodyRotate = 0;
		if (cannonRotate == 180)
			bulletz += 0.3;
		else {
			isShooting = !isShooting;
		}
	}
	else
		bulletz = 0;

}

//-------------------------------------------------------------------------------------

void display()
{
	Projection();
	//glClearColor(1,1,1,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();	// First push
	animation();
	lighting();
	glRotatef(objRotateX, 1, 0, 0);
	glRotatef(objRotateY, 0, 1, 0);
	glRotatef(objRotateZ, 0, 0, 1);
	glScalef(zoom + 1.0, zoom + 1.0, zoom + 1.0);

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	//textures[0] = loadTexture("night.bmp");
	skyBox(35, 2);
	//glDeleteTextures(1, &textures[0]);
	glPopMatrix();
	glScalef(0.7, 0.7, 0.7);

	//----------------------------------------
	glPushMatrix();
	glRotatef(upperBodyRotate, 0, 1, 0);

	glPushMatrix(); //head
	glTranslatef(0, 0, walktx);
	glTranslatef(0.0, 3.7, 0.01);
	glRotatef(headRotateDegree, 0.0, 1.0, 0.0);
	glScalef(0.36, 0.36, 0.36);
	head(1, 0);
	glPopMatrix();

	glPushMatrix(); //upper body
	glTranslatef(0, 0, walktx);
	upperBody();
	glPopMatrix();

	glPushMatrix(); //upper body
	glTranslatef(0, 0, walktx);
	wing();
	glPopMatrix();

	glPushMatrix(); //right arm
	glTranslatef(0, 0, walktx);
	glTranslatef(-2, 3.4, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glScalef(0.3, 0.3, 0.3);
	rightArmAnimated(1, 0);
	glPopMatrix();

	glPushMatrix(); //left arm
	glTranslatef(0, 0, walktx);
	glTranslatef(2, 3.4, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	glScalef(0.3, 0.3, 0.3);
	leftArmAnimated(1, 0);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glRotatef(lbRotatey, 0, 1, 0);

	glPushMatrix(); //lower body
	glTranslatef(0, 0, walktx);
	lowerBody(1, 2);
	glLineWidth(1.5);
	lowerBody(0, 1);
	glPopMatrix();

	glPopMatrix();
	//----------------------------------------

	glPopMatrix();	// First pop
	if (showGuideLine == true)
		guideLine();

	if (pcCount)
	{
		pcTotal = pc;
		cubeTotal = cubeC;
		pyramidTotal = pyramidC;
		sphereTotal = sphereC;
		cylinderTotal = cylinderC;
		coneTotal = coneC;
		quadTotal = quadC;
		triangleTotal = triangleC;
		circleTotal = circleC;
		lineTotal = lineC;
		pcCount = false;
	}

}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1000, 1000,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}

//glTranslatef(va, vb, vc);
	//glScalef(vd + 1.0, ve + 1.0, vf + 1.0);
	//glRotatef(rz, 0, 0, vi);
	//glRotatef(ry, 0, vh, 0);
	//glRotatef(rx, vg, 0, 0);
	//glTranslatef(0, 0, 0);