/* Buffer
 *
 * Copyright (C) 2005, Maurizio Monge <maurizio.monge@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <GL/gl.h>
#include <GL/glext.h>
#include "mathlib.h"

class Buffer
{
public:
    Buffer(int size, GLenum usage = GL_STATIC_DRAW_ARB, void *data = NULL );
    ~Buffer();
    void bind(GLenum target = GL_ARRAY_BUFFER_ARB);
    void unbind(GLenum target = GL_ARRAY_BUFFER_ARB);
    void reset(int size, GLenum usage = GL_STATIC_DRAW_ARB, void *data = NULL );
    void setData(int size, void *data, int start = 0);
    void getData(int size, void *data, int start = 0);

    GLuint id;
};

#endif //__BUFFER_H__
