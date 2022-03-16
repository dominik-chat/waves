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

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "idft.h"


#define PI 3.14159265d


static struct magnitude *magnitudes;
static size_t mag_cnt;
static volatile double time;


static double calcPoint(double t)
{
	size_t i;
	double temp;

	temp = 0.0d;

	for (i = 0; i < mag_cnt; i++) {
		temp += magnitudes[i].sine * sin(t * (double)i);
		temp += magnitudes[i].cosine * cos(t * (double)i);
	}

	return temp;
}


int idftInit(const struct magnitude *mag, size_t cnt)
{
	if ((mag == NULL) || (cnt == 0)){
		return EINVAL;
	}

	magnitudes = malloc(cnt * sizeof(*magnitudes));
	if (magnitudes == NULL) {
		return ENOMEM;
	}

	memcpy(magnitudes, mag, cnt * sizeof(*magnitudes));

	mag_cnt = cnt;
	time = 0.0d;

	return 0;
}

int idftCalc(double *out, size_t cnt)
{
	size_t i;

	if ((out == NULL) || (cnt == 0)){
		return EINVAL;
	}

	if (magnitudes == NULL) {
		return EPERM;
	}

	for (i = 0; i < cnt; i++) {
		out[i] = calcPoint(time + i*0.05d);
	}

	time += 0.0001d;

	return 0;
}
