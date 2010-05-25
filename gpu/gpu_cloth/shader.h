/* Shader
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

#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/gl.h>
#include <GL/glext.h>
#include "mathlib.h"

/**
@author Maurizio Monge
*/
class Shader{
public:
    Shader(const char *vertex_file = NULL, const char *fragment_file = NULL,
                                                    const char *header = NULL);
    ~Shader();

    void bind();
    void unbind();
    void dumpVars();
    void printInfoLog(GLhandleARB obj, const char *msg, ...);

    GLuint getAttribLoc(const char*);
    GLint  getUniformLoc(const char*);
    void setUniform(const char*,int);
    void setUniform(const char*,int,const int*);
    void setUniform(const char*,float);
    void setUniform(const char*,int,const float*);
    void setUniform(const char*,vec2);
    void setUniform(const char* var,int n,vec2* f);
    void setUniform(const char*,vec3);
    void setUniform(const char* var,int n,vec3* f);
    void setUniform(const char*,vec4);
    void setUniform(const char* var,int n,vec4* f);
    void setUniform(const char*,mat3,bool transpose=false);
    void setUniform(const char* var,int n,mat3* f,bool transpose);
    void setUniform(const char*,mat4,bool transpose=false);
    void setUniform(const char* var,int n,mat4* f,bool transpose);

    GLhandleARB fragment;
    GLhandleARB vertex;
    GLhandleARB program;
};

#endif //__SHADER_H__
