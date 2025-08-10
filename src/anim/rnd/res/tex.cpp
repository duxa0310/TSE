/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : tex.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Textures implementation module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tse.h"

/***
 * TEXTURE FUNCTIONS
 ***/

/* Default class constructor function */
tse::texture::texture( VOID )
{
} /* End of 'tse::texture::texture' function */

/* Class constructor function.
 * ARGUMENTS:
 *   - texture file name:
 *       const std::string &FileName;
 */
tse::texture::texture( const std::string &FileName ) : Name(FileName)
{
} /* End of 'tse::texture::texture' function */

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
tse::texture & tse::texture::Create( const std::string &NewName, INT NewW, INT NewH,
                                       INT BytesPerPixel, BYTE *Pixels, BOOL IsMips )
{
  W = NewW;
  H = NewH;
  /* Setup OpenGL texture */
  glGenTextures(1, &TexId);
  glBindTexture(GL_TEXTURE_2D, TexId);
  DBL mips = log(W > H ? W : H) / log(2);
  mips = (mips < 1 || !IsMips) ? 1 : mips;
  glTexStorage2D(GL_TEXTURE_2D, mips,
    BytesPerPixel == 4 ? GL_RGBA8 : BytesPerPixel == 3 ? GL_RGB8 : GL_R8, W, H);
  if (Pixels != NULL)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, 
      BytesPerPixel == 4 ? GL_BGRA : BytesPerPixel == 3 ? GL_BGR : GL_LUMINANCE,
      GL_UNSIGNED_BYTE, Pixels);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);
  tse::logger::Info("TEXTURE created: " + NewName);
  return *this;
} /* End of 'tse::texture::Create' function */

/* Texture create function.
 * ARGUMENTS:
 *   - texture file name:
 *       const std::string &FileName;
 * RETURNS:
 *   (texture &) self reference.
 */
tse::texture & tse::texture::Create( const std::string &FileName )
{
  image img(FileName);
  return Create(FileName, img.W, img.H, 4, img.RowsB[0][0], TRUE);
} /* End of 'tse::texture::Create' function */

/* Apply texture function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::texture::Apply( VOID )
{
  glBindTexture(GL_TEXTURE_2D, TexId);
} /* End of 'tse::texture::Apply' function */

/* Free texture function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::texture::Free( VOID )
{
  glDeleteTextures(1, &TexId);
} /* End of 'tse::texture::Free' function */

/***
 * TEXTURE MANAGER FUNCTIONS
 ***/

/* Create tetxure function.
 * ARGUMENTS:
 *   - texture file name:
 *       const std::string &FileName;
 * RETURNS:
 *   (texture *) created texture interface.
 */
tse::texture * tse::texture_manager::TexCreate( const std::string &FileName )
{
  return resource_manager::Add(texture(FileName).Create(FileName));
} /* End of 'tse::texture_manager::TexCreate' function */

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
tse::texture * tse::texture_manager::TexCreate( const std::string &NewName, INT NewW, INT NewH,
                                                  INT BytesPerPixel, BYTE *Pixels, BOOL IsMips )
{
  return resource_manager::Add(texture(NewName).Create(NewName, NewW, NewH, BytesPerPixel, Pixels, IsMips));
} /* End of 'tse::texture_manager::TexCreate' function */

/* Class constructor function.
 * ARGUMENTS:
 *   - render instance reference:
 *       render &Rnd;
 */
tse::texture_manager::texture_manager( render &Rnd ) : resource_manager(Rnd)
{
} /* End of 'tse::texture_manager::texture_manager' function */

/* Class destructor function */
tse::texture_manager::~texture_manager( VOID )
{
} /* End of '~tse::texture_manager::texture_manager' function */

/* END OF 'tex.cpp' FILE */
