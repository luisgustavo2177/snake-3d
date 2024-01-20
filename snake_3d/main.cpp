/*
	Implementa��o 3D do cl�ssico jogo SNAKE
	UFPI - Ci�ncia da Computa��o - Computa��o Gr�fica 2023.2
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

//Declara��o das fun��es
void display_callback();

void init(){
	glClearColor(1.0, 0.5, 0.0,  1.0); //(R, G, B, alpha);
}

//Fun��o Principal
int main(int argc, char *argv[]){
	glutCreateWindow("SNAKE 3D - OpenGL");//Nome do jogo
    glutInit(&argc, argv);
    glutInitWindowSize(1280, 720);//Tamanho da janela
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutDisplayFunc(display_callback);
    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}

//Fun��es usadas
void display_callback(){
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}
