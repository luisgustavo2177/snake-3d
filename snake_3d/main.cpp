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

// Defina um par para armazenar as coordenadas x e z
typedef std::pair<int, int> Position;

// Crie um vetor para armazenar as posições das paredes
std::vector<Position> wall_positions;

// Função para adicionar uma parede
void add_wall(int x, int z) {
    wall_positions.push_back({x, z});
}

// Função para inicializar as paredes
void initialize_walls() {
    // Esquerda
    for (int i = 0; i <= 150; ++i) {
        if (i >= 65 && i <= 85) continue; // Buraco
        add_wall(i, 0);
    }

    // Direita
    for (int i = 0; i <= 150; ++i) {
        if (i >= 65 && i <= 85) continue;
        add_wall(i, 150);
    }
 
    // Cima
    for (int i = 0; i <= 150; ++i) {
        if (i >= 65 && i <= 85) continue;
        add_wall(0, i);
    }

    // Baixo
    for (int i = 0; i <= 150; ++i) {
        if (i >= 65 && i <= 85) continue;
        add_wall(150, i);
    }
}

// Variáveis de controle
GLint lvl = 1;
GLint points = 0;
GLint size = 0;
GLbyte game_over = true;
GLbyte enable_light = true;

// Variáveis da cobra
GLint body_pos[2][100] = {{}};
GLint _x = 10;
GLint _z = 15;
GLint _oldX[2] = {};
GLint _oldZ[2] = {};
GLbyte direction = 0;

// Variáveis da comida
GLint _bx = 0;
GLint _bz = 0;

// Variáveis da janela
GLint _iw = 0;
GLint _ih = 0;
GLint _fw = 150;
GLint _fh = 150;

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
    initialize_walls(); // Inicializa as paredes antes de iniciar o jogo
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
    _x = 10;
    _z = 15;
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
    glScalef(0.4, 0.4, 0.4);
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
        _bx = random_number(_fw - _iw, _iw + 10);
        _bz = random_number(_fh - _ih, _ih + 10);

        // Checa se a comida não está em cima de alguma parede
        for (size_t i = 0; i < wall_positions.size(); i++) {
            if (wall_positions[i].first == _bx && wall_positions[i].second == _bz) {
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
        ((body_pos[0][i] >= _x) && (body_pos[0][i] <= _x + 5) && (body_pos[1][i] >= _z) && (body_pos[1][i] <= _z + 5)) ||
        ((body_pos[0][i] <= _x) && (body_pos[0][i] >= _x - 5) && (body_pos[1][i] <= _z) && (body_pos[1][i] >= _z - 5)) ||
        ((body_pos[0][i] <= _x) && (body_pos[0][i] >= _x - 5) && (body_pos[1][i] >= _z) && (body_pos[1][i] <= _z + 5)) ||
        ((body_pos[0][i] >= _x) && (body_pos[0][i] <= _x + 5) && (body_pos[1][i] <= _z) && (body_pos[1][i] >= _z - 5)))
        return true;
    }

    // Checa se a cobra colidiu com alguma parede
    for (size_t i = 0; i < wall_positions.size(); i++) {
        if ((wall_positions[i].first == _x && wall_positions[i].second == _z) ||
            ((wall_positions[i].first >= _x) && (wall_positions[i].first <= _x + 5) && (wall_positions[i].second >= _z) && (wall_positions[i].second <= _z + 5)) ||
            ((wall_positions[i].first >= _x) && (wall_positions[i].first <= _x + 5) && (wall_positions[i].second <= _z) && (wall_positions[i].second >= _z - 5)))
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
          if (_x > _fw-2) _x = _iw-1;
          break;
      case LEFT:
          head_rotation = -90;
          _x -= 6;
         if (_x < 0) _x = _fw-2;
          break;
      case UP:
          head_rotation =0;
          _z += 6;
          if (_z > _fh-2) _z = _ih-1;
          break;
      case DOWN:
          head_rotation = 180;
          _z -= 6;
          if (_z < 2) _z = _fh - 2;
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
    for (size_t i = 0; i < wall_positions.size(); ++i) {
        glPushMatrix();
        manipulate_view_angle();
        glTranslatef(wall_positions[i].first, -10.0, wall_positions[i].second);
        glColor3f(0.2, 0.2, 0.2);
        glScalef(0.5, 0.5, 0.5);
        glutSolidCube(7);
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
        if (direction != LEFT)
            direction = RIGHT;
        break;
    case GLUT_KEY_LEFT:
        if (direction != RIGHT)
            direction = LEFT;
        break;
    case GLUT_KEY_UP:
        if (direction != UP)
            direction = DOWN;
        break;
    case GLUT_KEY_DOWN:
        if (direction != DOWN)
            direction = UP;
        break;
    }
}

// Função para capturar as teclas do teclado
void keyboard(unsigned char key, int x, int y) { 
    switch (key) {
    case SPACE: // Iniciar/Reiniciar o jogo
        reset();
        glutPostRedisplay();
        break;
	case 'W': case 'w':
        if (direction != UP) direction = DOWN;
        break;
    case 'S': case 's':
        if (direction != DOWN) direction = UP;
        break;
    case 'A': case 'a':
        if (direction != RIGHT) direction = LEFT;
        break;
    case 'D': case 'd':
        if (direction != LEFT) direction = RIGHT;
        break;
    case ESC: // Sair do jogo
        exit(0);
        break;
    default:
        break;
    }
}
