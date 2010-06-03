/* Font
 *
 * Copyright (C) 2003-2004, Alexander Zaprjagaev <frustum@frustum.org>
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

#include <stdio.h>
#include <stdarg.h>
#include "texture.h"
#include "font.h"

Font::Font(const char *name) {
	
	int width,height;
	unsigned char *data = Texture::load(name,width,height);
	if(!data) {
		fprintf(stderr,"Font::Font(): can't open \"%s\" file\n",name);
		return;
	}
	
	glGenTextures(1,&tex_id);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	
	int size = width;
	step = size / 16;
	for(int y = 0, i = 0; y < 16; y++) {
		for(int x = 0; x < 16; x++, i++) {
			unsigned char *ptr;
			space[i][0] = 0;
			for(int j = 0; j < step; j++) {
				ptr = data + (size * y * step + x * step + j) * 4;
				int k;
				for(k = 0; k < step; k++) {
					if(*(ptr + 3) != 0) break;
					ptr += size * 4;
				}
				if(k != step) break;
				space[i][0]++;
			}
			space[i][1] = 0;
			if(space[i][0] == step) continue;
			for(int j = step - 1; j >= 0; j--) {
				ptr = data + (size * y * step + x * step + j) * 4;
				int k;
				for(k = 0; k < step; k++) {
					if(*(ptr + 3) != 0) break;
					ptr += size * 4;
				}
				if(k != step) break;
				space[i][1]++;
			}
			space[i][1] = step - space[i][0] - space[i][1];
		}
	}
	delete [] data;
	
	list_id = glGenLists(256);
	for(int y = 0, i = 0; y < 16; y++) {
		for(int x = 0; x < 16; x++, i++) {
			float s = (float)x / 16.0f + (float)space[i][0] / (float)size;
			float t = (float)y / 16.0f;
			float ds = (float)space[i][1] / (float)size;
			float dt = 1.0 / 16.0;
			glNewList(list_id + i,GL_COMPILE);
			glBegin(GL_QUADS);
			glTexCoord2f(s,t);
			glVertex2i(0,0);
			glTexCoord2f(s,t + dt);
			glVertex2i(0,step);
			glTexCoord2f(s + ds,t + dt);
			glVertex2i(space[i][1],step);
			glTexCoord2f(s + ds,t);
			glVertex2i(space[i][1],0);
			glEnd();
			glTranslatef((float)space[i][1],0,0);
			glEndList();
		}
	}
}

Font::~Font() {
	glDeleteTextures(1,&tex_id);
	glDeleteLists(256,list_id);
}

/*
 */
void Font::enable(int width,int height) {
	this->width = width;
	this->height = height;
	glGetFloatv(GL_PROJECTION_MATRIX,projection);
	glGetFloatv(GL_MODELVIEW_MATRIX,modelview);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width,height,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}

void Font::disable() {
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelview);
}

void Font::getSize(int font,const char *str,int &x,int &y) {
	x = 0;
	y = 0;
	int lines = 1;
	int length = 0;
	const char *s = str;
	while(*s) {
		if(*s == '\n') {
			lines++;
			if(x < length) x = length;
			length = 0;
		} else if(*s == '\r') {
			if(x < length) x = length;
			length = 0;
		} else if(*s == '\t') {
			length += step * 2;
		} else if(*s == ' ') {
			length += step / 2;
		} else {
			length += space[*(const unsigned char*)s][1];
		}
		s++;
	}
	if(x < length) x = length;
	y = step * lines;
	if(font != 0) {
		x = (int)((float)x * font / step);
		y = (int)((float)y * font / step);
	}
}

/*
 */
void Font::puts(int x,int y,int font,const char *str) {
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	float scale = 1.0f;
	if(font != 0) scale = (float)font / (float)step;
	glScalef(scale,scale,0);
	glTranslatef((float)x,(float)y,0);
	int lines = 0;
	const char *s = str;
	while(*s) {
		if(*s == '\n') {
			lines++;
			glPopMatrix();
			glPushMatrix();
			glScalef(scale,scale,0);
			glTranslatef((float)x / scale,(float)(y + step * lines) / scale,0);
		} else if(*s == '\r') {
			glPopMatrix();
			glPushMatrix();
			glScalef(scale,scale,0);
			glTranslatef((float)x / scale,(float)(y + step * lines) / scale,0);
		} else if(*s == '\t') {
			glTranslatef((float)step * 2.0f / scale,0,0);
		} else if(*s == ' ') {
			glTranslatef((float)step / 2.0f / scale,0,0);
		} else {
			glCallList(list_id + *(const unsigned char*)s);
		}
		s++;
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

/*
 */
void Font::printf(int x,int y,int font,const char *format,...) {
	char buf[1024];
	va_list argptr;
	va_start(argptr,format);
	vsprintf(buf,format,argptr);
	va_end(argptr);
	puts(x,y,font,buf);
}
