/*
	Implementação 3D do clássico jogo SNAKE
	UFPI - Ciência da Computação - Computação Gráfica 2023.2
	Prof: Laurindo de Sousa Britto Neto
	Aluno: Heitor Andrade Moura
		   Luis Gustavo da Silva Sousa
    Data: 04/02/2024
 */
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>

void display_callback();

int 
main(int argc, char *argv[])
{
	glutCreateWindow("SNAKE 3D");
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    // glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutDisplayFunc(display_callback);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void display_callback(){
	glutSwapBuffers();
}
