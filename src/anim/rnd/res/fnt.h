/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : buf.h
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Fonts declaration module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 04.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __fnt_h_
#define __fnt_h_

#include "resources.h"

/* Main program namespace */
namespace tse
{
  class render;
 
  /* Font handle class */
  class font : public resource
  {
    friend class render;

  private:
    DWORD LineH, BaseH; // Font line height and base line height in pixels
    FLT AdvanceX[256];  // Every letter shift right value (0 if no letter present)
    prim *Chars[256];   // Font characters primitives
    material *Mtl;      // Font material

  public:
    std::string Name;   // Font name

    /* Class default constructor function */
    font( VOID );
 
    /* Class constructor function.
     * ARGUMENTS:
     *   - font file name:
     *       const std::string &FileName;
     */
    font( const std::string &FileName );
 
    /* Class destructor function */
    ~font( VOID );
 
    /* Class constructor function.
     * ARGUMENTS:
     *   - font file name:
     *       const std::string &FileName;
     */
    font & Load( const std::string &FileName );

    /* Draw font function.
     * ARGUMENTS:
     *   - font text to draw:
     *       const std::string &Str;
     *   - draw position:
     *       vec3 Pos;
     *   - font size:
     *       FLT Size;
     * RETURNS: None.
     */
    VOID Draw( const std::string &Str, vec3 Pos, FLT Size );
 
    /* Free font function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

  }; /* End of 'font' class */
 
  /* Font manager */
  class font_manager : public resource_manager<font>
  {
  public:
    /* Create font function.
     * ARGUMENTS:
     *   - font file name:
     *       const std::string &Fileame;
     * RETURNS:
     *   (font *) created font interface.
     */
    font * FntCreate( const std::string &FileName );
 
    /* Class constructor function.
     * ARGUMENTS:
     *   - render instance reference:
     *       render &Rnd;
     */
    font_manager( render &Rnd );
 
    /* Class destructor function */
    ~font_manager( VOID );

  }; /* End of 'font_manager' class */

} /* end of  'tse' namespace */

#endif /* __fnt_h_ */

/* END OF 'fnt.h' FILE */
