/*************************************************************** 
*  Universidade Federal do Piaui - UFPI                        *
*  Disciplina: Computação Gráfica                              *
*  Trabalho Final: Snake Game 3D em OPENGL                     *
*  Alunos: Heitor Andrade Moura e Luis Gustavo da Silva Sousa  *
*  Data: 04/02/2024                                            *
****************************************************************/

// Bibliotecas OpenGL
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

// Bibliotecas padrão
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <vector>

// Definição de constantes
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ESC 27
#define SPACE 32

// Matriz de posições das paredes
GLint wall_positions[][2] = {
    // Superior
	{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}, {17, 0}, {18, 0}, {19, 0}, {20, 0}, {21, 0}, {22, 0}, {23, 0}, {24, 0}, {25, 0}, {26, 0}, {27, 0}, {28, 0}, {29, 0}, {30, 0}, {31, 0}, {32, 0}, {33, 0}, {34, 0}, {35, 0}, {36, 0}, {37, 0}, {38, 0}, {39, 0}, {40, 0}, {41, 0}, {42, 0}, {43, 0}, {44, 0}, {45, 0}, {46, 0}, {47, 0}, {48, 0}, {49, 0}, {50, 0}, {51, 0}, {52, 0}, {53, 0}, {54, 0}, {55, 0}, {56, 0}, {57, 0}, {58, 0}, {59, 0}, {60, 0}, {61, 0}, {62, 0}, {63, 0}, {64, 0}, {87, 0}, {88, 0}, {89, 0},
    {90, 0}, {91, 0}, {92, 0}, {93, 0}, {94, 0}, {95, 0}, {96, 0}, {97, 0}, {98, 0}, {99, 0}, {100, 0}, {101, 0}, {102, 0}, {103, 0}, {104, 0}, {105, 0}, {106, 0}, {107, 0}, {108, 0}, {109, 0}, {110, 0}, {111, 0}, {112, 0}, {113, 0}, {114, 0}, {115, 0}, {116, 0}, {117, 0}, {118, 0}, {119, 0}, {120, 0}, {121, 0}, {122, 0}, {123, 0}, {124, 0}, {125, 0}, {126, 0}, {127, 0}, {128, 0}, {129, 0}, {130, 0}, {131, 0}, {132, 0}, {133, 0}, {134, 0}, {135, 0}, {136, 0}, {137, 0}, {138, 0}, {139, 0}, {140, 0}, {141, 0}, {142, 0}, {143, 0}, {144, 0}, {145, 0}, {146, 0}, {147, 0}, {148, 0}, {149, 0}, {150, 0},
	// Inferior
    {0, 150}, {1, 150}, {2, 150}, {3, 150}, {4, 150}, {5, 150}, {6, 150}, {7, 150}, {8, 150}, {9, 150}, {10, 150}, {11, 150}, {12, 150}, {13, 150}, {14, 150}, {15, 150}, {16, 150}, {17, 150}, {18, 150}, {19, 150}, {20, 150}, {21, 150}, {22, 150}, {23, 150}, {24, 150}, {25, 150}, {26, 150}, {27, 150}, {28, 150}, {29, 150}, {30, 150}, {31, 150}, {32, 150}, {33, 150}, {34, 150}, {35, 150}, {36, 150}, {37, 150}, {38, 150}, {39, 150}, {40, 150}, {41, 150}, {42, 150}, {43, 150}, {44, 150}, {45, 150}, {46, 150}, {47, 150}, {48, 150}, {49, 150}, {50, 150}, {51, 150}, {52, 150}, {53, 150}, {54, 150}, {55, 150}, {56, 150}, {57, 150}, {58, 150}, {59, 150}, {60, 150}, {61, 150}, {62, 150}, {63, 150}, {64, 150}, {87, 150}, {88, 150},
    {89, 150}, {90, 150}, {91, 150}, {92, 150}, {93, 150}, {94, 150}, {95, 150}, {96, 150}, {97, 150}, {98, 150}, {99, 150}, {100, 150}, {101, 150}, {102, 150}, {103, 150}, {104, 150}, {105, 150}, {106, 150}, {107, 150}, {108, 150}, {109, 150}, {110, 150}, {111, 150}, {112, 150}, {113, 150}, {114, 150}, {115, 150}, {116, 150}, {117, 150}, {118, 150}, {119, 150}, {120, 150}, {121, 150}, {122, 150}, {123, 150}, {124, 150}, {125, 150}, {126, 150}, {127, 150}, {128, 150}, {129, 150}, {130, 150}, {131, 150}, {132, 150}, {133, 150}, {134, 150}, {135, 150}, {136, 150}, {137, 150}, {138, 150}, {139, 150}, {140, 150}, {141, 150}, {142, 150}, {143, 150}, {144, 150}, {145, 150}, {146, 150}, {147, 150}, {148, 150}, {149, 150}, {150, 150},
    // Esquerdo
    {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15}, {0, 16}, {0, 17}, {0, 18}, {0, 19}, {0, 20}, {0, 21}, {0, 22}, {0, 23}, {0, 24}, {0, 25}, {0, 26}, {0, 27}, {0, 28}, {0, 29}, {0, 30}, {0, 31}, {0, 32}, {0, 33}, {0, 34}, {0, 35}, {0, 36}, {0, 37}, {0, 38}, {0, 39}, {0, 40}, {0, 41}, {0, 42}, {0, 43}, {0, 44}, {0, 45}, {0, 46}, {0, 47}, {0, 48}, {0, 49}, {0, 50}, {0, 51}, {0, 52}, {0, 53}, {0, 54}, {0, 55}, {0, 56}, {0, 57}, {0, 58}, {0, 59}, {0, 60}, {0, 61}, {0, 62}, {0, 63}, {0, 64}, {0, 65}, {0, 87}, {0, 88}, {0, 89},
    {0, 90}, {0, 91}, {0, 92}, {0, 93}, {0, 94}, {0, 95}, {0, 96}, {0, 97}, {0, 98}, {0, 99}, {0, 100}, {0, 101}, {0, 102}, {0, 103}, {0, 104}, {0, 105}, {0, 106}, {0, 107}, {0, 108}, {0, 109}, {0, 110}, {0, 111}, {0, 112}, {0, 113}, {0, 114}, {0, 115}, {0, 116}, {0, 117}, {0, 118}, {0, 119}, {0, 120}, {0, 121}, {0, 122}, {0, 123}, {0, 124}, {0, 125}, {0, 126}, {0, 127}, {0, 128}, {0, 129}, {0, 130}, {0, 131}, {0, 132}, {0, 133}, {0, 134}, {0, 135}, {0, 136}, {0, 137}, {0, 138}, {0, 139}, {0, 140}, {0, 141}, {0, 142}, {0, 143}, {0, 144}, {0, 145}, {0, 146}, {0, 147}, {0, 148}, {0, 149}, {0, 150},
    // Direito
    {150, 1}, {150, 2}, {150, 3}, {150, 4}, {150, 5}, {150, 6}, {150, 7}, {150, 8}, {150, 9}, {150, 10}, {150, 11}, {150, 12}, {150, 13}, {150, 14}, {150, 15}, {150, 16}, {150, 17}, {150, 18}, {150, 19}, {150, 20}, {150, 21}, {150, 22}, {150, 23}, {150, 24}, {150, 25}, {150, 26}, {150, 27}, {150, 28}, {150, 29}, {150, 30}, {150, 31}, {150, 32}, {150, 33}, {150, 34}, {150, 35}, {150, 36}, {150, 37}, {150, 38}, {150, 39}, {150, 40}, {150, 41}, {150, 42}, {150, 43}, {150, 44}, {150, 45}, {150, 46}, {150, 47}, {150, 48}, {150, 49}, {150, 50}, {150, 51}, {150, 52}, {150, 53}, {150, 54}, {150, 55}, {150, 56}, {150, 57}, {150, 58}, {150, 59}, {150, 60}, {150, 61}, {150, 62}, {150, 63}, {150, 64}, {150, 87}, {150, 88}, {150, 89},
    {150, 90}, {150, 91}, {150, 92}, {150, 93}, {150, 94}, {150, 95}, {150, 96}, {150, 97}, {150, 98}, {150, 99}, {150, 100}, {150, 101}, {150, 102}, {150, 103}, {150, 104}, {150, 105}, {150, 106}, {150, 107}, {150, 108}, {150, 109}, {150, 110}, {150, 111}, {150, 112}, {150, 113}, {150, 114}, {150, 115}, {150, 116}, {150, 117}, {150, 118}, {150, 119}, {150, 120}, {150, 121}, {150, 122}, {150, 123}, {150, 124}, {150, 125}, {150, 126}, {150, 127}, {150, 128}, {150, 129}, {150, 130}, {150, 131}, {150, 132}, {150, 133}, {150, 134}, {150, 135}, {150, 136}, {150, 137}, {150, 138}, {150, 139}, {150, 140}, {150, 141}, {150, 142}, {150, 143}, {150, 144}, {150, 145}, {150, 146}, {150, 147}, {150, 148}, {150, 149}, {150, 150}
};

// Variáveis de controle
GLint lvl = 1;
GLint points = 0;
GLint size  = 0;
GLbyte game_over = true;
GLbyte enable_light = true;

// Variáveis da cobra
GLint body_pos[2][100] = {{}};
GLint _x = 5;
GLint _z = 10;
GLint _oldX[2] = {};
GLint _oldZ[2] = {};
GLbyte direction = 0;

// Variáveis da comida
GLint _bx = 0;
GLint _bz = 0;

// Variáveis da janela
GLint _Giw = 0;
GLint _Gih = 0;
GLint _Gfw = 150;
GLint _Gfh = 150;

// Variáveis para o ângulo da câmera
static GLfloat view_rotx = 45.0F ;
static GLfloat view_roty = 0.0F ;
static GLfloat view_rotz = 0.0F ;

static GLfloat head_rotation = 90.0F ;
static GLfloat zoom = -300.0f;

// Declaração de funções
void init(void);
void resize (int w, int h);
void display(void);
void special(int key, int x, int y);
void keyboard (unsigned char key, int x, int y);
void run(int value);
void new_food();

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,550);
    glutInitWindowPosition(80,80);
    glutCreateWindow("Snake Game 3D");
    init();
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    new_food();
    run(0);
    glutMainLoop();
}

// Configura a iluminação
void init_light() {
    // Adiciona luz ambiente
    GLfloat ambient_color[] = {0.3f, 0.4f, 0.8f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_color);

    // Adiciona luz posicionada
    GLfloat light_color0[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat light_pos0[] = {4.0f, 0.0f, 8.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

    // Adiciona luz direcional
    GLfloat light_color1[] = {0.5f, 0.2f, 0.2f, 1.0f};
    GLfloat light_pos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
}

// Configurações iniciais
void init(void) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.4f, 0.8f, 1.0f, 1.0f); // Muda a cor de fundo para azul céu
    
    if (enable_light) {
        glEnable(GL_COLOR_MATERIAL); // Habilita cor
        glEnable(GL_LIGHTING); // Habilita luz
        glEnable(GL_LIGHT0); // Habilita luz #0
        glEnable(GL_LIGHT1); // Habilita luz #1
        glEnable(GL_NORMALIZE); // Normaliza vetores
    }
}

// Configura o redimensionamento da janela
void resize (int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1, 800.0);
}

// Função para escrever na tela
void write(char *string) {
    while(*string) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
    }
}

// Função para manipular o ângulo da câmera
void manipulate_view_angle() {
    glRotatef(view_rotx, 1.0, 0.0, 0.0); // Rotaciona em torno do eixo X
    glRotatef(view_roty, 0.0, 1.0, 0.0); // Rotaciona em torno do eixo Y
    glRotatef(view_rotz, 0.0, 0.0, 1.0); // Rotaciona em torno do eixo Z
}

// Função para resetar o jogo
void reset() {
    _x = 5;
    _z = 10;
    direction = 0;
    lvl = 1;
    points = 0;
    size = 0;
    game_over = false;
    view_rotx = 45.0F ;
    view_roty = 0.0F ;
    view_rotz = 0.0F ;
    head_rotation = 90.0F ;
}

// Função para desenhar a tela de boas-vindas
void welcome_screen() {
    glRasterPos2f(-40, 20);
    write("========= BEM-VINDO =========");
    glRasterPos2f(-40, 10);
    write("========= Este é o jogo Snake-3D em OpenGL =========");
    glRasterPos2f(-40, 0);
    write("========= Para iniciar o jogo aperte a tecla espaco =========");
}

// Função para desenhar a cobra
void draw_snake() {
    int  i;

    // Desenha a cabeça da cobra e o plano
    glPushMatrix();
    manipulate_view_angle();
        // Desenha o plano que a cobra vai correr
        glPushMatrix();
        //glColor3f(1, 0.7, 0.2);
        glColor3f(0.0, 0.6, 0.2);
        glTranslatef(75.0, -16.0, 75.0);
        glScalef(155, 5.0, 155);
        glutSolidCube(1);
        glPopMatrix();

    // Desenha a cabeça da cobra
    glColor3f(1, 0, 0); // Cor da cabeça da cobra (vermelho)
    glTranslatef(_x, -10.0, _z);// Pega a posição da cabeça da cobra de acordo com _x e _z
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(10, 20, 20); // Desenha a cabeça como uma esfera um pouco maior que as esferas do corpo
    glRotatef(head_rotation, 0.0, 1.0, 0.0);
    glColor3f(1, 0, 0);
    glTranslatef(0, 0.0, 6.0);
    glScalef(0.8, 1.0, 1.0);
    glutSolidCone(10, 10, 20, 20);
    glColor3f(1, 1, 1);
    glTranslatef(-4.0, 10.0, 0.0);
    glScalef(0.3, 0.3, 0.3);
    glutSolidSphere(10, 20, 20);
    glTranslatef(26.0, 0.0, 0.0);
    glutSolidSphere(10, 20, 20);
    glPopMatrix();

    // Desenha o corpo da cobra
    for (i = 0; i < size; i++) { // Loop através do tamanho e desenha esferas representando o corpo
        glPushMatrix();
        manipulate_view_angle();
        glTranslatef(body_pos[0][i], -10.0, body_pos[1][i]); // Localização do corpo da cobra
        glColor3f(1,0,0); // Cor do corpo da cobra (vermelho)
        glScalef(0.5,0.5,0.5);
        glutSolidSphere(7,20,20);
        glPopMatrix();
    }
}

// Função para desenhar a comida
void draw_food() {
    // Desenha a esfera representando a comida para a cobra
    glPushMatrix();
    manipulate_view_angle();
    glTranslatef(_bx, -10.0, _bz);
    glColor3f(1, 1, 1);
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(7, 20, 20);
    glPopMatrix();
}

// Função para desenhar o status do jogo
void game_status() {
    char tmp_str[40];
    glColor3f(0.8, 0.2, 0);
    glRasterPos2f(5, 20);
    sprintf(tmp_str, "Snake ");
    write(tmp_str);
    // Escreve o status do jogo na tela
    glColor3f(0, 0, 0);
    glRasterPos2f(5, 10);
    sprintf(tmp_str, "Level: %d Points: %d", lvl, points);
    write(tmp_str);
}

// Gerador de números aleatórios para a localização da comida que a cobra vai comer
int random_number(int high, int low) {
    return (rand() % (high-low)) + low;
}

// Gerador de novas posições para a comida
void new_food() {
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);

    bool food_on_wall;
    do {
        food_on_wall = false;
        _bx = random_number(_Gfw - _Giw, _Giw + 10);
        _bz = random_number(_Gfh - _Gih, _Gih + 10);

        // Checa se a comida não está em cima de alguma parede
        for (int i = 0; i < sizeof(wall_positions) / sizeof(wall_positions[0]); i++) {
            if (wall_positions[i][0] == _bx && wall_positions[i][1] == _bz) {
                food_on_wall = true;
                break;
            }
        }
    } while (food_on_wall);
}

// Função para checar colisões
bool collision() {
    int i;

    // Checa se a cobra colidiu com alguma parte do corpo
    for (i = 0; i < size; i++) {
        if ((body_pos[0][i] == _x && body_pos[1][i] == _z) ||
        // ((body_pos[0][i] >= _x) && (body_pos[0][i] <= _x + 5) && (body_pos[1][i] >= _z) && (body_pos[1][i] <= _z + 5)) ||
        // ((body_pos[0][i] <= _x) && (body_pos[0][i] >= _x - 5) && (body_pos[1][i] <= _z) && (body_pos[1][i] >= _z - 5)) ||
        // ((body_pos[0][i] <= _x) && (body_pos[0][i] >= _x - 5) && (body_pos[1][i] >= _z) && (body_pos[1][i] <= _z + 5)) ||
        ((body_pos[0][i] >= _x) && (body_pos[0][i] <= _x + 5) && (body_pos[1][i] <= _z) && (body_pos[1][i] >= _z - 5)))
        return true;
    }

    // Checa se a cobra colidiu com alguma parede
    for (i = 0; i < sizeof(wall_positions) / sizeof(wall_positions[0]); i++) {
        if ((wall_positions[i][0] == _x && wall_positions[i][1] == _z) ||
        ((wall_positions[i][0] >= _x) && (wall_positions[i][0] <= _x + 5) && (wall_positions[i][1] >= _z) && (wall_positions[i][1] <= _z + 5)) ||
        // ((wall_positions[i][0] <= _x) && (wall_positions[i][0] >= _x - 5) && (wall_positions[i][1] <= _z) && (wall_positions[i][1] >= _z - 5)) ||
        // ((wall_positions[i][0] <= _x) && (wall_positions[i][0] >= _x - 5) && (wall_positions[i][1] >= _z) && (wall_positions[i][1] <= _z + 5)) ||
        ((wall_positions[i][0] >= _x) && (wall_positions[i][0] <= _x + 5) && (wall_positions[i][1] <= _z) && (wall_positions[i][1] >= _z - 5)))
        return true;
    }

    return false;
}

// Função para movimentar a cobra
void run(int value) {
    int i;
    _oldX[1] = _x;
    _oldZ[1] = _z;

    switch (direction) {
      case RIGHT:
          head_rotation = 90;
          _x += 6;
          // Checa se a cobra está indo para fora da tela, se sim, ela aparece do outro lado
          if (_x > _Gfw-2) _x = _Giw-1;
          break;
      case LEFT:
          head_rotation = -90;
          _x -= 6;
         if (_x < 0) _x = _Gfw-2;
          break;
      case UP:
          head_rotation =0;
          _z += 6;
          if (_z > _Gfh-2) _z = _Gih-1;
          break;
      case DOWN:
          head_rotation = 180;
          _z -= 6;
          if (_z < 2) _z = _Gfh - 2;
          break;
    }

    // Checa se a cobra colidiu com alguma parede, se sim, o jogo acaba
    if (collision()) game_over = true;

    // Checa se a cobra comeu a comida, se sim, aumenta os pontos e o tamanho da cobra e cria uma nova comida
    if ((_x == _bx && _z == _bz) ||
    ((_x >= _bx) && (_x <= _bx + 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
    ((_x <= _bx) && (_x >= _bx - 4) && (_z <= _bz) && (_z >= _bz - 4)) ||
    ((_x <= _bx) && (_x >= _bx - 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
    ((_x >= _bx) && (_x <= _bx + 4) && (_z <= _bz) && (_z >= _bz - 4))) {
        points++;
        if (points < 100) size++;
        if ((points % 5) == 0 && lvl < 15) lvl++;
        new_food();
    }

    for (i = 0; i < size; i++) { // Salva as posições das partes do corpo
        _oldX[0] = _oldX[1];
        _oldZ[0] = _oldZ[1];
        _oldX[1] = body_pos[0][i];
        _oldZ[1] = body_pos[1][i];
        body_pos[0][i] = _oldX[0];
        body_pos[1][i] = _oldZ[0];
    }

    // Configura o timer para a velocidade da cobra
    glutTimerFunc(130, run, 0);
}

// Adicione esta função para desenhar as paredes
void draw_walls() {
    for (int i = 0; i < sizeof(wall_positions) / sizeof(wall_positions[0]); ++i) {
        glPushMatrix();
        manipulate_view_angle();
        glTranslatef(wall_positions[i][0], -10.0, wall_positions[i][1]);
        glColor3f(0.2, 0.2, 0.2); // Cor da parede
        glScalef(0.5, 0.5, 0.5);  // Ajuste conforme necessário
        glutSolidCube(7);         // Use glutSolidCube ou outro objeto para representar a parede
        glPopMatrix();
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (enable_light) init_light();

    glTranslatef (-60.0, 40.0, zoom);

    // Checa se o valor da flag "Game Over" não é verdadeiro, se for, continua o jogo
    if (!game_over) {
        game_status();
        draw_food();
        draw_snake();
        draw_walls();
    } else {
        welcome_screen();
    }

    glutPostRedisplay();
    glutSwapBuffers();
}

// Função para capturar as teclas especiais
void special(int key, int x, int y) {
    switch(key) {
    case GLUT_KEY_RIGHT:
        if(direction != LEFT)
            direction = RIGHT;
        break;
    case GLUT_KEY_LEFT:
        if(direction != RIGHT)
            direction = LEFT;
        break;
    case GLUT_KEY_UP:
        if(direction != UP)
            direction = DOWN;
        break;
    case GLUT_KEY_DOWN:
        if(direction != DOWN)
            direction = UP;
        break;
    }
}

// Função para capturar as teclas do teclado
void keyboard(unsigned char key, int x, int y) { 
    switch (key) {
    case SPACE:
        reset();
        glutPostRedisplay();
        break;
	case 'W':
    case 'w':
        if (direction != UP)
            direction = DOWN;
        break;
    case 'S':
    case 's':
        if (direction != DOWN)
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
    case ESC:
        exit(0); // ESC para sair
        break;
    default:
        break;
    }
}
