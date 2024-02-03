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
#include <math.h>
#include <vector>

// Biblioteca para uso de som
#include <mmsystem.h>

// Biblioteca para criação das telas 
#include "glut_text.h"

// Definição de constantes
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ESC 27
#define SPACE 32

// Variáveis de controle
GLint lvl = 1;
GLint points = 0;
GLint size = 0;
GLbyte game_over = true;
GLbyte enable_light = true;
GLbyte paused = false;
GLint last_level_changed = 1;
GLbyte game_over_sound_played = false;

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
static GLfloat view_rotx = 45.0F;
static GLfloat view_roty = 0.0F;
static GLfloat view_rotz = 0.0F;

static GLfloat head_rotation = 90.0F;
static GLfloat zoom = -300.0f;

/*************************
|  CONFIGURAÇÕES DE SOM  |
*************************/

// Configurar o som
typedef struct {
    const char* file_name;  // nome do arquivo de som
    const char* ext;        // extensão (wav, mp4, mp3...)
} music;

music background_sound = {"./sounds/snake_song.wav", "wav"};
music score_sound = {"./sounds/score.wav", "wav"};
music win_sound = {"./sounds/win.wav", "wav"};
music game_over_sound = {"./sounds/game_over.wav", "wav"};

// Função para tocar o som
void play_song() {
    PlaySound(background_sound.file_name, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

// Função para tocar o som de pontuação
void play_score_sound() {
    PlaySound(score_sound.file_name, NULL, SND_FILENAME | SND_ASYNC);
}

// Função para tocar o som quando a cobra passar de nível
void play_win_sound() {
    PlaySound(win_sound.file_name, NULL, SND_FILENAME | SND_ASYNC);
}

// Função para tocar o som de game over
void play_game_over_sound() {
    PlaySound(game_over_sound.file_name, NULL, SND_FILENAME | SND_ASYNC);
}

// Função para carregar o som
void load_song() {
    PlaySound(background_sound.file_name, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NOSTOP);
    PlaySound(NULL, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP | SND_NOSTOP | SND_PURGE);
    PlaySound(background_sound.file_name, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NOSTOP | SND_NOWAIT);
}

/*****************************
|  CONFIGURAÇÕES DE TEXTURA  |
*****************************/

//Configura as texturas
// typedef struct {////////////////////////////////////////////////////////////
//     int width;
//     int height;
//     int channels;
//     unsigned char* data;
// } Image;

// // Image* load_image(const char* filename);
// void free_image(Image* image);

// // Armazena o ID da textura gerada pelo OpenGL
// GLuint textureID;////////////////////////////////////////////////////////////
// GLuint textureID1;
// GLuint textureID2;

// void read_image(const char* filename, int texture) {//////////////////////
//     Image* image = load_image(filename);

//     if (image) {
//     	if (texture == 1) {
// 			glGenTextures(1, &textureID);
//         	glBindTexture(GL_TEXTURE_2D, textureID);	
// 		} else if (texture == 2) {
// 			glGenTextures(1, &textureID1);
//         	glBindTexture(GL_TEXTURE_2D, textureID1);
// 		} else {
// 			glGenTextures(1, &textureID2);
//         	glBindTexture(GL_TEXTURE_2D, textureID2);
// 		}

//         // Configura os parâmetros da textura
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//         // Carrega os dados da imagem na textura
//         GLenum format = (image->channels == 3) ? GL_RGB : GL_RGBA;
//         glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);

//         // Gera mipmaps para a textura
//         gluBuild2DMipmaps(GL_TEXTURE_2D, format, image->width, image->height, format, GL_UNSIGNED_BYTE, image->data);

//         // Desvincula a textura
//         glBindTexture(GL_TEXTURE_2D, 0);

//         // Libera a memória da imagem
//         free_image(image);
//     }
// }

/**************************
|  CONFIGURAÇÕES DE MAPA  |
**************************/

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
    // Quadrado maior
    for (int i = 0; i <= 150; ++i) {
        if (i >= 65 && i <= 87) continue; // Buraco
        add_wall(i, 0); // Superior
        add_wall(0, i); // Esquerda
    }

    for (int i = 0; i <= 150; ++i) {
        if (i >= 65 && i <= 87) continue;
        add_wall(i, 151); // Inferior
        add_wall(150, i); // Direita
    }
}

// Função para resetar a cobra quando passar de nível
void reset_snake() {
    _x = 10;
    _z = 15;
    direction = 0;
    size = 0;
    head_rotation = 90.0F;
}

void level_2() {
    wall_positions.clear();
    initialize_walls();

    for (int i = 30; i <= 120; ++i) {
        if (i >= 65 && i <= 87) continue;
        add_wall(i, 30);
        add_wall(30, i);
    }

    for (int i = 30; i <= 120; ++i) {
        if (i >= 65 && i <= 87) continue;
        add_wall(i, 120);
        add_wall(120, i);
    }
}

void level_3() {
    wall_positions.clear();
    initialize_walls();

    for (int i = 30; i <= 120; ++i) {
        if (i >= 65 && i <= 87) continue;
        add_wall(i, 29);
        add_wall(30, i);
    }

    for (int i = 30; i <= 120; ++i) {
        if (i >= 65 && i <= 87) continue;
        add_wall(i, 121);
        add_wall(120, i);
    }

    for (int i = 55; i <= 95; ++i) {
        add_wall(75, i);
        add_wall(i, 75);
    }
}

// Função para mudar de nível
void change_level(int level) {
    switch (level) {
        case 1:
            wall_positions.clear();
            initialize_walls();
            break;
        case 2:
            level_2();
            reset_snake();
            break;
        case 3:
            level_3();
            reset_snake();
            break;
        default:
            // Código para lidar com níveis inválidos
            break;
    }
}

/***************************
|  CONFIGURAÇÕES DE TEXTO  |
***************************/

// Função para desenhar a tela de boas-vindas
bool game_started = false; // Verifica se deve mostrar a tela de Bem-vindo ou Game Over
void welcome_screen_game_over_screen() {
    if (!game_started) {
        glColor3f(0.16, 0.95, 0.43); // Cor do texto (PRETO)
        draw_text_bitmap(35, 24, "BEM-VINDO!");
        glColor3f(0.0, 0.0, 0.0);
        draw_text_bitmap(0, 10, "Este eh o jogo Snake-3D em OpenGL");
        draw_text_bitmap(-5, 0, "Para iniciar o jogo aperte a tecla ESPACO...");
    } else if (game_over) {
        glColor3f(0.95, 0.18, 0.16); // Cor do texto (vermelho)
        draw_text_bitmap(35, 24, "GAME OVER!");
        glColor3f(0.0, 0.0, 0.0);
        draw_text_bitmap(25, 10, "PONTUACAO FINAL: " + to_string(points) + " ");
        draw_text_bitmap(0, 0, "Pressione ESPACO para reiniciar...");
    }
}

/*********************
|  CÓDIGO PRINCIPAL  |
*********************/

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
    glutInitWindowSize(800, 550);
    glutInitWindowPosition(400, 150);
    glutCreateWindow("Snake Game 3D");
    init();
    
    load_song(); // Carrega o som
    play_song(); // Inicia o som
    
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    
    glEnable(GL_DEPTH_TEST);
    // read_image("./texturas/snake.bmp", 1);
    // read_image("./texturas/grass_mine.bmp", 2);
    // read_image("./texturas/paredes.bmp", 3);
    
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
    glClearColor(0.47, 0.32, 0.31, 1.0f); // Muda a cor de fundo para verde
    
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
    view_rotx = 45.0F;
    view_roty = 0.0F;
    view_rotz = 0.0F;
    head_rotation = 90.0F;
}

// Função para desenhar a cobra
void draw_snake() {
    int  i;

    // Desenha a cabeça da cobra e o plano
    glPushMatrix();
    manipulate_view_angle();

    // Desenha o plano que a cobra vai correr
    glPushMatrix();
    glColor3f(0.0, 0.6, 0.2);
    glTranslatef(75.0, -16.0, 75.0);
    glScalef(155, 5.0, 155);
    glutSolidCube(1);
    glPopMatrix();

    // Desenha a cabeça da cobra
    glColor3f(1, 0, 0); // Cor da cabeça da cobra (vermelho)
    glTranslatef(_x, -10.0, _z); // Pega a posição da cabeça da cobra de acordo com _x e _z
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

// Função para desenhar o status do jogo na tela
void game_status() {
    char tmp_str[40];
    glColor3f(0.0, 0.0, 0.0);
    sprintf(tmp_str, "SNAKE 3D - GAME");
    draw_text_bitmap(43, 22, tmp_str);
    glColor3f(0, 0, 0);
    sprintf(tmp_str, "NIVEL: %d    PONTOS: %d", lvl, points);
    draw_text_bitmap(38, 10, tmp_str);
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

    const int min_distance = 10; // A distância mínima entre a comida e as paredes

    bool food_too_close_to_wall;
    do {
        food_too_close_to_wall = false;
        _bx = random_number(_fw - _iw, _iw + 10);
        _bz = random_number(_fh - _ih, _ih + 10);

        // Checa se a comida não está muito perto de alguma parede
        for (size_t i = 0; i < wall_positions.size(); i++) {
            if (abs(wall_positions[i].first - _bx) <= min_distance &&
                abs(wall_positions[i].second - _bz) <= min_distance) {
                food_too_close_to_wall = true;
                break;
            }
        }
    } while (food_too_close_to_wall);
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
        double distance = sqrt(pow(wall_positions[i].first - _x, 2) + pow(wall_positions[i].second - _z, 2));
        if (distance <= 3) {
            return true;
        }
    }
    
    return false;
}

// Função para movimentar a cobra
void run(int value) {
    if (!paused) {
        int i;
        _oldX[1] = _x;
        _oldZ[1] = _z;

        switch (direction) {
        case RIGHT:
            head_rotation = 90;
            _x += 6;
            // Checa se a cobra está indo para fora da tela, se sim, ela aparece do outro lado
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
        if (collision()) {
            game_over = true;
            if (!game_over_sound_played) {
                play_game_over_sound(); // Toca o som de game over
                game_over_sound_played = true;
            }
        }

        // Checa se a cobra comeu a comida, se sim, aumenta os pontos e o tamanho da cobra e cria uma nova comida
        if ((_x == _bx && _z == _bz) ||
            ((_x >= _bx) && (_x <= _bx + 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
            ((_x <= _bx) && (_x >= _bx - 4) && (_z <= _bz) && (_z >= _bz - 4)) ||
            ((_x <= _bx) && (_x >= _bx - 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
            ((_x >= _bx) && (_x <= _bx + 4) && (_z <= _bz) && (_z >= _bz - 4))) {
            points++;
            // play_score_sound(); // Toca o som de pontuação
            if (points < 100) size++;
            if ((points % 10) == 0 && lvl < 3) lvl++;
            
            if (lvl != last_level_changed) {
                change_level(lvl);
                play_song(); // Reinicia o som
                last_level_changed = lvl;
            }
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

        // Configura o timer para a próxima iteração
        glutTimerFunc(110, run, 0);
    } else {
        // Se o jogo estiver pausado, configura o timer para continuar verificando após um intervalo
        glutTimerFunc(500, run, 0);
    }
}

// void drawTexturedCube(GLuint t) {
//     GLfloat vertices[][3] = {
//         {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},
//         {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}
//     };

//     GLfloat texCoords[][2] = {
//         {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}
//     };

//     GLint faces[][4] = {
//         {0, 1, 2, 3}, {1, 5, 6, 2}, {5, 4, 7, 6}, {4, 0, 3, 7}, {3, 2, 6, 7}, {4, 5, 1, 0}
//     };

//     glBindTexture(GL_TEXTURE_2D, t);
//     glEnable(GL_TEXTURE_2D);

//     glBegin(GL_QUADS);
//     for (int i = 0; i < 6; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             glTexCoord2fv(texCoords[j]);
//             glVertex3fv(vertices[faces[i][j]]);
//         }
//     }
//     glEnd();
    
//     glDisable(GL_TEXTURE_2D);
//     glBindTexture(GL_TEXTURE_2D, 0);
// }

// Adicione esta função para desenhar as paredes
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

    // Checa se o valor da flag "Game Over" não é verdadeiro, se for, continua o jogo
    if (!game_over && game_started) {
        game_status();
        draw_food();
        draw_snake();
        draw_walls();
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
        if (game_over) play_song(); // Reinicia o som

        reset();
        game_started = true;
        game_over_sound_played = false;
        lvl = last_level_changed = 1;
        change_level(lvl);
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
