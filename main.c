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
#include "idft.h"


#define FOV		50.0
#define RENDER_DIST	200
#define CAMERA_X	-110.0
#define CAMERA_Y	-30.0
#define CAMERA_Z	50.0


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
		tab[i] = 0.01 * (rand() % 100);
	}
}

static void render(void)
{
	int x, y;
	double cur_dft[100];
	double old_dft[100];
	double cur_noi[100];
	double old_noi[100];
	double temp;

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(CAMERA_X, CAMERA_Y, CAMERA_Z, 0, 0, 0, 0, 0, 1);

	genNoise(old_noi, 100);
	idftCalc(ctx, old_dft, 100);

	glColor3f(0, 0.6, 1);
	glBegin(GL_LINES);
	for (x = -50; x < 50; x++) {
		genNoise(cur_noi, 100);
		idftCalc(ctx, cur_dft, 100);

		for (y = -50; y < 50; y++) {
			temp = old_dft[y+50] + old_noi[y+50];
			glVertex3d((double)x, (double)y, temp);
			temp = cur_dft[y+50] + cur_noi[y+50];
			glVertex3d((double)x + 1.0, (double)y, temp);
		}

		memcpy(old_dft, cur_dft, sizeof(old_dft));
		memcpy(old_noi, cur_noi, sizeof(old_dft));
	}

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

	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutReshapeFunc(resize);

	glutMainLoop();

	return 0;
}
