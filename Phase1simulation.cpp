/*
	Ashkenazi, Nir
	Kim, Yun
	Wasifi, Walid

	Project Warbird Simulation
	Phase 1
	Oct 6, 2014
	Phase1simulation.cpp
*/

# define __Windows__ // define your target operating system
# include "../includes465/include465.hpp"  

# include "Shape3D.hpp"

// Shapes
const int nShapes = 7;
Shape3D * shape[nShapes];
// Model for shapes

char * modelFile[nShapes] = 
	{	"Ruber.tri", "Unum.tri", "Duo.tri", "Primus.tri", "Secundus.tri", "WarBird.tri", "missile.tri"}; 
const GLuint nVertices[nShapes] = 
	{	792 * 3, 792 * 3, 792 * 3, 792 * 3, 792 * 3, 148 * 3, 60 * 3};  
float boundingRadius[nShapes];  
int Index =  0;  
float modelSize[nShapes] = 
	{	2000.0f, 200.0f, 400.0f, 100.0f, 150.0f, 100.0f, 25.0f};
const int Ruber = 0, Unum = 1, Duo = 2, Primus = 3, Secundus = 4, Warbird = 5, Missile = 6;

GLuint vao[nShapes];  // VertexArrayObject
GLuint buffer[nShapes];  // Vertex Buffer Object
GLuint shaderProgram; 
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";

// shader handles
GLuint MVP ;  // Model View Projection matrix's handle
GLuint Position[nShapes], Color[nShapes], Normal[nShapes];   // vPosition, vColor, vNormal handles for models

glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 modelMatrix;          // set in shape[i]-->updateDraw()
glm::mat4 viewMatrix;           // set in keyboard()
glm::mat4 ModelViewProjectionMatrix; // set in display();
glm::vec3 scale[nShapes]; 

// display state and "state strings" for title display
// window title strings
char baseStr[50] =    "465 Ruber system phase 1: {f, t, w, u, d, a} : ";
char viewStr[15] =    " Front view /";
char timerStr[20] = " / Normal speed / ";
char titleStr [100]; 
char fpsStr[15] = "";
int timerDelay = 40, frameCount = 0;
double currentTime, lastTime, timeInterval; 

// camera
// vectors and values for lookAt
glm::vec3 eye, at, up;
int viewstate = 0;

// vectors for "model"
glm::vec4 vertex[nShapes];
glm::vec3 normal[nShapes];
glm::vec4 diffuseColorMaterial[nShapes];

// rotation variables
glm::mat4 identity(1.0f); 
glm::mat4 rotation;
// A delay of 40 milliseconds is 25 updates / second

void init (void) 
{
	shaderProgram = loadShaders(vertexShaderFile,fragmentShaderFile);
	glUseProgram(shaderProgram);
  
	// generate VAOs and VBOs
	glGenVertexArrays( nShapes, vao );
	glGenBuffers( nShapes, buffer );



	// load the buffers from the model files
	for (int i = 0; i < nShapes; i++) 
	{
		boundingRadius[i] = loadModelBuffer(modelFile[i], nVertices[i], vao[i], buffer[i], 
							shaderProgram, Position[i], Color[i], Normal[i], "vPosition", "vColor", "vNormal"); 
		scale[i] = glm::vec3( modelSize[i] * 1.0f  /boundingRadius[i]);		// set scale for models given bounding radius  
	}
    
	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");

	// inital view
	eye = glm::vec3(0.0f, 30000.0f, 20000.0f);
	at = glm::vec3(0);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	
	viewMatrix = glm::lookAt(eye, at, up);
  
	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.02f, 0.02f, 0.025f, 1.0f); //RGBA

	// create shape
	for(int i = 0; i < nShapes; i++)
	{
		shape[i] = new Shape3D(i);
		shape[i]->setScale(scale[i]);
	}

	printf("%d Shapes created \n", nShapes);

	lastTime = glutGet(GLUT_ELAPSED_TIME); /////////////////////////////////////
}

void reshape(int width, int height) 
{
	glViewport(0, 0, width, height);
	projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat) width /  (GLfloat) height, 1.0f, 100000.0f); 
}

// update and display animation state in window title
void updateTitle() 
{
	strcpy(titleStr, baseStr);
	strcat(titleStr, viewStr);
	strcat(titleStr, fpsStr);
	strcat(titleStr, timerStr);
	glutSetWindowTitle(titleStr);
}

void display(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// update model matrix, set MVP, draw
	for(int i = 0; i < nShapes; i++) 
	{ 
		if (i == 3)
			modelMatrix = shape[3]->getModelMatrix(shape[Duo]->getPosition());
		else 
			modelMatrix = shape[i]->getModelMatrix();
		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix; 
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix)); 
		glBindVertexArray(vao[i]);
		glEnableVertexAttribArray(Position[i]);
		glEnableVertexAttribArray(Color[i]);
		glEnableVertexAttribArray(Normal[i]);
		glDrawArrays(GL_TRIANGLES, 0, nVertices[i]);
	}
	glutSwapBuffers();
	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;
	if ( timeInterval >= 1000) 
	{
		sprintf(fpsStr, " fps %4d", (int) (frameCount / (timeInterval / 1000.0f)) );
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}
}

void unumduoview (unsigned char key)
{
	if (key == 'u' || key == 'U')
	{
		glm::mat4 unum_loc = shape[Unum]->get_rotation() * shape[Unum]->get_translation();
		eye = glm::vec3(unum_loc[3].x, 5000.0f, unum_loc[3].z);
		at = glm::vec3(unum_loc[3].x, 0.0f, unum_loc[3].z);
		up = glm::vec3(0.0f, 0.0f, -1.0f);
		strcpy(viewStr, "Unum view / ");
	}

	if (key == 'd' || key == 'D')
	{
		glm::mat4 duo_loc = shape[Duo]->get_rotation() * shape[Duo]->get_translation();
		eye = glm::vec3(duo_loc[3].x, 5000.0f, duo_loc[3].z);
		at = glm::vec3(duo_loc[3].x, duo_loc[3].y, duo_loc[3].z);
		up = glm::vec3(1.0f, 0.0f, 0.0f);
		strcpy(viewStr, "Duo view / ");
	}
	viewMatrix = glm::lookAt(eye, at, up);
}

// Animate scene objects by updating their transformation matrices
// timerDelay = 40, or 25 updates / second
void update (int i) 
{ 
	glutTimerFunc(timerDelay, update, 1);
	for(int i = 0; i < nShapes; i++) 
		shape[i] -> update();
}

// Quit or set the view
void keyboard (unsigned char key, int x, int y) 
{
	switch(key) 
	{
		case 033 : case 'q' :  case 'Q' : exit(EXIT_SUCCESS); 
			break;

		case 'f' : case 'F' :  // front view
			eye = glm::vec3(0.0f, 20000.0f, 20000.0f);
			at = glm::vec3(0);
			up = glm::vec3(0.0f, 1.0f, 0.0f);
			strcpy(viewStr, " Front view / "); 
			break;

		case 't' : case 'T' :  // top view
			eye = glm::vec3(0.0f, 30000.0f,  0.0f);  
			at  = glm::vec3(0.0f,     0.0f,  0.0f);    
			up  = glm::vec3(1.0f,     0.0f,  0.0f);   
			strcpy(viewStr, " Top view / ");
			break;

		case 'w' : case 'W' : // War bird view
			eye = glm::vec3(5000.0f, 2000.0f, 6000.0f);   
			at  = glm::vec3(5000.0f, 1200.0f, 5000.0f);     
			up  = glm::vec3(0.0f,   1.0f,    0.0f);   
			strcpy(viewStr, " War Bird view / ");
			break;

		case 'u' : case 'U' : case 'd' : case 'D' : // Unum view
			unumduoview(key);			
			break;

		case 'a' : case 'A' : // increase speed
			if (timerDelay == 40)
			{
				timerDelay = 0;
				strcpy(timerStr, " / Fast speed / ");
			}
			else
			{
				timerDelay = 40;
				strcpy(timerStr, " / Normal speed / ");
			}
			break;
	}
	viewMatrix = glm::lookAt(eye, at, up);
	updateTitle();
}
    
int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("465 manyCubes Example {f, t, w, u, d, a} : Front view / ");
	// initialize and verify glew
	glewExperimental = GL_TRUE;  // needed my home system 
	GLenum err = glewInit();  
	if (GLEW_OK != err) 
		printf("GLEW Error: %s \n", glewGetErrorString(err));      
	else 
	{
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n", 
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	// initialize scene
	init();
	// set glut callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(timerDelay, update, 1);
	glutIdleFunc(display);
	glutMainLoop();
	printf("done\n");
	return 0;
}
  

