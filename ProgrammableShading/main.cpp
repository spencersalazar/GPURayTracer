// main.cpp

//
// For this project, we use OpenGL, GLUT
// and GLEW (to load OpenGL extensions)
//
#include "stglew.h"

#include <stdio.h>
#include <string.h>

//
// Globals used by this application.
// As a rule, globals are Evil, but this is a small application
// and the design of GLUT makes it hard to avoid them.
//

// Window size, kept for screenshots
static int gWindowSizeX = 0;
static int gWindowSizeY = 0;

static int XTesselationDepth = 400;
static int ZTesselationDepth = 400;

// File locations
std::string vertexShader;
std::string fragmentShader;
std::string normalMap;
std::string lightProbe;

STShaderProgram *shader;

// Stored mouse position for camera rotation, panning, and zoom.
int gPreviousMouseX = -1;
int gPreviousMouseY = -1;
int gMouseButton = -1;
STVector3 mCameraTranslation;
float mCameraAzimuth;
float mCameraElevation;
bool teapot = false;

float g_t = 0;

void resetCamera()
{
    mCameraTranslation = STVector3(0.f, 1.f, 1.5f);
    mCameraAzimuth = 0.f;
    mCameraElevation = 65.0f;
}


//
// Initialize the application, loading all of the settings that
// we will be accessing later in our fragment shaders.
//
void Setup()
{
    shader = new STShaderProgram();
    shader->LoadVertexShader(vertexShader);
    shader->LoadFragmentShader(fragmentShader);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    gluOrtho2D(0, gWindowSizeX, 0, gWindowSizeY);
}

//
// Display the output image from our vertex and fragment shaders
//
void DisplayCallback()
{
    g_t += 1.0/30.0;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//    STVector3 trans = -mCameraTranslation;
//    
//    glTranslatef(trans.x, trans.y, trans.z);
    
//    glRotatef(-mCameraElevation, 1, 0, 0);
//    glRotatef(-mCameraAzimuth, 0, 1, 0);
    
//    glRotatef(-90.0f, 1, 0, 0);
//    glScalef(1.0, -1.0, 1.0);


	float leftX   = -1;
	float rightX  = 1;

	float planeY  = 0;

    float aspect = ((float)gWindowSizeX)/((float)gWindowSizeY);
	float nearZ   = -1*aspect;
	float farZ    = 1*aspect;

    // Invoke the shader.  Now OpenGL will call our
    // shader programs on anything we draw.
    shader->Bind();

    shader->SetUniform("c", STVector3(0, 0, 2));
    shader->SetUniform("r", 0.5);
    shader->SetUniform("L", STVector3(gWindowSizeX/2, gWindowSizeY, 2));
    
    {
        // Draw a coplanar quadrilateral on the y=0 plane.
        // This is the surface we will distort for the
        // assignment.
        glBegin(GL_QUADS);
        glColor4f(1, 1, 0, 1.0f);

        // All vertices on the plane have the same normal
        glNormal3f(0.0f, 1.0f, 0.0f);

        for (int i = 0; i < XTesselationDepth; i++)
            for (int j = 0; j < ZTesselationDepth; j++) {
                float s0 = (float)  i      / (float) XTesselationDepth;
                float s1 = (float) (i + 1) / (float) XTesselationDepth;
                float x0 =  s0 * (rightX - leftX) + leftX;
                float x1 =  s1 * (rightX - leftX) + leftX;

                float t0 = (float) j       / (float) ZTesselationDepth;
                float t1 = (float) (j + 1) / (float) ZTesselationDepth;
                float z0 = t0 * (farZ - nearZ) + nearZ;
                float z1 = t1 * (farZ - nearZ) + nearZ;

                glTexCoord2f(s0, t0);
                glVertex3f(x0, z0, planeY);

                glTexCoord2f(s1, t0);
                glVertex3f(x1, z0, planeY);

                glTexCoord2f(s1, t1);
                glVertex3f(x1, z1,  planeY);

                glTexCoord2f(s0, t1);
                glVertex3f(x0,  z1,  planeY);
            }

        glEnd();
    }
    
    shader->UnBind();

    glutSwapBuffers();
}

//
// Reshape the window and record the size so
// that we can use it for screenshots.
//
void ReshapeCallback(int w, int h)
{
	gWindowSizeX = w;
    gWindowSizeY = h;

    glViewport(0, 0, gWindowSizeX, gWindowSizeY);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
//	// Set up a perspective projection
    float aspect = (float) gWindowSizeX / (float) gWindowSizeY;
//	gluPerspective(30.0f, aspectRatio, .1f, 100.0f);
    gluOrtho2D(-1*aspect, 1*aspect, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpecialKeyCallback(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            break;
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_UP:
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void KeyCallback(unsigned char key, int x, int y)
{
    switch(key) {
        case 's': {
            //
            // Take a screenshot, and save as screenshot.jpg
            //
            STImage* screenshot = new STImage(gWindowSizeX, gWindowSizeY);
            screenshot->Read(0,0);
            screenshot->Save("screenshot.jpg");
            delete screenshot;
        }
            break;
        case 'r':
            resetCamera();
            break;
        case 't':
//            teapot = !teapot;
            break;
        case 'q':
            exit(0);
            break;
        case 'p':
            g_t = 0;
            break;
            
        default:
            break;
    }
    
    glutPostRedisplay();
}


/**
 * Mouse event handler
 */
void MouseCallback(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON)
    {
        gMouseButton = button;
    } else
    {
        gMouseButton = -1;
    }
    
    if (state == GLUT_UP)
    {
        gPreviousMouseX = -1;
        gPreviousMouseY = -1;
    }
}

/**
 * Mouse active motion callback (when button is pressed)
 /*/
void MouseMotionCallback(int x, int y)
{
}

void TimerCallback(int i)
{
    glutPostRedisplay();
    
    glutTimerFunc(1000/30, TimerCallback, 0);
}


void usage()
{
	printf("usage: ProgrammableShading vertShader fragShader\n");
	exit(0);
}

int main(int argc, char** argv)
{
	if (argc != 3)
		usage();

	vertexShader   = std::string(argv[1]);
	fragmentShader = std::string(argv[2]);

    //
    // Initialize GLUT.
    //
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(20, 20);
    glutInitWindowSize(640, 480);
    glutCreateWindow("CS148 Assignment 7");
    
    //
    // Initialize GLEW.
    //
#ifndef __APPLE__
    glewInit();
    if(!GLEW_VERSION_2_0) {
        printf("Your graphics card or graphics driver does\n"
			   "\tnot support OpenGL 2.0, trying ARB extensions\n");

        if(!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader) {
            printf("ARB extensions don't work either.\n");
            printf("\tYou can try updating your graphics drivers.\n"
				   "\tIf that does not work, you will have to find\n");
            printf("\ta machine with a newer graphics card.\n");
            exit(1);
        }
    }
#endif

    // Be sure to initialize GLUT (and GLEW for this assignment) before
    // initializing your application.

    Setup();

    ReshapeCallback(640, 480);
    
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutSpecialFunc(SpecialKeyCallback);
    glutKeyboardFunc(KeyCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MouseMotionCallback);
//    glutIdleFunc(DisplayCallback);
    glutTimerFunc(1000/30, TimerCallback, 0);

    glutMainLoop();


    // Cleanup code should be called here.


    return 0;
}
