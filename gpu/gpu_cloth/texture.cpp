/* Texture
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
#include <string.h>
#include <png.h>
#include <setjmp.h>
extern "C"
{
#include <jpeglib.h>
}

#ifdef _WIN32
#pragma comment (lib,"libpng.lib")
#pragma comment (lib,"zlib.lib")
#pragma comment (lib,"libjpeg.lib")
#endif

#include "texture.h"

/*
 */
Texture::Texture(int width,int height,GLuint target,int flag, void* data)
        : width(width), height(height), depth(1), flag(flag), target(target)
{
    glGenTextures(1,&id);
    glBindTexture(target,id);

    if(flag & LUMINANCE) format = GL_LUMINANCE;
    else if(flag & LUMINANCE_ALPHA) format = GL_LUMINANCE_ALPHA;
    else if(flag & RGB) format = GL_RGB;
    else if(flag & RGBA) format = GL_RGBA;
    else
    {
        fprintf(stderr,"Texture::Texture(): unknown texture format\n");
        return;
    }

    type = GL_UNSIGNED_BYTE;
    internalformat = format;

    if(flag & FLOAT16)
    {
        type = GL_FLOAT;
        if(flag & LUMINANCE) internalformat = GL_LUMINANCE_FLOAT16_ATI;
        else if(flag & LUMINANCE_ALPHA) internalformat = GL_LUMINANCE_ALPHA_FLOAT16_ATI;
        else if(flag & RGB) internalformat = GL_RGB_FLOAT16_ATI;
        else internalformat = GL_RGBA_FLOAT16_ATI;
    }
    if(flag & FLOAT32)
    {
        type = GL_FLOAT;
        if(flag & LUMINANCE) internalformat = GL_LUMINANCE_FLOAT32_ATI;
        else if(flag & LUMINANCE_ALPHA) internalformat = GL_LUMINANCE_ALPHA_FLOAT32_ATI;
        else if(flag & RGB) internalformat = GL_RGB_FLOAT32_ATI;
        else internalformat = GL_RGBA_FLOAT32_ATI;
    }
    else if(flag & DXT_1)
    {
        if(flag & RGB) internalformat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        else if(flag & RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        else fprintf(stderr,"Texture::Texture(): can't set DXT_1 compression for not RGB or RGBA format\n");
    }
    else if(flag & DXT_3)
    {
        if(flag & RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        else fprintf(stderr,"Texture::Texture(): can't set DXT_3 compression for not RGBA format\n");
    }
    else if(flag & DXT_5)
    {
        if(flag & RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        else fprintf(stderr,"Texture::Texture(): can't set DXT_5 compression for not RGBA format\n");
    }

    if(flag & CLAMP)
    {
        glTexParameteri(target,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(target,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameteri(target,GL_TEXTURE_WRAP_R,GL_CLAMP);
    }
    else if(flag & CLAMP_TO_EDGE)
    {
        glTexParameteri(target,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(target,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(target,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    }

    if(flag & NEAREST)
    {
        glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    }
    else if(flag & LINEAR)
    {
        glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    }
    else if(flag & NEAREST_MIPMAP_NEAREST)
    {
        glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
        glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    }
    else if(flag & LINEAR_MIPMAP_NEAREST)
    {
        glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
        glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    }
    else if(flag & LINEAR_MIPMAP_LINEAR)
    {
        glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
        glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    }

    if(flag & ANISOTROPY_1) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,1.0);
    else if(flag & ANISOTROPY_2) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,2.0);
    else if(flag & ANISOTROPY_4) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,4.0);
    else if(flag & ANISOTROPY_8) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,8.0);
    else if(flag & ANISOTROPY_16) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,16.0);


    int tsize;
    switch(internalformat)
    {
        case GL_RGB: tsize = 3; break;
        case GL_RGBA: tsize = 4; break;
        case GL_RGB_FLOAT16_ATI: tsize = 6; break;
        case GL_RGBA_FLOAT16_ATI: tsize = 8; break;
        case GL_RGB_FLOAT32_ATI: tsize = 12;break;
        case GL_RGBA_FLOAT32_ATI: tsize = 16;break;
        default: tsize = 0;
    }
    //printf("Texture: using %d Kb of precious vidmem\n", (width*height*tsize)/1024);

    if(target == TEXTURE_2D || target == TEXTURE_RECT)
        glTexImage2D(target,0,internalformat,width,height,0,format,type,data);
    else if(target == TEXTURE_CUBE)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,internalformat,width,height,0,format,type,NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,internalformat,width,height,0,format,type,NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,internalformat,width,height,0,format,type,NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,internalformat,width,height,0,format,type,NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,internalformat,width,height,0,format,type,NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,internalformat,width,height,0,format,type,NULL);
    }
    else if(target == TEXTURE_3D) glTexImage3D(target,0,internalformat,width,height,depth,0,format,type,data);
}

/*
 */
Texture::Texture(const char *name,GLuint target,int flag)
{
    load(name,target,flag);
}

/*
 */
Texture::~Texture()
{
    glDeleteTextures(1,&id);
}

/*
 */
void Texture::load(const char *name,GLuint target,int flag)
{

    width = 0;
    height = 0;
    depth = 1;
    this->target = target;
    this->flag = flag;

    glGenTextures(1,&id);
    glBindTexture(target,id);

    if(flag & LUMINANCE) format = GL_LUMINANCE;
    else if(flag & LUMINANCE_ALPHA) format = GL_LUMINANCE_ALPHA;
    else if(flag & RGB) format = GL_RGB;
    else if(flag & RGBA) format = GL_RGBA;
    else
    {
        fprintf(stderr,"Texture::Texture(): unknown texture format\n");
        return;
    }

    type = GL_UNSIGNED_BYTE;
    internalformat = format;

    if(flag & FLOAT16 ||flag & FLOAT32 )
        fprintf(stderr,"Texture::Texture(): FLOAT flag is not accessible for \"%s\" file\n",name);

    if(flag & DXT_1)
    {
        if(flag & RGB) internalformat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        else if(flag & RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        else fprintf(stderr,"Texture::Texture(): can't set DXT_1 compression for not RGB or RGBA format\n");
    }
    else if(flag & DXT_3)
    {
        if(flag & RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        else fprintf(stderr,"Texture::Texture(): can't set DXT_3 compression for not RGBA format\n");
    }
    else if(flag & DXT_5)
    {
        if(flag & RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        else fprintf(stderr,"Texture::Texture(): can't set DXT_5 compression for not RGBA format\n");
    }

    if(flag & CLAMP)
    {
        glTexParameteri(target,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(target,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameteri(target,GL_TEXTURE_WRAP_R,GL_CLAMP);
    }
    else if(flag & CLAMP_TO_EDGE)
    {
        glTexParameteri(target,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(target,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(target,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    }

    if(flag & NEAREST)
    {
        glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    }
    else if(flag & LINEAR)
    {
        glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    }
    else if(flag & NEAREST_MIPMAP_NEAREST)
    {
        glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
        glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    }
    else if(flag & LINEAR_MIPMAP_NEAREST)
    {
        glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
        glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    }
    else if(flag & LINEAR_MIPMAP_LINEAR)
    {
        glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
        glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    }

    if(flag & ANISOTROPY_1) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,1.0);
    else if(flag & ANISOTROPY_2) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,2.0);
    else if(flag & ANISOTROPY_4) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,4.0);
    else if(flag & ANISOTROPY_8) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,8.0);
    else if(flag & ANISOTROPY_16) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,16.0);

    if(target == TEXTURE_2D || target == TEXTURE_RECT)
    {
        unsigned char *data = load(name,width,height);
        if(data && format != GL_RGBA)
        {
            if(format == GL_LUMINANCE) data = rgba2luminance(data,width,height);
            else if(format == GL_LUMINANCE_ALPHA) data = rgba2luminance_alpha(data,width,height);
            else if(format == GL_RGB) data = rgba2rgb(data,width,height);
        }
        glTexImage2D(target,0,internalformat,width,height,0,format,GL_UNSIGNED_BYTE,data);
        if(data) delete [] data;
    }
    else if(target == TEXTURE_CUBE)
    {
        GLuint targets[6] = {
                                GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
                            };
        char *suffix[6] = { "px", "nx", "py", "ny", "pz", "nz" };
        for(int i = 0; i < 6; i++)
        {
            char buf[1024];
            sprintf(buf,name,suffix[i]);
            unsigned char *data = load(buf,width,height);
            if(data && format != GL_RGBA)
            {
                if(format == GL_LUMINANCE) data = rgba2luminance(data,width,height);
                else if(format == GL_LUMINANCE_ALPHA) data = rgba2luminance_alpha(data,width,height);
                else if(format == GL_RGB) data = rgba2rgb(data,width,height);
            }
            glTexImage2D(targets[i],0,internalformat,width,height,0,format,GL_UNSIGNED_BYTE,data);
            if(data) delete [] data;
        }
    }
    else if(target == TEXTURE_3D)
    {
        int fmt;
        unsigned char *data = load_3d(name,width,height,depth,fmt);
        if(fmt == GL_LUMINANCE) format = GL_LUMINANCE;
        else if(fmt == GL_LUMINANCE_ALPHA) format = GL_LUMINANCE_ALPHA;
        else if(fmt == RGB) format = GL_RGB;
        else format = GL_RGBA;
        glTexImage3D(target,0,internalformat,width,height,depth,0,format,GL_UNSIGNED_BYTE,data);
        if(data) delete [] data;
    }
}

/*
 */
void Texture::enable(Texture *old)
{
    if(old)
    {
        if(old == this) return;
        if(old->target != target)
        {
            glDisable(old->target);
            glEnable(target);
        }
    }
    else
    {
        glEnable(target);
    }
}

void Texture::disable()
{
    glDisable(target);
}

void Texture::bind(Texture *old)
{
    if(old == this) return;
    glBindTexture(target,id);
}

/*
 */
void Texture::enable(int unit,Texture *old)
{
    if(old == this) return;
    glActiveTexture(GL_TEXTURE0 + unit);
    enable(old);
    glActiveTexture(GL_TEXTURE0);
}

void Texture::disable(int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    disable();
    glActiveTexture(GL_TEXTURE0);
}

void Texture::bind(int unit,Texture *old)
{
    if(old == this) return;
    glActiveTexture(GL_TEXTURE0 + unit);
    bind(old);
    glActiveTexture(GL_TEXTURE0);
}

/*
 */
void Texture::copy(GLuint target)
{
    if(target == 0) glCopyTexSubImage2D(this->target,0,0,0,0,0,width,height);
    else glCopyTexSubImage2D(target,0,0,0,0,0,width,height);
}

/*
 */
void Texture::render(float x0,float y0,float x1,float y1)
{
    glBegin(GL_QUADS);
    if(target == TEXTURE_RECT)
    {
        glTexCoord2i(0,0);
        glVertex2f(x0,y0);
        glTexCoord2i(width,0);
        glVertex2f(x1,y0);
        glTexCoord2i(width,height);
        glVertex2f(x1,y1);
        glTexCoord2i(0,height);
        glVertex2f(x0,y1);
    }
    else
    {
        glTexCoord2f(0,0);
        glVertex2f(x0,y0);
        glTexCoord2f(1,0);
        glVertex2f(x1,y0);
        glTexCoord2f(1,1);
        glVertex2f(x1,y1);
        glTexCoord2f(0,1);
        glVertex2f(x0,y1);
    }
    glEnd();
}

/*****************************************************************************/
/*                                                                           */
/* IO functions                                                              */
/*                                                                           */
/*****************************************************************************/

/*
 */
unsigned char *Texture::load(const char *name,int &width,int &height)
{
    if(strstr(name,".tga")) return load_tga(name,width,height);
    if(strstr(name,".png")) return load_png(name,width,height);
    if(strstr(name,".jpg")) return load_jpeg(name,width,height);
    if(strstr(name,".dds")) return load_dds(name,width,height);
    fprintf(stderr,"Texture::load(): unknown format of \"%s\" file\n",name);
    return NULL;
}

int Texture::save(const char *name,const unsigned char *data,int width,int height)
{
    if(strcmp(name,".tga")) return save_tga(name,data,width,height);
    if(strcmp(name,".jpg")) return save_jpeg(name,data,width,height,80);
    fprintf(stderr,"Texture::save(): unknown format of \"%s\" file\n",name);
    return 0;
}

/*****************************************************************************/
/*                                                                           */
/* tga images                                                                */
/*                                                                           */
/*****************************************************************************/

/* load tga images
 */
unsigned char *Texture::load_tga(const char *name,int &width,int &height)
{
    FILE *file = fopen(name,"rb");
    if(!file)
    {
        fprintf(stderr,"Texture::load_tga(): error open \"%s\" file\n",name);
        return NULL;
    }
    unsigned char info[18];
    fread(&info,1,18,file);
    width = info[12] + info[13] * 256;
    height = info[14] + info[15] * 256;
    int components = 0;
    if(info[16] == 24) components = 3;
    else if(info[16] == 32) components = 4;
    else
    {
        fprintf(stderr,"Texture::load_tga(): only 24 and 32 bpp images are suported\n");
        fclose(file);
        return NULL;
    }
    int size = width * height * components;
    unsigned char *buf = new unsigned char[size];
    unsigned char *data = new unsigned char[width * height * 4];
    fseek(file,info[0],SEEK_CUR);
    if(info[2] == 2)
    {
        fread(buf,1,size,file);
    }
    else if(info[2] == 10)
    {
        unsigned char *ptr = buf;
        for(int i = 0; i < size;)
        {
            unsigned char rep;
            fread(&rep,1,1,file);
            if(rep & 0x80)
            {
                rep ^= 0x80;
                fread(ptr,1,components,file);
                ptr += components;
                for(int j = 0; j < rep * components; j++)
                {
                    *ptr = *(ptr - components);
                    ptr++;
                }
                i += components * (rep + 1);
            }
            else
            {
                int j = components * (rep + 1);
                fread(ptr,1,j,file);
                ptr += j;
                i += j;
            }
        }
    }
    else
    {
        fclose(file);
        delete [] buf;
        delete [] data;
        return NULL;
    }
    // bgr->rgb
    for(int i = 0, j = 0; i < size; i += components, j += 4)
    {
        data[j] = buf[i + 2];
        data[j + 1] = buf[i + 1];
        data[j + 2] = buf[i];
        if(components == 4) data[j + 3] = buf[i + 3];
        else data[j + 3] = 255;
    }
    // flip verticaly
    if(!(info[17] & 0x20))
    {
        for(int j = 0; j < height / 2; j++)
        {
            for(int i = 0; i < width * 4; i++)
            {
                unsigned char c = data[j * width * 4 + i];
                data[j * width * 4 + i] = data[(height - j - 1) * width * 4 + i];
                data[(height - j - 1) * width * 4 + i] = c;
            }
        }
    }
    fclose(file);
    delete [] buf;
    return data;
}

/* save tga images
 */
int Texture::save_tga(const char *name,const unsigned char *data,int width,int height)
{
    FILE *file = fopen(name,"wb");
    if(!file)
    {
        fprintf(stderr,"Texture::save_tga(): error create \"%s\" file\n",name);
        return 0;
    }
    unsigned char *buf = new unsigned char[18 + width * height * 4];
    memset(buf,0,18);
    buf[2] = 2;
    buf[12] = width % 256;
    buf[13] = width / 256;
    buf[14] = height % 256;
    buf[15] = height / 256;
    buf[16] = 32;
    buf[17] = 0x28;
    memcpy(buf + 18,data,width * height * 4);
    // rgba->bgra
    for(int i = 18; i < 18 + width * height * 4; i += 4)
    {
        unsigned char c = buf[i];
        buf[i] = buf[i + 2];
        buf[i + 2] = c;
    }
    fwrite(buf,1,18 + width * height * 4,file);
    fclose(file);
    delete [] buf;
    return 1;
}

/*****************************************************************************/
/*                                                                           */
/* jpeg images                                                               */
/*                                                                           */
/*****************************************************************************/

/* load jpeg images
 */
struct my_error_mgr
{
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

static void my_error_exit(j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr)cinfo->err;
    (*cinfo->err->output_message)(cinfo);
    longjmp(myerr->setjmp_buffer,1);
}

unsigned char *Texture::load_jpeg(const char *name,int &width,int &height)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    FILE *file = fopen(name,"rb");
    if(!file)
    {
        fprintf(stderr,"Texture::load_jpeg(): error open \"%s\" file\n",name);
        return NULL;
    }
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if(setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        fclose(file);
        return NULL;
    }
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo,file);
    jpeg_read_header(&cinfo,TRUE);
    jpeg_start_decompress(&cinfo);
    int cont = 0;
    int row_stride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo,JPOOL_IMAGE,row_stride,1);
    JSAMPLE *data_buffer = new JSAMPLE[cinfo.image_width * cinfo.image_height * cinfo.output_components];
    while(cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo,buffer,1);
        memcpy(data_buffer + cinfo.image_width * cinfo.output_components * cont,buffer[0],row_stride);
        cont++;
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    width = cinfo.image_width;
    height = cinfo.image_height;
    unsigned char *data = new unsigned char[width * height * 4];
    switch(cinfo.output_components)
    {
            case 1:
            for(int i = 0, j = 0; i < width * height; i++, j += 4)
            {
                data[j] = data[j + 1] = data[j + 2] = data_buffer[i];
                data[j + 3] = 255;
            }
            break;
            case 3:
            for(int i = 0, j = 0; i < width * height * 3; i += 3, j += 4)
            {
                data[j] = data_buffer[i];
                data[j + 1] = data_buffer[i + 1];
                data[j + 2] = data_buffer[i + 2];
                data[j + 3] = 255;
            }
            break;
            default:
            delete [] data;
            delete [] data_buffer;
            return NULL;
    }
    delete [] data_buffer;
    fclose(file);
    return data;
}

/* save jpeg images
 */
int Texture::save_jpeg(const char *name,const unsigned char *data,int width,int height,int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *file = fopen(name,"wb");
    if(!file)
    {
        fprintf(stderr,"Texture::load_jpeg(): error create \"%s\" file\n",name);
        return 0;
    }
    unsigned char *data_buffer = new unsigned char[width * height * 3];
    for(int i = 0, j = 0; i < width * height * 4; i += 4, j += 3)
    {
        data_buffer[j + 0] = data[i + 0];
        data_buffer[j + 1] = data[i + 1];
        data_buffer[j + 2] = data[i + 2];
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo,file);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo,quality,TRUE);
    jpeg_start_compress(&cinfo,TRUE);
    int row_stride = width * 3;
    while(cinfo.next_scanline < cinfo.image_height)
    {
        JSAMPROW row_pointer;
        row_pointer = &data_buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo,&row_pointer,1);
    }
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    delete [] data_buffer;
    fclose(file);
    return 1;
}

/*****************************************************************************/
/*                                                                           */
/* png images                                                                */
/*                                                                           */
/*****************************************************************************/

/*
 */
unsigned char *Texture::load_png(const char *name,int &width,int &height)
{
    FILE *file = fopen(name,"rb");
    if(!file)
    {
        fprintf(stderr,"Texture::load_png(): error open \"%s\" file\n",name);
        return NULL;
    }
    png_byte sig[8];
    fread(sig,8,1,file);
    if(!png_check_sig(sig,8))
    {
        fprintf(stderr,"Texture::load_png(): wrong signature in \"%s\" file\n",name);
        fclose(file);
        return NULL;
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
    if(!png_ptr)
    {
        fclose(file);
        return NULL;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        png_destroy_read_struct(&png_ptr,0,0);
        fclose(file);
        return NULL;
    }
    png_init_io(png_ptr,file);
    png_set_sig_bytes(png_ptr,8);
    png_read_info(png_ptr,info_ptr);
    unsigned long w,h;
    int bit_depth,color_type;
    png_get_IHDR(png_ptr,info_ptr,&w,&h,&bit_depth,&color_type,0,0,0);
    if(bit_depth == 16) png_set_strip_16(png_ptr);
    if(color_type == PNG_COLOR_TYPE_PALETTE) png_set_expand(png_ptr);
    if(bit_depth < 8) png_set_expand(png_ptr);
    if(png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS)) png_set_expand(png_ptr);
    if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);
    double gamma;
    if(png_get_gAMA(png_ptr,info_ptr,&gamma)) png_set_gamma(png_ptr,(double)2.2,gamma);
    png_read_update_info(png_ptr,info_ptr);
    png_get_IHDR(png_ptr,info_ptr,&w,&h,&bit_depth,&color_type,0,0,0);
    png_uint_32 row_bytes = png_get_rowbytes(png_ptr,info_ptr);
    png_uint_32 channels = png_get_channels(png_ptr,info_ptr);
    png_byte *img = new png_byte[row_bytes * h];
    png_byte **row = new png_byte*[h];
    for(int i = 0; i < (int)h; i++)
    {
        row[i] = img + row_bytes * i;
    }
    png_read_image(png_ptr,row);
    png_read_end(png_ptr,NULL);
    png_destroy_read_struct(&png_ptr,0,0);
    fclose(file);
    delete [] row;
    width = w;
    height = h;
    unsigned char *data = new unsigned char[width * height * 4];
    unsigned char *ptr = data;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int k = row_bytes * i + j * channels;
            *ptr++ = img[k + 0];
            *ptr++ = img[k + 1];
            *ptr++ = img[k + 2];
            if(channels == 4) *ptr++ = img[k + 3];
            else *ptr++ = 255;
        }
    }
    delete [] img;
    return data;
}

/*****************************************************************************/
/*                                                                           */
/* dds images                                                                */
/*                                                                           */
/*****************************************************************************/

struct dds_colorkey
{
    unsigned int dwColorSpaceLowValue;
    unsigned int dwColorSpaceHighValue;
};

struct dds_header
{
    unsigned int magic;
    unsigned int dwSize;
    unsigned int dwFlags;
    unsigned int dwHeight;
    unsigned int dwWidth;
    int lPitch;
    unsigned int dwDepth;
    unsigned int dwMipMapCount;
    unsigned int dwAlphaBitDepth;
    unsigned int dwReserved;
    void *lpSurface;
    dds_colorkey ddckCKDestOverlay;
    dds_colorkey ddckCKDestBlt;
    dds_colorkey ddckCKSrcOverlay;
    dds_colorkey ddckCKSrcBlt;
    unsigned int dwPFSize;
    unsigned int dwPFFlags;
    unsigned int dwFourCC;
    unsigned int dwRGBBitCount;
    unsigned int dwRBitMask;
    unsigned int dwGBitMask;
    unsigned int dwBBitMask;
    unsigned int dwRGBAlphaBitMask;
    unsigned int dwCaps;
    unsigned int dwCaps2;
    unsigned int dwCaps3;
    unsigned int dwVolumeDepth;
    unsigned int dwTextureStage;
};

struct dds_color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

enum {
    DDS_ERROR = -1,
    DDS_RGB,
    DDS_RGBA,
    DDS_DXT1,
    DDS_DXT2,
    DDS_DXT3,
    DDS_DXT4,
    DDS_DXT5
};

enum {
    DDPF_ALPHAPIXELS = 0x01,
    DDPF_FOURCC = 0x04,
    DDPF_RGB = 0x40,
    DDPF_RGBA = 0x41
};

unsigned char *Texture::load_dds(const char *name,int &width,int &height)
{
    FILE *file = fopen(name,"rb");
    if(!file)
    {
        fprintf(stderr,"Texture::load_dds(): error open \"%s\" file\n",name);
        return NULL;
    }
    dds_header header;
    fread(&header,sizeof(dds_header),1,file);
    if(header.magic != ('D' | 'D' << 8 | 'S' << 16 | ' ' << 24))
    {
        fprintf(stderr,"Texture::load_dds(): wrong magic in \"%s\" file\n",name);
        fclose(file);
        return NULL;
    }
    width = header.dwWidth;
    height = header.dwHeight;
    int format = DDS_ERROR;
    if(header.dwPFFlags & DDPF_FOURCC)
    {
        if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('1' << 24))) format = DDS_DXT1;
        else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('2' << 24))) format = DDS_DXT2;
        else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('3' << 24))) format = DDS_DXT3;
        else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('4' << 24))) format = DDS_DXT4;
        else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('5' << 24))) format = DDS_DXT5;
    }
    else if(header.dwPFFlags == DDPF_RGB && header.dwRGBBitCount == 24) format = DDS_RGB;
    else if(header.dwPFFlags == DDPF_RGBA && header.dwRGBBitCount == 32) format = DDS_RGBA;
    if(format == DDS_ERROR)
    {
        fprintf(stderr,"Texture::load_dds(): unknown format of \"%s\" file\n",name);
        fclose(file);
        return NULL;
    }
    if(format == DDS_DXT2 || format == DDS_DXT4)
    {
        fprintf(stderr,"Texture::load_dds(): DXT2 or DXT4 is not supported in \"%s\" file\n",name);
        fclose(file);
        return NULL;
    }
    unsigned char *data = new unsigned char[width * height * 4];
    if(format == DDS_RGB)
    {
        unsigned char *buf = new unsigned char[width * height * 3];
        fread(buf,width * height * 3,1,file);
        unsigned char *src = buf;
        unsigned char *dest = data;
        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                *dest++ = *src++;
                *dest++ = *src++;
                *dest++ = *src++;
                *dest++ = 255;
            }
        }
        delete [] buf;
    }
    else if(format == DDS_RGBA)
    {
        unsigned char *buf = new unsigned char[width * height * 4];
        fread(buf,width * height * 4,1,file);
        unsigned char *src = buf;
        unsigned char *dest = data;
        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                *dest++ = *src++;
                *dest++ = *src++;
                *dest++ = *src++;
                *dest++ = *src++;
            }
        }
        delete [] buf;
    }
    else
    {
        unsigned char *buf = new unsigned char[width * height];
        unsigned char *src = buf;
        fread(buf,width * height,1,file);
        for(int y = 0; y < height; y += 4)
        {
            for(int x = 0; x < width; x += 4)
            {
                unsigned long long alpha = 0;
                unsigned int a0 = 0;
                unsigned int a1 = 0;
                if(format == DDS_DXT3)
                {
                    alpha = *(unsigned long long*)src;
                    src += 8;
                }
                else if(format == DDS_DXT5)
                {
                    alpha = (*(unsigned long long*)src) >> 16;
                    a0 = src[0];
                    a1 = src[1];
                    src += 8;
                }
                unsigned int c0 = *(unsigned short*)(src + 0);
                unsigned int c1 = *(unsigned short*)(src + 2);
                src += 4;
                dds_color color[4];
                color[0].r = ((c0 >> 11) & 0x1f) << 3;
                color[0].g = ((c0 >> 5) & 0x3f) << 2;
                color[0].b = (c0 & 0x1f) << 3;
                color[1].r = ((c1 >> 11) & 0x1f) << 3;
                color[1].g = ((c1 >> 5) & 0x3f) << 2;
                color[1].b = (c1 & 0x1f) << 3;
                if(c0 > c1)
                {
                    color[2].r = (color[0].r * 2 + color[1].r) / 3;
                    color[2].g = (color[0].g * 2 + color[1].g) / 3;
                    color[2].b = (color[0].b * 2 + color[1].b) / 3;
                    color[3].r = (color[0].r + color[1].r * 2) / 3;
                    color[3].g = (color[0].g + color[1].g * 2) / 3;
                    color[3].b = (color[0].b + color[1].b * 2) / 3;
                }
                else
                {
                    color[2].r = (color[0].r + color[1].r) / 2;
                    color[2].g = (color[0].g + color[1].g) / 2;
                    color[2].b = (color[0].b + color[1].b) / 2;
                    color[3].r = 0;
                    color[3].g = 0;
                    color[3].b = 0;
                }
                for(int i = 0; i < 4; i++)
                {
                    unsigned int index = *src++;
                    unsigned char *dest = data + (width * (y + i) + x) * 4;
                    for(int j = 0; j < 4; j++)
                    {
                        *dest++ = color[index & 0x03].r;
                        *dest++ = color[index & 0x03].g;
                        *dest++ = color[index & 0x03].b;
                        if(format == DDS_DXT1)
                        {
                            *dest++ = ((index & 0x03) == 3 && c0 <= c1) ? 0 : 255;
                        }
                        else if(format == DDS_DXT3)
                        {
                            *dest++ = (unsigned char)((alpha & 0x0f) << 4);
                            alpha >>= 4;
                        }
                        else if(format == DDS_DXT5)
                        {
                            unsigned int a = (unsigned int)(alpha & 0x07);
                            if(a == 0) *dest++ = a0;
                            else if(a == 1) *dest++ = a1;
                            else if(a0 > a1) *dest++ = ((8 - a) * a0 + (a - 1) * a1) / 7;
                            else if(a > 5) *dest++ = (a == 6) ? 0 : 255;
                            else *dest++ = ((6 - a) * a0 + (a - 1) * a1) / 5;
                            alpha >>= 3;
                        }
                        else *dest++ = 255;
                        index >>= 2;
                    }
                }
            }
        }
        delete [] buf;
    }
    fclose(file);
    return data;
}

/*****************************************************************************/
/*                                                                           */
/* converters                                                                */
/*                                                                           */
/*****************************************************************************/

unsigned char *Texture::rgba2rgb(unsigned char *data,int width,int height)
{
    unsigned char *dest = new unsigned char[width * height * 3];
    unsigned char *d = dest;
    unsigned char *s = data;
    for(int i = 0; i < width * height; i++)
    {
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        s++;
    }
    delete [] data;
    return dest;
}

unsigned char *Texture::rgba2luminance(unsigned char *data,int width,int height)
{
    unsigned char *dest = new unsigned char[width * height];
    unsigned char *d = dest;
    unsigned char *s = data;
    for(int i = 0; i < width * height; i++)
    {
        *d++ = *s++;
        s += 3;
    }
    delete [] data;
    return dest;
}

unsigned char *Texture::rgba2luminance_alpha(unsigned char *data,int width,int height)
{
    unsigned char *dest = new unsigned char[width * height * 2];
    unsigned char *d = dest;
    unsigned char *s = data;
    for(int i = 0; i < width * height; i++)
    {
        *d++ = *s++;
        s += 2;
        *d++ = *s++;
    }
    delete [] data;
    return dest;
}

/*****************************************************************************/
/*                                                                           */
/* 3d textures                                                               */
/*                                                                           */
/*****************************************************************************/

unsigned char *Texture::load_3d(const char *name,int &width,int &height,int &depth,int &format)
{
    FILE *file = fopen(name,"rb");
    if(!file)
    {
        fprintf(stderr,"Texture::load_3d(): error open \"%s\" file\n",name);
        return NULL;
    }
    int magic;
    fread(&magic,sizeof(int),1,file);
    if(magic != ('3' | 'D' << 8 | 'T' << 16 | 'X' << 24))
    {
        fprintf(stderr,"Texture::load_3d(): wrong magic in \"%s\" file\n",name);
        fclose(file);
        return NULL;
    }
    fread(&width,sizeof(int),1,file);
    fread(&height,sizeof(int),1,file);
    fread(&depth,sizeof(int),1,file);
    fread(&format,sizeof(int),1,file);
    int size = width * height * depth;
    if(format == LUMINANCE) size *= 1;
    else if(format == LUMINANCE_ALPHA) size *= 2;
    else if(format == RGB) size *= 3;
    else if(format == RGBA) size *= 4;
    else
    {
        fprintf(stderr,"Texture::load_3d(): unknown format of \"%s\" file\n",name);
        fclose(file);
        return NULL;
    }
    unsigned char *data = new unsigned char[size];
    fread(data,sizeof(unsigned char),size,file);
    fclose(file);
    return data;
}

/*
 */
int Texture::save_3d(const char *name,const unsigned char *data,int width,int height,int depth,int format)
{
    FILE *file = fopen(name,"wb");
    if(!file)
    {
        fprintf(stderr,"Texture::save_3d(): error create \"%s\" file\n",name);
        return 0;
    }
    int magic = ('3' | 'D' << 8 | 'T' << 16 | 'X' << 24);
    fwrite(&magic,sizeof(int),1,file);
    fwrite(&width,sizeof(int),1,file);
    fwrite(&height,sizeof(int),1,file);
    fwrite(&depth,sizeof(int),1,file);
    fwrite(&format,sizeof(int),1,file);
    int size = width * height * depth;
    if(format == LUMINANCE) size *= 1;
    else if(format == LUMINANCE_ALPHA) size *= 2;
    else if(format == RGB) size *= 3;
    else if(format == RGBA) size *= 4;
    else fprintf(stderr,"Texture::save_3d(): unknown format\n");
    fwrite(data,sizeof(unsigned char),size,file);
    fclose(file);
    return 1;
}
