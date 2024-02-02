/*************************************************************** 
*  Universidade Federal do Piaui - UFPI                        *
*  Disciplina: Computa��o Gr�fica                              *
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

// Bibliotecas padr�o
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <vector>

//Biblioteca para uso de som
#include <mmsystem.h>

//Biblioteca para cria��o das telas 
#include "glut_text.h"

// Configurar o som
typedef struct {
    const char* nome_arquivo;  // nome do arquivo de som
    const char* ext;         // extens�o(wav, mp4, mp3...)
} music;

// music sons_de_fundo = {"./music_de_fundo/classicMusicSnakeGame.wav", "wav"};
// music sons_de_fundo = {"./music_de_fundo/Written-In-The-Stars-ft.wav", "wav"};
music sons_de_fundo = {"./music_de_fundo/1.wav", "wav"};

void playSound(){
    PlaySound(sons_de_fundo.nome_arquivo, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void loadSound(){
    PlaySound(sons_de_fundo.nome_arquivo, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NOSTOP);
    PlaySound(NULL, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP | SND_NOSTOP | SND_PURGE);
    PlaySound(sons_de_fundo.nome_arquivo, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NOSTOP | SND_NOWAIT);
}

//Configura as texturas
typedef struct {////////////////////////////////////////////////////////////
    int width;
    int height;
    int channels;
    unsigned char* data;
} Image;

Image* load_image(const char* filename);
void free_image(Image* image);

// Armazena o ID da textura gerada pelo OpenGL
GLuint textureID;////////////////////////////////////////////////////////////
GLuint textureID1;
GLuint textureID2;

void lerImagem(const char* filename, int textura) {//////////////////////
    Image* image = load_image(filename);

    if (image) {
    	if(textura == 1){
			glGenTextures(1, &textureID);
        	glBindTexture(GL_TEXTURE_2D, textureID);	
		}else if(textura == 2){
			glGenTextures(1, &textureID1);
        	glBindTexture(GL_TEXTURE_2D, textureID1);
		}else{
			glGenTextures(1, &textureID2);
        	glBindTexture(GL_TEXTURE_2D, textureID2);
		}

        // Configura os par�metros da textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Carrega os dados da imagem na textura
        GLenum format = (image->channels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);

        // Gera mipmaps para a textura
        gluBuild2DMipmaps(GL_TEXTURE_2D, format, image->width, image->height, format, GL_UNSIGNED_BYTE, image->data);

        // Desvincula a textura
        glBindTexture(GL_TEXTURE_2D, 0);

        // Libera a mem�ria da imagem
        free_image(image);
    }
}

// Defini��o de constantes
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ESC 27
#define SPACE 32

// Defina um par para armazenar as coordenadas x e z
typedef std::pair<int, int> Position;

// Crie um vetor para armazenar as posi��es das paredes
std::vector<Position> wall_positions;

// Fun��o para adicionar uma parede
void add_wall(int x, int z) {
    wall_positions.push_back({x, z});
}

// Fun��o para inicializar as paredes
void initialize_walls() {
    // Superior
    for (int i = 0; i <= 150; ++i) {
        if (i >= 65 && i <= 87) continue; // Buraco
        add_wall(i, 0);
    }

    // Inferior
    for (int i = 0; i <= 150; ++i) {
        if (i >= 65 && i <= 87) continue;
        add_wall(i, 150);
    }
 
    // Esquerda
    for (int i = 0; i <= 150; ++i) {
        if (i >= 65 && i <= 87) continue;
        add_wall(0, i);
    }

    // Direita
    for (int i = 0; i <= 150; ++i) {
        if (i >= 65 && i <= 87) continue;
        add_wall(150, i);
    }
}

// Vari�veis de controle
GLint lvl = 1;
GLint points = 0;
GLint size = 0;
GLbyte game_over = true;
GLbyte enable_light = true;
GLbyte paused = false;

// Vari�veis da cobra
GLint body_pos[2][100] = {{}};
GLint _x = 10;
GLint _z = 15;
GLint _oldX[2] = {};
GLint _oldZ[2] = {};
GLbyte direction = 0;

// Vari�veis da comida
GLint _bx = 0;
GLint _bz = 0;

// Vari�veis da janela
GLint _iw = 0;
GLint _ih = 0;
GLint _fw = 150;
GLint _fh = 150;

// Vari�veis para o �ngulo da c�mera
static GLfloat view_rotx = 45.0F ;
static GLfloat view_roty = 0.0F ;
static GLfloat view_rotz = 0.0F ;

static GLfloat head_rotation = 90.0F ;
static GLfloat zoom = -300.0f;

// Declara��o de fun��es
void init(void);
void resize (int w, int h);
void display(void);
void special(int key, int x, int y);
void keyboard (unsigned char key, int x, int y);
void run(int value);
void new_food();

// Fun��o para desenhar a tela de boas-vindas
bool game_started = false;//Verifica se deve mostrar a tela de Bem-vindo ou Game Over
void welcome_screen_game_over_screen() {
	if(!game_started){
	    glColor3f(0.0, 0.0, 0.0); // Cor do texto (VERDE)
	    draw_text_bitmap(35, 20, "BEM-VINDO");
	    draw_text_bitmap(0, 10, "Este eh o jogo Snake-3D em OpenGL");
	    draw_text_bitmap(-5, 0, "Para iniciar o jogo aperte a tecla espaco");
	}
    else if (game_over){
		glColor3f(1.0, 0.0, 0.0); // Cor do texto (vermelho)
	    draw_text_bitmap(35, 20, "GAME OVER");
	    draw_text_bitmap(25, 10, "Pontuacao Final: " + to_string(points) + " ");
	    draw_text_bitmap(0, 0, "Pressione ESPACO para reiniciar");
	}
}


// Fun��o principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,550);
    glutInitWindowPosition(80,80);
    glutCreateWindow("Snake Game 3D");
    initialize_walls(); // Inicializa as paredes antes de iniciar o jogo
    init();
    
	loadSound(); // carrega o som
    playSound(); // inicia o som
    
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    
	glEnable(GL_DEPTH_TEST);
	lerImagem("./texturas/snake.bmp", 1);
	lerImagem("./texturas/grass_mine.bmp", 2);
	lerImagem("./texturas/paredes.bmp", 3);
    
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    new_food();
    run(0);
    glutMainLoop();
}

// Configura a ilumina��o
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

// Configura��es iniciais
void init(void) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 1.0, 0.0, 1.0f); // Muda a cor de fundo para verde
    
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

// Fun��o para manipular o �ngulo da c�mera
void manipulate_view_angle() {
    glRotatef(view_rotx, 1.0, 0.0, 0.0); // Rotaciona em torno do eixo X
    glRotatef(view_roty, 0.0, 1.0, 0.0); // Rotaciona em torno do eixo Y
    glRotatef(view_rotz, 0.0, 0.0, 1.0); // Rotaciona em torno do eixo Z
}

// Fun��o para resetar o jogo
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

// Fun��o para desenhar a cobra
void draw_snake() {
    int  i;

    // Desenha a cabe�a da cobra e o plano
    glPushMatrix();
    manipulate_view_angle();

    // Desenha o plano que a cobra vai correr
    glPushMatrix();
    glColor3f(0.0, 0.6, 0.2);
    glTranslatef(75.0, -16.0, 75.0);
    glScalef(155, 5.0, 155);
	glutSolidCube(1);
	glPopMatrix();
	

    // Desenha a cabe�a da cobra
    glColor3f(1, 0, 0); // Cor da cabe�a da cobra (vermelho)
    glTranslatef(_x, -10.0, _z);// Pega a posi��o da cabe�a da cobra de acordo com _x e _z
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(10, 20, 20); // Desenha a cabe�a como uma esfera um pouco maior que as esferas do corpo
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
    for (i = 0; i < size; i++) { // Loop atrav�s do tamanho e desenha esferas representando o corpo
        glPushMatrix();
        manipulate_view_angle();
        glTranslatef(body_pos[0][i], -10.0, body_pos[1][i]); // Localiza��o do corpo da cobra
        glColor3f(1,0,0); // Cor do corpo da cobra (vermelho)
        glScalef(0.5,0.5,0.5);
        glutSolidSphere(7,20,20);
        glPopMatrix();
    }
}

// Fun��o para desenhar a comida
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

// Fun��o para desenhar o status do jogo na tela
void game_status() {
    char tmp_str[40];
    glColor3f(1.0, 0.0, 0.0);
    sprintf(tmp_str, "SNAKE 3D - GAME");
    draw_text_bitmap(45, 20, tmp_str);
    glColor3f(0, 0, 0);
    sprintf(tmp_str, "LEVEL: %d PONTOS: %d", lvl, points);
    draw_text_bitmap(40, 10,tmp_str);
}

// Gerador de n�meros aleat�rios para a localiza��o da comida que a cobra vai comer
int random_number(int high, int low) {
    return (rand() % (high-low)) + low;
}

// Gerador de novas posi��es para a comida
void new_food() {
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);

    bool food_on_wall;
    do {
        food_on_wall = false;
        _bx = random_number(_fw - _iw, _iw + 10);
        _bz = random_number(_fh - _ih, _ih + 10);

        // Checa se a comida n�o est� em cima de alguma parede
        for (size_t i = 0; i < wall_positions.size(); i++) {
            if (wall_positions[i].first == _bx && wall_positions[i].second == _bz) {
                food_on_wall = true;
                break;
            }
        }
    } while (food_on_wall);
}

// Fun��o para checar colis�es
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
            ((wall_positions[i].first >= _x) && (wall_positions[i].first <= _x + 2) && (wall_positions[i].second >= _z) && (wall_positions[i].second <= _z + 2)) ||
            ((wall_positions[i].first >= _x) && (wall_positions[i].first <= _x + 2) && (wall_positions[i].second <= _z) && (wall_positions[i].second >= _z - 2)))
            return true;
    }
    
    return false;
}

// Fun��o para movimentar a cobra
void run(int value) {
    if (!paused) {
        int i;
        _oldX[1] = _x;
        _oldZ[1] = _z;

        switch (direction) {
        case RIGHT:
            head_rotation = 90;
            _x += 6;
            // Checa se a cobra est� indo para fora da tela, se sim, ela aparece do outro lado
            if (_x > _fw - 2) _x = _iw - 1;
            break;
        case LEFT:
            head_rotation = -90;
            _x -= 6;
            if (_x < 0) _x = _fw - 2;
            break;
        case UP:
            head_rotation = 0;
            _z += 6;
            if (_z > _fh - 2) _z = _ih - 1;
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

        for (i = 0; i < size; i++) { // Salva as posi��es das partes do corpo
            _oldX[0] = _oldX[1];
            _oldZ[0] = _oldZ[1];
            _oldX[1] = body_pos[0][i];
            _oldZ[1] = body_pos[1][i];
            body_pos[0][i] = _oldX[0];
            body_pos[1][i] = _oldZ[0];
        }

        // Configura o timer para a pr�xima itera��o
        glutTimerFunc(130, run, 0);
    } else {
        // Se o jogo estiver pausado, configura o timer para continuar verificando ap�s um intervalo
        glutTimerFunc(500, run, 0);
    }
}

void drawTexturedCube(GLuint t) {
    GLfloat vertices[][3] = {
        {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},
        {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}
    };

    GLfloat texCoords[][2] = {
        {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}
    };

    GLint faces[][4] = {
        {0, 1, 2, 3}, {1, 5, 6, 2}, {5, 4, 7, 6}, {4, 0, 3, 7}, {3, 2, 6, 7}, {4, 5, 1, 0}
    };

    glBindTexture(GL_TEXTURE_2D, t);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            glTexCoord2fv(texCoords[j]);
            glVertex3fv(vertices[faces[i][j]]);
        }
    }
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Adicione esta fun��o para desenhar as paredes
void draw_walls() {
    for (size_t i = 0; i < wall_positions.size(); ++i) {
        glPushMatrix();
        manipulate_view_angle();
        glTranslatef(wall_positions[i].first, -10.0, wall_positions[i].second);
        glColor3f(0.2, 0.2, 0.2);
        glScalef(0.5, 0.5, 0.5);
        glutSolidCube(7);
        // drawTexturedCube(textureID2);
        glPopMatrix();
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (enable_light) init_light();

    glTranslatef (-60.0, 40.0, zoom);

    welcome_screen_game_over_screen();

    // Checa se o valor da flag "Game Over" n�o � verdadeiro, se for, continua o jogo
    if (!game_over && game_started) {
        game_status();
        draw_food();
        draw_snake();
        draw_walls();
    }
    glutPostRedisplay();
    glutSwapBuffers();
}

// Fun��o para capturar as teclas especiais
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

// Fun��o para capturar as teclas do teclado
void keyboard(unsigned char key, int x, int y) { 
    switch (key) {
    case SPACE: // Iniciar/Reiniciar o jogo
        reset();
        game_started = true;
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
	case 'P': case 'p': // Pausar/Despausar o jogo
        paused = !paused;
        break;
    case ESC: // Sair do jogo
        exit(0);
        break;
    default:
        break;
    }
}
