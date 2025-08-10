/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : tex.h
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Textures declaration module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tex_h_
#define __tex_h_

#include "resources.h"

/* Main program namespace */
namespace tse
{
  class render;
 
  /* Texture handle class */
  class texture : public resource
  {
    friend class render;

  public:    
    std::string Name; // Texture name
    INT W = 0, H = 0; // Image size
    UINT TexId = 0;   // OpenGL texture Id
 
    /* Default class constructor function */
    texture( VOID );
 
    /* Class constructor function.
     * ARGUMENTS:
     *   - texture file name:
     *       const std::string &FileName;
     */
    texture( const std::string &FileName );

    /* Texture create function.
     * ARGUMENTS:
     *   - texture name:
     *       const std::string &NewName;
     *   - image size:
     *       INT NewW, NewH;
     *   - components count:
     *       INT BytesPerPixel;
     *   - image pixel data:
     *       VOID *Pixels;
     * RETURNS:
     *   (texture &) self reference.
     */
    texture & Create( const std::string &NewName, INT NewW, INT NewH,
                      INT BytesPerPixel, BYTE *Pixels, BOOL IsMips = TRUE );
 
    /* Texture create function.
     * ARGUMENTS:
     *   - texture file name:
     *       const std::string &FileName;
     * RETURNS:
     *   (texture &) self reference.
     */
    texture & Create( const std::string &FileName );
 
    /* Apply texture function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Apply( VOID );

    /* Free texture function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

  }; /* End of 'texture' class */
 
  /* Texture manager class */
  class texture_manager : public resource_manager<texture, std::string>
  {
  public:
    /* Create tetxure function.
     * ARGUMENTS:
     *   - texture file name:
     *       const std::string &FileName;
     * RETURNS:
     *   (texture *) created texture interface.
     */
    texture * TexCreate( const std::string &FileName );

    /* Create texture function.
     * ARGUMENTS:
     *   - texture name:
     *       const std::string &NewName;
     *   - image size:
     *       INT NewW, NewH;
     *   - components count:
     *       INT BytesPerPixel;
     *   - image pixel data:
     *       VOID *Pixels;
     * RETURNS:
     *   (texture *) created texture interface.
     */
    texture * TexCreate( const std::string &NewName, INT NewW, INT NewH,
                         INT BytesPerPixel, BYTE *Pixels, BOOL IsMips = TRUE );
 
    /* Class constructor function.
     * ARGUMENTS:
     *   - render instance reference:
     *       render &Rnd;
     */
    texture_manager( render &Rnd );
 
    /* Class destructor function */
    ~texture_manager( VOID );

  }; /* End of 'texture_manager' class */

} /* end of 'tse' namespace */

#endif /* __tex_h_ */

/* END OF 'tex.h' FILE */
