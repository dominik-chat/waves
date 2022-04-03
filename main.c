/*
 * Copyright (C) 2022 Dominik Chat
 *
 * This file is part of Waves.
 *
 * Waves is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Waves is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Waves. If not, see <https://www.gnu.org/licenses/>.
 */

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "idft.h"


#define SIZE_D		100.0
#define SIZE		(size_t)SIZE_D
#define FOV		50.0
#define RENDER_DIST	200.0
#define CAMERA_X	-30.0
#define CAMERA_Y	-30.0
#define CAMERA_Z	40.0
#define EYE_X		(SIZE_D/2)
#define EYE_Y		(SIZE_D/2)


static struct idft_ctx *ctx;


static void resize(int w, int h)
{
	float ratio;

	if (h == 0) {
		h = 1;
	}

	ratio = (float)w/(float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(FOV, ratio, 1, RENDER_DIST);
	glMatrixMode(GL_MODELVIEW);
}

static void genNoise(double *tab, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++) {
		tab[i] += (0.003 * (rand() % 100));
	}
}

static void *genWaveMap(size_t width, size_t length)
{
	size_t i;
	size_t size;
	double *map;

	size = width * length;
	map = calloc(size, sizeof(double));

	for (i = 0; i < size; i += width) {
		idftCalc(ctx, &map[i], width);
	}

	genNoise(map, size);

	return (void *)map;
}

GLdouble *calcNormal(GLdouble *a, GLdouble *b, GLdouble *c)
{
	size_t i;
	GLdouble ab[3];
	GLdouble ac[3];
	GLdouble len;
	static GLdouble norm[3];

	/* AB = (Bx - Ax, By - Ay, Bz - Az) */
	for (i = 0; i < 3; i++) {
		ab[i] = b[i] - a[i];
		ac[i] = c[i] - a[i];
	}

	/* ABxAC = (a, b, c) */
	norm[0] = ab[1]*ac[2] - ab[2]*ac[1];
	norm[1] = ab[2]*ac[0] - ab[0]*ac[2];
	norm[2] = ab[0]*ac[1] - ab[1]*ac[0];

	len = sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);
	for(i = 0; i < 3; i++) {
		norm[i] /= len;
	}

	return norm;
}

static void render(void)
{
	size_t x, y;
	double (*map)[SIZE][SIZE];
	GLdouble a[3], b[3], c[3], d[3];
	GLfloat wave_diff[] = {0.0, 0.6, 1.0, 1.0};
	GLfloat wave_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat wave_ambient[] = {0.0, 0.1, 0.2, 1.0};
	GLfloat wave_shine[] = {100.0};

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(CAMERA_X, CAMERA_Y, CAMERA_Z, EYE_X, EYE_Y, 0, 0, 0, 1);

	map = genWaveMap(SIZE, SIZE);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, wave_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, wave_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, wave_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, wave_shine);

	glBegin(GL_TRIANGLES);

	for (x = 0; x < SIZE - 1; x++) {
		for (y = 0; y < SIZE - 1; y++) {
			a[0] = x;	a[1] = y;	a[2] = (*map)[x][y];
			b[0] = x + 1;	b[1] = y;	b[2] = (*map)[x+1][y];
			c[0] = x + 1;	c[1] = y + 1;	c[2] = (*map)[x+1][y+1];
			d[0] = x; 	d[1] = y + 1;	d[2] = (*map)[x][y+1];

			glNormal3dv(calcNormal(a, b, c));
			glVertex3dv(a);
			glVertex3dv(b);
			glVertex3dv(c);

			glNormal3dv(calcNormal(a, c, d));
			glVertex3dv(a);
			glVertex3dv(c);
			glVertex3dv(d);
		}
	}

	free(map);
	glEnd();
	glutSwapBuffers();
}

static struct magnitude mags[] = {
	{2.0, 0.0, 2.5},
	{4.0, 1.25, 0.0},
	{6.0, 0.0, 0.625},
};

int main(int argc, char** argv)
{
	srand(1);
	idftInit(&ctx, mags, 3);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Wave noise simulator");

	GLfloat light_ambient[]={0.2, 0.2, 0.2, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_position[] = {1.0, 1.0, 0.3, 1.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutReshapeFunc(resize);

	glutMainLoop();

	return 0;
}
