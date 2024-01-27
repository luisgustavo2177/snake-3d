/*
	Implementação 3D do clássico jogo SNAKE
	UFPI - Ciência da Computação - Computação Gráfica 2023.2
	Prof: Laurindo de Sousa Britto Neto
	Aluno: Heitor Andrade Moura
		   Luis Gustavo da Silva Sousa
    Data: 04/02/2024
 */

//Bibliotecas usadas
#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <gl/glut.h>

#define UP 1
#define Down 2
#define LEFT 3
#define RIGHT 4

GLint wallPositions[][2] = {//posições das parede
	{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}, {17, 0}, {18, 0}, {19, 0}, {20, 0}, {21, 0}, {22, 0}, {23, 0}, {24, 0}, {25, 0}, {26, 0}, {27, 0}, {28, 0}, {29, 0}, {30, 0}, {31, 0}, {32, 0}, {33, 0}, {34, 0}, {35, 0}, {36, 0}, {37, 0}, {38, 0}, {39, 0}, {40, 0}, {41, 0}, {42, 0}, {43, 0}, {44, 0}, {45, 0}, {46, 0}, {47, 0}, {48, 0}, {49, 0}, {50, 0},
	{51, 0}, {52, 0}, {53, 0}, {54, 0}, {55, 0}, {56, 0}, {57, 0}, {58, 0}, {59, 0}, {60, 0}, {61, 0}, {62, 0}, {63, 0}, {64, 0}, {65, 0}, {66, 0}, {67, 0}, {68, 0}, {69, 0}, /*{70, 0}, {71, 0}, {72, 0}, {73, 0}, {74, 0}, {75, 0}, {76, 0}, {77, 0}, {78, 0}, {79, 0}, {80, 0},*/ {81, 0}, {82, 0}, {83, 0}, {84, 0}, {85, 0}, {86, 0}, {87, 0}, {88, 0}, {89, 0}, {90, 0}, {91, 0}, {92, 0}, {93, 0}, {94, 0}, {95, 0}, {96, 0}, {97, 0}, {98, 0}, {99, 0}, {100, 0},
	{101, 0}, {102, 0}, {103, 0}, {104, 0}, {105, 0}, {106, 0}, {107, 0}, {108, 0}, {109, 0}, {110, 0}, {111, 0}, {112, 0}, {113, 0}, {114, 0}, {115, 0}, {116, 0}, {117, 0}, {118, 0}, {119, 0}, {120, 0}, {121, 0}, {122, 0}, {123, 0}, {124, 0}, {125, 0}, {126, 0}, {127, 0}, {128, 0}, {129, 0}, {130, 0}, {131, 0}, {132, 0}, {133, 0}, {134, 0}, 
	{135, 0}, {136, 0}, {137, 0}, {138, 0}, {139, 0}, {140, 0}, {141, 0}, {142, 0}, {143, 0}, {144, 0}, {145, 0}, {146, 0}, {147, 0}, {148, 0}, {149, 0}, {150, 0},
	
	{15, 20}, {25,150}, {150,150}

}; 

// Status Variables
GLint   lvl = 1;
GLint   points = 0;
GLint   size  = 0;
GLbyte  gameOver = true;
GLbyte  EnableLight = true;

// Snake Variables
GLint   bodyPos[2][100] = {{}};
GLint   _x       = 5;
GLint   _z       = 10;
GLint   _oldX[2] = {};
GLint   _oldZ[2] = {};
GLbyte  direction  = 0;

// Food Variables
GLint   _bx      = 0;
GLint   _bz      = 0;

// Screen variables
GLint   _Giw     = 0;
GLint   _Gih     = 0;
GLint   _Gfw     = 150;
GLint   _Gfh     = 150;

//Variables for the Camera Angle
static GLfloat view_rotx=45.0F ;
static GLfloat view_roty=0.0F ;
static GLfloat view_rotz=0.0F ;

static GLfloat headRotation=90.0F ;
static GLfloat zoom=-300.0f;
//Configure the lightning
void initLight()
{
   //Add ambient light
    GLfloat ambientColor[] = {0.3f, 0.4f, 0.8f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    //Add positioned light
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    //Add directed light
    GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f};
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}
//initialize the first configurations
void Initialize(void)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.4f, 0.8f, 1.0f, 1.0f); //Change the background to sky blue
    if(EnableLight){
        glEnable(GL_COLOR_MATERIAL); //Enable color
        glEnable(GL_LIGHTING); //Enable lighting
        glEnable(GL_LIGHT0); //Enable light #0
        glEnable(GL_LIGHT1); //Enable light #1
        glEnable(GL_NORMALIZE); //Automatically normalize normals
    }
}
//Configure window resize
void resize (int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h,1, 800.0);
}
void Write(char *string){//Write string on the screen
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
}
//This Function will rotate the object according to the Angles
void ManipulateViewAngle() {
    glRotatef(view_rotx,1.0,0.0,0.0);//Rotate Arround X axis
    glRotatef(view_roty,0.0,1.0,0.0);//Rotate Arround Y axis
    glRotatef(view_rotz,0.0,0.0,1.0);//Rotate Arround Z axis
}
//This Function will reset the snake size and location...
void Reset(){
    _x       = 5;
    _z       = 10;
    direction  = 0;
    lvl      = 1;
    points = 0;
    size  = 0;
    gameOver = false;
    view_rotx=45.0F ;
    view_roty=0.0F ;
    view_rotz=0.0F ;
    headRotation=90.0F ;
}
//Display a welcome screen
void WelcomeScreen(){

    glRasterPos2f(-40, 20);
    Write("========= BEM-VINDO =========");
    glRasterPos2f(-40, 10);
    Write("========= Este é o jogo Snake-3D em OpenGL =========");
    glRasterPos2f(-40, 0);
    Write("========= Para iniciar o jogo aperte a tecla espaco =========");
}
void DrawSnake(){
    int  i;
    //Drawing the head & the plane
    glPushMatrix();
    ManipulateViewAngle();
        //This will draw the plane that the snake will run on.
        glPushMatrix();
        //glColor3f(1, 0.7, 0.2);
        glColor3f(0.0, 0.6, 0.2);
        glTranslatef(75.0, -16.0, 75.0);
        glScalef(155,5.0,155);
        glutSolidCube(1);
        glPopMatrix();
    //Here we will draw the Head of the snake
    glColor3f(1,0,0);//Color it red
    glTranslatef(_x,-10.0,_z);//Give it the location according to _x & _z
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(10,20,20);//Draw the head as a sphere a litle bit bigger than the body spheres
    glRotatef(headRotation, 0.0, 1.0, 0.0);
    glColor3f(1,0,0);
    glTranslatef(0,0.0,6.0);
    glScalef(0.8,1.0,1.0);
    glutSolidCone(10,10,20,20);
    glColor3f(1,1,1);
    glTranslatef(-4.0,10.0,0.0);
    glScalef(0.3,0.3,0.3);
    glutSolidSphere(10,20,20);
    glTranslatef(26.0,0.0,0.0);
    glutSolidSphere(10,20,20);
    glPopMatrix();

    //Drawing the body
    for(i=0; i<size; i++){//Loop throw the size and draw spheres representing the body
        glPushMatrix();
        ManipulateViewAngle();
        glTranslatef(bodyPos[0][i],-10.0,bodyPos[1][i]);//this will locate the spheres
        glColor3f(1,0,0);//Color Red
        glScalef(0.5,0.5,0.5);
        glutSolidSphere(7,20,20);
        glPopMatrix();
    }
}
void DrawFood()
{
    //Draw the Sphere representing the Food for the snake
    glPushMatrix();
    ManipulateViewAngle();
    glTranslatef(_bx,-10.0,_bz);
    glColor3f(1, 1, 1);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(7,20,20);
    glPopMatrix();
}
void GameStatus(){
    char tmp_str[40];
    glColor3f(0.8, 0.2, 0);
    glRasterPos2f(5, 20);
    sprintf(tmp_str, "Snake ");
    Write(tmp_str);
    // Print the status of the game on the screen
    glColor3f(0, 0, 0);
    glRasterPos2f(5, 10);
    sprintf(tmp_str, "Level: %d Points: %d", lvl, points);
    Write(tmp_str);
}
//Generates Random Numbers for the location of the food that the snake will eat
int RandomNumber(int high, int low)
{
    return (rand() % (high-low))+low;
}
//Generate the New Food that the snake will eat
void newFood(){
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    _bx = RandomNumber(_Gfw-_Giw, _Giw+10);
    _bz = RandomNumber(_Gfh-_Gih, _Gih+10);
}
//This Function Will Check for Collision
bool collision(){
    int i;
    for(i=0; i<size; i++){
        if((bodyPos[0][i] == _x && bodyPos[1][i] == _z) ||
        //((bodyPos[0][i] >= _x) && (bodyPos[0][i] <= _x + 5) && (bodyPos[1][i] >= _z) && (bodyPos[1][i] <= _z + 5)) ||
        //((bodyPos[0][i] <= _x) && (bodyPos[0][i] >= _x - 5) && (bodyPos[1][i] <= _z) && (bodyPos[1][i] >= _z - 5)) ||
        //((bodyPos[0][i] <= _x) && (bodyPos[0][i] >= _x - 5) && (bodyPos[1][i] >= _z) && (bodyPos[1][i] <= _z + 5)) ||
        ((bodyPos[0][i] >= _x) && (bodyPos[0][i] <= _x + 5) && (bodyPos[1][i] <= _z) && (bodyPos[1][i] >= _z - 5)))
        return true;
    }
   return false;
}
//This Function will move the snake according to the directions
//Taken from the Keyboard keys
void Run(int value){
    int i;
    _oldX[1] = _x;
    _oldZ[1] = _z;
    switch(direction){
      case RIGHT:
          headRotation =90;
          _x += 6;
          if(_x > _Gfw-2) _x = _Giw-1;//This will check if the snake is going into the border so it will appear on the other side
          break;
      case LEFT:
          headRotation =-90;
          _x -= 6;
         if(_x < 0) _x = _Gfw-2;//This will check if the snake is going into the border so it will appear on the other side
          break;
      case UP:
          headRotation =0;
          _z += 6;
          if(_z > _Gfh-2) _z = _Gih-1;//This will check if the snake is going into the border so it will appear on the other side
          break;
      case Down:
          headRotation =180;
          _z -= 6;
          if(_z < 2) _z = _Gfh-2;//This will check if the snake is going into the border so it will appear on the other side
          break;
    }
    //Checks for Collisoin if yes Game Over
    if(collision()) gameOver = true;
    //Checks if the snake ate the food (check the X and Y)
    // If yes it will increase the points & the size of the snake & create a new food
    if((_x == _bx && _z == _bz) ||
    ((_x >= _bx) && (_x <= _bx + 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
    ((_x <= _bx) && (_x >= _bx - 4) && (_z <= _bz) && (_z >= _bz - 4)) ||
    ((_x <= _bx) && (_x >= _bx - 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
    ((_x >= _bx) && (_x <= _bx + 4) && (_z <= _bz) && (_z >= _bz - 4))){
        points++;
        if(points < 100) size++;
        if(points%5 == 0 && lvl < 15) lvl++;
        newFood();
    }

    for(i = 0; i<size; i++){// Save the positions of the body parts
        _oldX[0]       = _oldX[1];
        _oldZ[0]       = _oldZ[1];
        _oldX[1]       = bodyPos[0][i];
        _oldZ[1]       = bodyPos[1][i];
        bodyPos[0][i] = _oldX[0];
        bodyPos[1][i] = _oldZ[0];
    }

    //Set the Timer
    glutTimerFunc(130, Run, 0);
}
// Adicione esta função para desenhar as paredes
void DrawWalls() {
    for (int i = 0; i < sizeof(wallPositions) / sizeof(wallPositions[0]); ++i) {
        glPushMatrix();
        ManipulateViewAngle();
        glTranslatef(wallPositions[i][0], -10.0, wallPositions[i][1]);
        glColor3f(0.2, 0.2, 0.2);  // Cor da parede
        glScalef(0.5, 0.5, 0.5);    // Ajuste conforme necessário
        glutSolidCube(7);          // Use glutSolidCube ou outro objeto para representar a parede
        glPopMatrix();
    }
}

void Display(void){//Draw Function
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(EnableLight) initLight();

    glTranslatef (-60.0, 40.0, zoom);
    //Check if the value of the Flag "Game Over is not True it will continue the game"
    if(!gameOver){
        GameStatus();
        DrawFood();
        DrawSnake();
        DrawWalls();
    }
    else
        WelcomeScreen();
    // Updates the screen
    glutPostRedisplay();
    glutSwapBuffers();
}
void Special(int key, int x, int y){
    switch(key){
    case GLUT_KEY_RIGHT     :
        if(direction != LEFT)
            direction = RIGHT;
        break;
    case GLUT_KEY_LEFT      :
        if(direction != RIGHT)
            direction = LEFT;
        break;
    case GLUT_KEY_UP        :
        if(direction != UP)
            direction = Down;
        break;
    case GLUT_KEY_DOWN      :
        if(direction != Down)
            direction = UP;
        break;
    }
}
void keyboard(unsigned char key, int x, int y){ 
    switch (key) {
    case ' ':
        Reset();
        glutPostRedisplay();
        break;
	case 'W':
    case 'w':
        if (direction != UP)
            direction = Down;
        break;
    case 'S':
    case 's':
        if (direction != Down)
            direction = UP;
        break;
    case 'A':
    case 'a':
        if (direction != RIGHT)
            direction = LEFT;
        break;
    case 'D':
    case 'd':
        if (direction != LEFT)
            direction = RIGHT;
        break;
    case 27:
        exit(0);// ESC para sair
        break;
    default:
        break;
    }
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280,720);
    glutInitWindowPosition(10,10);
    glutCreateWindow("SNAKE 3D - OpenGL");//Nome do jogo
    Initialize();
    glutSpecialFunc(Special);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(Display);
    glutReshapeFunc(resize);
    newFood();
    Run(0);
    glutMainLoop();
}

