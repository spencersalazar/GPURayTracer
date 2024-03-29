// main.cpp

//
// For this project, we use OpenGL, GLUT
// and GLEW (to load OpenGL extensions)
//
#include "stglew.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

//
// Globals used by this application.
// As a rule, globals are Evil, but this is a small application
// and the design of GLUT makes it hard to avoid them.
//

// Window size, kept for screenshots
static int gWindowSizeX = 0;
static int gWindowSizeY = 0;

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

	float leftX   = -1;
	float rightX  = 1;

	float planeY  = 0;

    float aspect = ((float)gWindowSizeX)/((float)gWindowSizeY);
	float nearZ   = -1*aspect;
	float farZ    = 1*aspect;

    // Invoke the shader.  Now OpenGL will call our
    // shader programs on anything we draw.
    shader->Bind();
    
    float theta = -M_PI/2.0 + M_PI/8.0*cosf(g_t*2*M_PI*0.25) * expf(-g_t/25.0);
    
    shader->SetUniform("spheres[0].C", STVector3(4.5*cosf(theta), 4.75+4.5*sinf(theta), 1.25));
    shader->SetUniform("spheres[0].r", 0.3);
    shader->SetUniform("spheres[0].color", STColor4f(1.0, 1.0, 0.0, 1.0));
    shader->SetUniform("spheres[0].shininess", 64.0);
    shader->SetUniform("spheres[0].reflectiveness", 0.1);
    shader->SetUniform("spheres[0].specular", 1.0);
    shader->SetUniform("spheres[0].diffuse", 1.0);
    
//    shader->SetUniform("spheres[1].C", STVector3(0, -2.5+0.5*sinf(g_t*2*M_PI*0.1) * expf(-g_t/25.0), 2));
//    shader->SetUniform("spheres[1].r", 2);
//    shader->SetUniform("spheres[1].color", STColor4f(0.5, 0.0, 0.0, 1.0));
//    shader->SetUniform("spheres[1].shininess", 10.0);
//    shader->SetUniform("spheres[1].reflectiveness", 0.5);
//    shader->SetUniform("spheres[1].specular", 1.0);
//    shader->SetUniform("spheres[1].diffuse", 1.0);

//    shader->SetUniform("spheres[2].C", STVector3(0, 0, 2));
//    shader->SetUniform("spheres[2].r", 0.5);
//    shader->SetUniform("spheres[2].color", STColor4f(1.0, 1.0, 0.0, 1.0));
//    shader->SetUniform("spheres[2].shininess", 64.0);
    
    glUniform1i(glGetUniformLocation(shader->programid, "numSpheres"), 1);
    
    shader->SetUniform("tris[0].v[0]", STVector3(-20, -0.25, 20));
    shader->SetUniform("tris[0].v[1]", STVector3(20, -0.25, 20));
    shader->SetUniform("tris[0].v[2]", STVector3(0, -0.25, -1));
    shader->SetUniform("tris[0].color", STColor4f(0.5, 0.0, 0.0, 1.0));
    shader->SetUniform("tris[0].shininess", 10.0);
    shader->SetUniform("tris[0].reflectiveness", 0.5);
    shader->SetUniform("tris[0].specular", 1.0);
    shader->SetUniform("tris[0].diffuse", 1.0);
    
    glUniform1i(glGetUniformLocation(shader->programid, "numTris"), 1);
    
    shader->SetUniform("lights[0].position", STVector3(1.0, 1.0, 1.0));
    shader->SetUniform("lights[0].diffuseColor", STColor4f(0.75, 0.75, 0.75, 1.0));
    shader->SetUniform("lights[0].specularColor", STColor4f(0.75, 0.75, 0.75, 1.0));
    
    shader->SetUniform("lights[1].position", STVector3(-1.0, 1.0, 1.0));
    shader->SetUniform("lights[1].diffuseColor", STColor4f(0.75, 0.75, 0.75, 1.0));
    shader->SetUniform("lights[1].specularColor", STColor4f(0.75, 0.75, 0.75, 1.0));
    
    shader->SetUniform("lights[2].position", STVector3(2.5, 0.5, 0.5));
    shader->SetUniform("lights[2].diffuseColor", STColor4f(0.5, 0.5, 0.5, 1.0));
    shader->SetUniform("lights[2].specularColor", STColor4f(0.5, 0.5, 0.5, 1.0));
    
    shader->SetUniform("lights[3].position", STVector3(-2.5, 0.5, 0.5));
    shader->SetUniform("lights[3].diffuseColor", STColor4f(0.5, 0.5, 0.5, 1.0));
    shader->SetUniform("lights[3].specularColor", STColor4f(0.5, 0.5, 0.5, 1.0));
    
    glUniform1i(glGetUniformLocation(shader->programid, "numLights"), 4);

//    glUniform1i(glGetUniformLocation(shader->programid, "depth_max"), 2);

    
    {
        // Draw a coplanar quadrilateral on the y=0 plane.
        // This is the surface we will distort for the
        // assignment.
        glBegin(GL_QUADS);
        glColor4f(1, 1, 0, 1.0f);

        // All vertices on the plane have the same normal
        glNormal3f(0.0f, 1.0f, 0.0f);

//        for (int i = 0; i < XTesselationDepth; i++)
//            for (int j = 0; j < ZTesselationDepth; j++) {
//                float s0 = (float)  i      / (float) XTesselationDepth;
//                float s1 = (float) (i + 1) / (float) XTesselationDepth;
//                float x0 =  s0 * (rightX - leftX) + leftX;
//                float x1 =  s1 * (rightX - leftX) + leftX;
//
//                float t0 = (float) j       / (float) ZTesselationDepth;
//                float t1 = (float) (j + 1) / (float) ZTesselationDepth;
//                float z0 = t0 * (farZ - nearZ) + nearZ;
//                float z1 = t1 * (farZ - nearZ) + nearZ;
//
//                glTexCoord2f(s0, t0);
//                glVertex3f(x0, z0, planeY);
//
//                glTexCoord2f(s1, t0);
//                glVertex3f(x1, z0, planeY);
//
//                glTexCoord2f(s1, t1);
//                glVertex3f(x1, z1,  planeY);
//
//                glTexCoord2f(s0, t1);
//                glVertex3f(x0,  z1,  planeY);
//            }
        
        glTexCoord2f(0, 0);
        glVertex3f(leftX, nearZ, planeY);
        
        glTexCoord2f(1, 0);
        glVertex3f(rightX, nearZ, planeY);
        
        glTexCoord2f(1, 1);
        glVertex3f(rightX, farZ,  planeY);
        
        glTexCoord2f(0, 1);
        glVertex3f(leftX,  farZ,  planeY);

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
    float aspect = (float) gWindowSizeX / (float) gWindowSizeY;
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
        case 's':
        {
            //
            // Take a screenshot, and save as screenshot.jpg
            //
            
            char buf[64];
            time_t t = time(NULL);
            strftime(buf, 64, "%F %H-%M-%S", localtime(&t));
            
            STImage* screenshot = new STImage(gWindowSizeX, gWindowSizeY);
            screenshot->Read(0,0);
            screenshot->Save(std::string("screenshot ") + buf + ".jpg");
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
    if(argc >= 2)
        vertexShader   = std::string(argv[1]);
    else
        vertexShader = "kernels/default.vert";
    if(argc >= 3)
        fragmentShader = std::string(argv[2]);
    else
        fragmentShader = "kernels/rt.frag";
    
	if (argc > 3)
		usage();


    //
    // Initialize GLUT.
    //
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(20, 20);
    glutInitWindowSize(640, 480);
    glutCreateWindow("GPU Ray Tracer");
    
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
