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


#define PI 3.14159265


struct idft_ctx {
	struct magnitude *mags;
	size_t mag_cnt;
	double time;
};


static double calcPoint(struct idft_ctx *ctx, double t)
{
	size_t i;
	double temp;

	temp = 0.0;

	for (i = 0; i < ctx->mag_cnt; i++) {
		temp += ctx->mags[i].sine * sin(t * ctx->mags[i].frequency);
		temp += ctx->mags[i].cosine * cos(t * ctx->mags[i].frequency);
	}

	return temp;
}


int idftInit(struct idft_ctx **ctx, const struct magnitude *mag, size_t cnt)
{
	if ((mag == NULL) || (cnt == 0) || (ctx == NULL)){
		return EINVAL;
	}

	(*ctx) = malloc(sizeof(**ctx));
	if ((*ctx) == NULL) {
		return ENOMEM;
	}

	(*ctx)->mags = malloc(cnt * sizeof(struct magnitude));
	if ((*ctx)->mags == NULL) {
		return ENOMEM;
	}
	memcpy((*ctx)->mags, mag, cnt * sizeof(struct magnitude));

	(*ctx)->mag_cnt = cnt;
	(*ctx)->time = 0.0;

	return 0;
}

int idftCalc(struct idft_ctx *ctx, double *out, size_t cnt)
{
	size_t i;

	if ((out == NULL) || (cnt == 0) || (ctx == NULL)){
		return EINVAL;
	}

	if (ctx->mags == NULL) {
		return EPERM;
	}

	for (i = 0; i < cnt; i++) {
		out[i] = calcPoint(ctx, ctx->time + i*0.05);
	}

	ctx->time += 0.0001;

	return 0;
}
