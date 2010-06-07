/***************************************************************************
 *   Copyright (C) 2007  Antony Martin                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   as published by the Free Software Foundation; either version 2        *
 *   of the License, or (at your option) any later version.                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA          *
 ***************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/glut.h>


#include "loadprogram.h"





/* charge le code source d'un shader */
static char* LoadSource(const char *filename)
{
    
    char *src = NULL;   /* code source de notre shader */
    FILE *fp = NULL;    /* fichier */
    long size;          /* taille du fichier */
    long i;             /* compteur */
    
    
    /* on ouvre le fichier */
    fp = fopen(filename, "r");
    /* on verifie si l'ouverture a echoue */
    if(fp == NULL)
    {
        fprintf(stderr, "impossible d'ouvrir le fichier '%s'\n", filename);
        return NULL;
    }
    
    /* on recupere la longueur du fichier */
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    
    /* on se replace au debut du fichier */
    rewind(fp);
    
    /* on alloue de la memoire pour y placer notre code source */
    src = (char*)malloc(size+1); /* +1 pour le caractere de fin de chaine '\0' */
    if(src == NULL)
    {
        fclose(fp);
        fprintf(stderr, "erreur d'allocation de memoire!\n");
        return NULL;
    }
    
    /* lecture du fichier */
    for(i=0; i<size; i++)
        src[i] = fgetc(fp);
    
    /* on place le dernier caractere a '\0' */
    src[size] = '\0';
    
    fclose(fp);
    
    return src;
}


GLuint LoadProgram(const char *vsname, const char *psname)
{
    
    GLuint prog = 0;
    GLuint vs = 0, ps = 0;
    GLint link_status = GL_TRUE;
    GLint logsize = 0;
    char *log = NULL;
    
    
    /* verification des arguments */
    if(vsname == NULL && psname == NULL)
    {
        fprintf(stderr, "creation d'un program demande, mais aucuns "
                        "noms de fichiers source envoye, arret.\n");
        
        return 0;
    }
    
    
    /* chargement des shaders */
    if(vsname != NULL)
    {
        vs = LoadShader(GL_VERTEX_SHADER, vsname);
        if(vs == 0)
            return 0;
    }
    if(psname != NULL)
    {
        ps = LoadShader(GL_FRAGMENT_SHADER, psname);
        if(ps == 0)
        {
            if(glIsShader(vs))
                glDeleteShader(vs);
            return 0;
        }
    }
    
    
    
    /* creation du program */
    prog = glCreateProgram();
    
    /* on envoie nos shaders a notre program */
    if(vs)
        glAttachShader(prog, vs);
    if(ps)
        glAttachShader(prog, ps);
    
    /* on lie le tout */
    glLinkProgram(prog);
    
    /* on verifie que tout s'est bien passe */
    glGetProgramiv(prog, GL_LINK_STATUS, &link_status);
    if(link_status != GL_TRUE)
    {
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logsize);
        log = (char*)malloc(logsize + 1);
        if(log == NULL)
        {
            glDeleteProgram(prog);
            glDeleteShader(vs);
            glDeleteShader(ps);
            
            fprintf(stderr, "impossible d'allouer de la memoire!\n");
            return 0;
        }
        memset(log, '\0', logsize + 1);
        glGetProgramInfoLog(prog, logsize, &logsize, log);
        
        fprintf(stderr, "impossible de lier le program :\n%s", log);
        
        free(log);
        glDeleteProgram(prog);
        glDeleteShader(vs);
        glDeleteShader(ps);
        
        return 0;
    }
    
    /* les shaders sont dans le program maintenant, on en a plus besoin */
    glDeleteShader(vs);
    glDeleteShader(ps);
    
    return prog;
}


GLuint LoadShader(GLenum type, const char *filename)
{


        
    GLuint shader = 0;
    GLsizei logsize = 0;
    GLint compile_status = GL_TRUE;
    char *log = NULL;
    char *src = NULL;
    
    
    /* creation d'un shader de sommet */
    shader = glCreateShader(type);
    if(shader == 0)
    {
        fprintf(stderr, "impossible de creer le shader\n");
        return 0;
    }
    
    
    
    /* chargement du code source */
    src = LoadSource(filename);
    if(src == NULL)
    {
        /* theoriquement, la fonction LoadSource a deja affiche un message
           d'erreur, nous nous contenterons de supprimer notre shader
           et de retourner 0 */
        
        glDeleteShader(shader);
        return 0;
    }
    
    /* assignation du code source */
    glShaderSource(shader, 1, (const GLchar**)&src, NULL);
    
    
    /* compilation du shader */
    glCompileShader(shader);
    
    /* liberation de la memoire du code source */
    free(src);
    src = NULL;
    
    /* verification du succes de la compilation */
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if(compile_status != GL_TRUE)
    {
        /* erreur a la compilation recuperation du log d'erreur */
        
        /* on recupere la taille du message d'erreur */
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
        
        /* on alloue un esapce memoire dans lequel OpenGL ecrira le message */
        log = (char*)malloc(logsize + 1);
        if(log == NULL)
        {
            fprintf(stderr, "impossible d'allouer de la memoire!\n");
            return 0;
        }
        /* initialisation du contenu */
        memset(log, '\0', logsize + 1);
        
        glGetShaderInfoLog(shader, logsize, &logsize, log);
        fprintf(stderr, "impossible de compiler le shader '%s' :\n%s",
                filename, log);
        
        /* ne pas oublier de liberer la memoire et notre shader */
        free(log);
        glDeleteShader(shader);
        
        return 0;
    }
    
    return shader;
}
