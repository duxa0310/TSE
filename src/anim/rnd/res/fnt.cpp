/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : fnt.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Fonts implementation module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 04.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tse.h"

/***
 * FONT FUNCTIONS
 ***/

/* Class default constructor function */
tse::font::font( VOID )
{
} /* End of 'tse::font::font' function */

/* Class constructor function.
 * ARGUMENTS:
 *   - font file name:
 *       const std::string &FileName;
 */
tse::font::font( const std::string &FileName )
{
  Name = FileName;
} /* End of 'tse::font::font' function */

/* Class destructor function */
tse::font::~font( VOID )
{
} /* End of 'tse::font::~font' function */

/* Class constructor function.
 * ARGUMENTS:
 *   - font file name:
 *       const std::string &FileName;
 */
tse::font & tse::font::Load( const std::string &FileName )
{
  std::fstream f(FileName, std::fstream::in | std::fstream::binary);
  if (!f.is_open())
    return *this;

  f.seekg(0, std::fstream::end);
  SIZE_T flen = f.tellg();
  f.seekg(0, std::fstream::beg);

  std::vector<BYTE> mem;
  mem.resize(flen);
  f.read((CHAR *)mem.data(), flen);
  BYTE *ptr = mem.data();

  rdr rd(ptr);

  /* Load header */
  DWORD sign;
  rd(&sign);
  if (sign != *(DWORD *)"G3DF")
    return *this;

  /* Load storage info */
  rd(&LineH);
  rd(&BaseH);
  rd(AdvanceX, 256);

  /* Load vertices */
  tse::vertex_std4 *V;
  DWORD W, H;
  rd(&V, 256 * 4);

  /* Load texture */
  rd(&W);
  rd(&H);
  DWORD *Img = new DWORD[W * H];
  rd(Img, W * H);

  /* Create primitives */
  Mtl = anim::Get().MtlCreate(FileName);
  Mtl->Tex[0] = anim::Get().TexCreate(FileName, W, H, 4,
    reinterpret_cast<BYTE *>(Img));
  delete[] Img;
  Mtl->Shd = anim::Get().shader_manager::Find("font");
  if (Mtl->Shd == nullptr)
    Mtl->Shd = anim::Get().ShdCreate("font");

  for (INT i = 0; i < 256; i++)
    Chars[i] = anim::Get().PrimCreate(Mtl, prim_type::STRIP,
      std::span(V + i * 4, 4), {});

  tse::logger::Info("FONT loaded: " + FileName);
  return *this;
} /* End of 'tse::font::Load' function */

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
VOID tse::font::Draw( const std::string &Str, vec3 Pos, FLT Size )
{
  FLT startx = Pos.X;

  for (INT i = 0; i < Str.length(); i++)
  {
    UCHAR chr = Str[i];
    if (chr == '\n')
    {
      Pos.X = startx;
      Pos.Y -= Size;
    }
    else
    {
      anim::Get().Draw(Chars[chr], matr::Scale(Size, Size, 1) * matr::Translate(Pos));
      Pos.X += AdvanceX[chr] * Size;
    }
  }
} /* End of 'tse::font::Draw' function */

/* Free font function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::font::Free( VOID )
{
} /* End of 'tse::font::Free' function */

/***
 * FONT MANAGER FUNCTIONS
 ***/

/* Create font function.
 * ARGUMENTS:
 *   - font file name:
 *       const std::string &Fileame;
 * RETURNS:
 *   (font *) created font interface.
 */
tse::font * tse::font_manager::FntCreate( const std::string &FileName )
{
  return resource_manager::Add(font(FileName).Load(FileName));
} /* End of 'tse::font_manager::FntCreate' function */

/* Class constructor function.
 * ARGUMENTS:
 *   - render instance reference:
 *       render &Rnd;
 */
tse::font_manager::font_manager( render &Rnd ) : resource_manager(Rnd)
{
} /* End of 'tse::font_manager::font_manager' function */

/* Class destructor function */
tse::font_manager::~font_manager( VOID )
{
} /* End of 'tse::font_manager::~font_manager' function */

/* END OF 'fnt.cpp' FILE */
