/* Cloth demo
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

#include "glapp.h"
#include "mathlib.h"
#include "pbuffer.h"
#include "buffer.h"
#include "texture.h"
#include "shader.h"
#include "font.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 128

class GLAppMain : public GLApp
{
    public:
        int init();
        virtual void idle();
        virtual void render();
        void render_pbuf();

        vec3 pos;
        vec3 lookat;
        mat4 modelview;
        mat4 projection;
        Font *font;
        Buffer  *vertex_buffer;
        Buffer  *normal_buffer;
        Buffer  *index_buffer;
        Buffer  *texcoord_buffer;
        PBuffer *pbuffer;
        Texture *img;
        Texture *positions;
        Texture *speeds;
        Texture *normals;
        Shader  *positions_shader;
        Shader  *speeds_shader;
        Shader  *normals_shader;
	int goon;
};

int GLAppMain::init()
{
    goon = 1;
    pos = vec3(0,0.5,5);
    lookat = vec3(0,0,-1);

    font = new Font("data/font.png");
    //img = new Texture( "data/papuanewguinea.png" );
    img = new Texture("data/kiribati.png");


    /* create a pbuffer and some vertex buffers */
    pbuffer = new PBuffer( SIZE, SIZE, PBuffer::RGBA | PBuffer::FLOAT32 );
    vertex_buffer = new Buffer( SIZE*SIZE*4*sizeof(float), GL_DYNAMIC_COPY_ARB );
    normal_buffer = new Buffer( SIZE*SIZE*4*sizeof(float), GL_DYNAMIC_COPY_ARB );


    /* calculate indices buffer data */
    unsigned int *indices;
    indices = (unsigned int*)malloc(SIZE*(SIZE-1)*2*sizeof(unsigned int));
    for(unsigned int j=0;j<SIZE-1;j++)
    for(unsigned int i=0;i<SIZE;i++) {
        if(j&1){
            indices[j*SIZE*2+2*i+1] = SIZE-1-i      +j*SIZE;
            indices[j*SIZE*2+2*i]   = SIZE-1-i+SIZE +j*SIZE;
        } else {
            indices[j*SIZE*2+2*i+1] = i+SIZE +j*SIZE;
            indices[j*SIZE*2+2*i]   = i      +j*SIZE;
        }
    }
    index_buffer = new Buffer( SIZE*(SIZE-1)*2*sizeof(unsigned int),
                                     GL_STATIC_DRAW_ARB, indices );
    free(indices);


    /* calculate texcoords buffer data */
    vec2 *textures;
    textures = (vec2*)malloc(SIZE*SIZE*sizeof(vec2));
    for(unsigned int j=0;j<SIZE;j++)
        for(unsigned int i=0;i<SIZE;i++)
            textures[i+j*SIZE] = vec2((i+0.5)/SIZE,(j+0.5)/SIZE);
    texcoord_buffer = new Buffer( SIZE*SIZE*sizeof(vec2),
                                    GL_STATIC_DRAW_ARB, textures );
    free(textures);


    /* calculate positions texture data */
    vec4 *pos;
    pos = (vec4*)malloc(SIZE*SIZE*sizeof(vec4));
    for(unsigned int j=0;j<SIZE;j++)
        for(unsigned int i=0;i<SIZE;i++)
            pos[i+j*SIZE] = vec4(4.0*i/(SIZE-1)-2.0,1.0,4.0*j/(SIZE-1)-2.0,0.0);
    positions = new Texture( SIZE, SIZE, Texture::TEXTURE_2D, Texture::CLAMP |
               Texture::RGBA | Texture::NEAREST | Texture::FLOAT32, pos );


    /* calculate speeds texture data */
    for(unsigned int j=0;j<SIZE;j++)
        for(unsigned int i=0;i<SIZE;i++)
            pos[i+j*SIZE] = vec4(0.0,0.0,0.0,0.0);
    speeds = new Texture( SIZE, SIZE, Texture::TEXTURE_2D,
                          Texture::RGBA | Texture::NEAREST | Texture::FLOAT32, pos );


    /* calculate normals texture data */
    for(unsigned int j=0;j<SIZE;j++)
        for(unsigned int i=0;i<SIZE;i++)
            pos[i+j*SIZE] = vec4(0.0,1.0,0.0,0.0);
    normals = new Texture( SIZE, SIZE, Texture::TEXTURE_2D,
                           Texture::RGBA | Texture::NEAREST | Texture::FLOAT32, pos );
    free(pos);


    /* load shaders */
    char header[127];
    sprintf(header,"#define SIZE %d\n#define LINK %d\n",
            SIZE,   SIZE<=64 ? 2 : (SIZE>=256 ? 4 : 3));

    positions_shader = new Shader("shaders/positions.vert", "shaders/positions.frag",header);
    positions_shader->bind();
    positions_shader->setUniform("positions",0);
    positions_shader->setUniform("speeds",1);
    positions_shader->setUniform("normals",2);
    positions_shader->unbind();

    speeds_shader = new Shader("shaders/speeds.vert", "shaders/speeds.frag",header);
    speeds_shader->bind();
    speeds_shader->setUniform("speeds",0);
    speeds_shader->setUniform("speeds",1);
    speeds_shader->setUniform("normals",2);
    speeds_shader->unbind();

    normals_shader = new Shader("shaders/normals.vert", "shaders/normals.frag",header);
    normals_shader->bind();
    normals_shader->setUniform("normals",0);
    normals_shader->setUniform("normals",1);
    normals_shader->setUniform("normals",2);
    normals_shader->unbind();

    /* init the state in the pbuffer */
    pbuffer->enable();
        glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0,1,0,1,-1,1);
        glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
    pbuffer->disable();

    /* init the state in the main context */
    glClearColor( 0.2, 0.3, 0.4, 0 );

    return 1;
}

void GLAppMain::idle()
{
    float ratio = (float)window_width / (float) window_height;
    projection.perspective(45,ratio,1,10000);

    vec3 lat = vec3(0,1,0)^lookat;
    lat.normalize();

    // keyboard events
    if(keys[KEY_ESC]) exit();
    if(keys[' ']){ 
	    goon = !goon;
	    keys[' '] = 0;
	  }
    if(keys[KEY_UP]) {
        pos += lookat*(50/fps);
        keys[KEY_UP] = 0;
    }
    if(keys[KEY_DOWN]) {
        pos -= lookat*(50/fps);
        keys[KEY_DOWN] = 0;
    }
    if(keys[KEY_LEFT]) {
        pos += lat*(50/fps);
        keys[KEY_LEFT] = 0;
    }
    if(keys[KEY_RIGHT]) {
        pos -= lat*(50/fps);
        keys[KEY_RIGHT] = 0;
    }

    static int look = 0;
    if(!look && mouse_button & BUTTON_LEFT) {
        setCursor(window_width / 2,window_height / 2);
        mouse_x = window_width / 2;
        mouse_y = window_height / 2;
        look = 1;
    }
    if(mouse_button & BUTTON_RIGHT) look = 0;

    if(look) {
        showCursor(0);
        mat3 m;
        m.rotate( lat, (mouse_y - window_height / 2) * 0.2 );
        lookat = m*lookat;
        m.rotate_y( -(mouse_x - window_width / 2) * 0.2 );
        lookat = m*lookat;
        setCursor(window_width / 2,window_height / 2);
    } else showCursor(1);

    vec3 up = lookat^lat;
    up.normalize();
    modelview.look_at(pos,pos+lookat,up);
}

void GLAppMain::render_pbuf()
{
    pbuffer->enable();
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        positions->bind();
        glActiveTexture(GL_TEXTURE1);
        speeds->bind();
        glActiveTexture(GL_TEXTURE2);
        normals->bind();
        glActiveTexture(GL_TEXTURE0);

        /* render to speeds array */
        speeds_shader->bind();
        glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(0,0,0);
            glTexCoord2f(0,1);
            glVertex3f(0,1,0);
            glTexCoord2f(1,1);
            glVertex3f(1,1,0);
            glTexCoord2f(1,0);
            glVertex3f(1,0,0);
        glEnd();
        speeds_shader->unbind();
        glActiveTexture(GL_TEXTURE1);
        speeds->copy();
        glActiveTexture(GL_TEXTURE0);

        /* render to positions array */
        positions_shader->bind();
        glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(0,0,0);
            glTexCoord2f(0,1);
            glVertex3f(0,1,0);
            glTexCoord2f(1,1);
            glVertex3f(1,1,0);
            glTexCoord2f(1,0);
            glVertex3f(1,0,0);
        glEnd();
        positions_shader->unbind();
        positions->copy();

        /* copy positions to buffer */
        vertex_buffer->bind(GL_PIXEL_PACK_BUFFER_ARB);
        glReadPixels(0,0,SIZE,SIZE,GL_RGBA,GL_FLOAT,0);
        vertex_buffer->unbind(GL_PIXEL_PACK_BUFFER_ARB);

        /* render to normals array */
        normals_shader->bind();
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(0,0,0);
        glTexCoord2f(0,1);
        glVertex3f(0,1,0);
        glTexCoord2f(1,1);
        glVertex3f(1,1,0);
        glTexCoord2f(1,0);
        glVertex3f(1,0,0);
        glEnd();
        normals_shader->unbind();
        glActiveTexture(GL_TEXTURE2);
        normals->copy();
        glActiveTexture(GL_TEXTURE0);

        /* copy normals to buffer */
        normal_buffer->bind(GL_PIXEL_PACK_BUFFER_ARB);
        glReadPixels(0,0,SIZE,SIZE,GL_RGBA,GL_FLOAT,0);
        normal_buffer->unbind(GL_PIXEL_PACK_BUFFER_ARB);
    pbuffer->disable();
}

void GLAppMain::render()
{
    if(goon)
    	render_pbuf();
    //usleep(200000);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelview);

    glEnable(GL_TEXTURE_2D);
    img->bind();

    GLfloat lpos[4] = { pos.x, pos.y+2, pos.z, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDisable(GL_COLOR_MATERIAL);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    vertex_buffer->bind();
    glVertexPointer(3,GL_FLOAT,16,0);
    vertex_buffer->unbind();

    normal_buffer->bind();
    glNormalPointer(GL_FLOAT,16,0);
    normal_buffer->unbind();

    texcoord_buffer->bind();
    glTexCoordPointer(2,GL_FLOAT,8,0);
    texcoord_buffer->unbind();

    index_buffer->bind(GL_ELEMENT_ARRAY_BUFFER);
    glDrawElements(GL_TRIANGLE_STRIP,SIZE*(SIZE-1)*2,GL_UNSIGNED_INT,0);
    index_buffer->unbind(GL_ELEMENT_ARRAY_BUFFER);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    font->enable(800,600);
    font->printf(10,10,20,"fps: %g",fps);
    font->disable();
}

int main(int argc,char **argv)
{
    GLAppMain *glApp = new GLAppMain;

    int width = 1024;
    int height = 768;
    int flags = 0;
    for(int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i],"-w") && i < argc - 1) sscanf(argv[++i],"%d",&width);
        else if(!strcmp(argv[i],"-h") && i < argc - 1) sscanf(argv[++i],"%d",&height);
        else if(!strcmp(argv[i],"-fs")) flags |= GLApp::FULLSCREEN;
        else if(!strcmp(argv[i],"-2x")) flags |= GLApp::MULTISAMPLE_2;
        else if(!strcmp(argv[i],"-4x")) flags |= GLApp::MULTISAMPLE_4;
        else
        {
            glApp->exit("unknown option \"%s\"",argv[i]);
            break;
        }
    }

    if(!glApp->done)
    {
        if(!glApp->setVideoMode(width,height,flags)) return 0;

        glApp->setTitle("Invisible demo http://frustum.org");

        if(!glApp->init()) glApp->exit("initialization failed\n");

        glApp->main();
    }

    delete glApp;

    return 0;
}
