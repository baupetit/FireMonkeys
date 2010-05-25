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

#include "buffer.h"
#include <stdio.h>

Buffer::Buffer(int size, GLenum usage, void *data)
{
    //printf("Buffer: using %d Kb of precious vidmem\n", size/1024);

    glGenBuffersARB(1,&id);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,id);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,size, data, usage);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
}

Buffer::~Buffer()
{
    glDeleteBuffersARB(1,&id);
}

void Buffer::bind(GLenum target)
{
    glBindBufferARB(target,id);
}

void Buffer::unbind(GLenum target)
{
    glBindBufferARB(target,0);
}

void Buffer::reset(int size, GLenum usage, void *data)
{
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,size, data, usage);
}

void setData(int size, void *data, int start)
{
    glBufferSubData(GL_ARRAY_BUFFER_ARB, start, size, data);
}

void getData(int size, void *data, int start)
{
    glGetBufferSubData(GL_ARRAY_BUFFER_ARB, start, size, data);
}
