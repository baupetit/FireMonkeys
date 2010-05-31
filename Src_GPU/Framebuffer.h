#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/*!*****************************************************************************
*
*       \file       :      Framebuffer.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      31 mai 2010
*       \version    :      0.1
*
*******************************************************************************/



#define PING 1
#define PONG 2
    
class Framebuffer{
private:
    /** Identifiant du premier buffer */
    GLuint buffer1;
    
    /** Identifiant du deuxième buffer */
    GLuint buffer2;
    
    /** Texture associée au buffer 1 */
    GLuint texture_buf1;
    
    /** Texture associée au buffer 2 */
    GLuint texture_buf2;
    
    /** Buffer/Texture courante */
    int pingpong;
    
    
public:
    /** Constructeur par défaut */
    Framebuffer();
    
    /** Destructeur */
    ~Framebuffer();
    
    /** Change le buffer actif */
    void Swap_Active_Buffer();
    
    /** Charge le buffer courant */
    void Bind_Buffer_Courant();
    
    /** Decharge le buffer courant */
    void Unbind_Buffer();
    
    /** Retourne l'id de la texture 1*/
    GLuint Get_texture_buf1(){
        return texture_buf1;
    }
    
    /** Retourne l'id de la texture 2*/
    GLuint Get_texture_buf2(){
        return texture_buf2;
    }
    
    /** Retourne l'id de la texture courante */
    GLuint Get_texture_courante();
    
    /** Retourne l'id du buffer courant */
    GLuint Get_buffer_courant();
    
    /** Initialise la texture id avec le tableau donné */
    void Initialiser_texture_buf(const int id_texture, const float *tab, const int taille);
    
};
#endif
