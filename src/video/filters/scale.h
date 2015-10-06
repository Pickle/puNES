/*
 *  Copyright (C) 2010-2016 Fabio Cavallo (aka FHorse)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef SCALE_H_
#define SCALE_H_

#include "common.h"
#include "gfx.h"
#include "palette.h"

gfx_filter_function(scale_surface);
void scale_surface1x(WORD **screen_index, uint32_t *palette, BYTE bpp, uint32_t pitch, void *pix);
void scale_surface2x(WORD **screen_index, uint32_t *palette, BYTE bpp, uint32_t pitch, void *pix);
void scale_surface3x(WORD **screen_index, uint32_t *palette, BYTE bpp, uint32_t pitch, void *pix);
void scale_surface4x(WORD **screen_index, uint32_t *palette, BYTE bpp, uint32_t pitch, void *pix);

#endif /* SCALE_H_ */