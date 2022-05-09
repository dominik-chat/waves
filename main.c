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


static struct idft_ctx *ctx_x;
static struct idft_ctx *ctx_y;


static struct magnitude mags_x[] = {
	{2.0, 0.0, 2.5},
	{4.0, 1.25, 0.0},
	{6.0, 0.0, 0.625},
};
static struct magnitude mags_y[] = {
	{0.7, 0.0, 1.0},
};


static void resize(int w, int h)
{
	double ratio;

	if (h == 0) {
		h = 1;
	}

	ratio = (double)w/(double)h;

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
		tab[i] += (0.0005 * (rand() % 100));
	}
}

static void *genWaveMap(size_t width, size_t length)
{
	size_t i;
	size_t x, y;
	size_t size;
	double *map;
	double *tmp;
	int err;

	size = width * length;
	map = calloc(size, sizeof(double));
	tmp = calloc(size, sizeof(double));

	for (i = 0; i < size; i += width) {
		err = idftCalc(ctx_x, &map[i], width);
		if (err) {
			exit(err);
		}

		err = idftCalc(ctx_y, &tmp[i], width);
		if (err) {
			exit(err);
		}
	}

	for (y = 0; y < SIZE; y++) {
		for (x = 0; x < SIZE; x++) {
			map[y*SIZE + x] *= tmp[x*SIZE + y];
		}
	}

	genNoise(map, size);

	free(tmp);
	return map;
}

static double *calcNormal(double *a, double *b, double *c)
{
	size_t i;
	double ab[3];
	double ac[3];
	double len;
	static double norm[3];

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
	double a[3], b[3], c[3], d[3];
	float wave_diff[] = {0.0f, 0.6f, 1.0f, 1.0f};
	float wave_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float wave_ambient[] = {0.0f, 0.1f, 0.2f, 1.0f};
	float wave_shine[] = {100.0f};

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


int main(int argc, char** argv)
{
	int err;

	srand(1);
	err = idftInit(&ctx_x, mags_x, 3);
	if (err) {
		return err;
	}

	err = idftInit(&ctx_y, mags_y, 1);
	if (err) {
		return err;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Wave noise simulator");

	GLfloat light_ambient[]={0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_position[] = {1.0f, 1.0f, 0.3f, 1.0f};

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

	return err;
}
