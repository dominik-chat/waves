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

#ifndef _IDFT_H_
#define _IDFT_H_

#include <stdint.h>


struct magnitude {
	double frequency;
	double sine;
	double cosine;
};

struct idft_ctx;


/**
 * @brief Initialize inverse DFT module.
 *
 * Initialize inverse DFT module by saving the
 * parameters locally.
 *
 * @param ctx Pointer to context of initialized instance.
 * @param mag Pointer to an array of magnitudes.
 * @param cnt Number of consecutive magnitudes.
 *
 * @retval 0 Success.
 * @retval EINVAL Invalid value of parameters.
 * @retval ENOMEM Not enough memory.
 */
int idftInit(struct idft_ctx **ctx, const struct magnitude *mag, size_t cnt);

/**
 * @brief Calculate current idft.
 *
 * Calculate idft of current time index for
 * given magnitudes
 *
 * @param ctx Context of initialized instance.
 * @param out Pointer to an outpu array.
 * @param cnt Number of an output array.
 *
 * @retval 0 Success.
 * @retval EINVAL Invalid value of parameters.
 * @retval EPERM Not initialized.
 */
int idftCalc(struct idft_ctx *ctx, double *out, size_t cnt);

#endif /* _IDFT_H_ */
