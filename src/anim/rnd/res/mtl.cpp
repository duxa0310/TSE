/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mtl.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Materials implementation module.
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
 * MATERIAL FUNCTIONS
 ***/

/* Class default constructor */
tse::material::material( VOID )
{
} /* End of 'tse::material::material' function */

/* Class destructor */
tse::material::~material( VOID )
{
} /* End of 'tse::material::~material' function */

/* Material creation function.
 * ARGUMENTS:
 *   - material name:
 *       const std::string &NewName;
 *   - illumination coefficients (ambient. diffuse, specular):
 *       const color3 &NewKa, &NewKd, &NewKs;
 *   - Phong shininess coefficient:
 *       FLT NewPh;
 *   - transparency coefficient:
 *       FLT NewTrans;
 *   - shader pointer:
 *       shader *NewShd;
 *   - textures list:
 *       std::initializer_list<texture *> Textures;
 * RETURNS:
 *   (material &) self reference.
 */
tse::material & tse::material::Create( const std::string &NewName,
  const vec3 &NewKa, const vec3 &NewKd,
  const vec3 &NewKs, FLT NewPh, FLT NewTrans,
  shader *NewShd, std::initializer_list<texture *> Textures )
{
  Free();
  Name = NewName;
  Ka = NewKa;
  Kd = NewKd;
  Ks = NewKs;
  Ph = NewPh;
  Trans = NewTrans;
  Shd = NewShd;
 
  INT i = 0;
  for (auto t : Textures)
    Tex[i++] = t;
  for (; i < 8; i++)
    Tex[i++] = nullptr;
 
  BUF_MTL buf =
  {
    vec4(Ka),
    vec4(Kd, Trans),
    vec4(Ks, Ph)
  };
  for (i = 0; i < 8; i++)
    buf.TexFlags[i] = Tex[i] != nullptr;
  BufferMtl = anim::Get().buffer_manager::BufCreate(3, &buf);
  tse::logger::Info("MATERIAL created: " + Name);
  return *this;
} /* End of 'tse::material::Create' function */
 
/* Material destroy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::material::Free( VOID )
{
  if (BufferMtl != nullptr)
    BufferMtl->Free();
  BufferMtl = nullptr;
} /* End of 'tse::material::Free' function */

/* Apply material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (shader *) shader interface;
 */
tse::shader * tse::material::Apply( VOID )
{
  if (Shd == nullptr)
    Shd = anim::Get().shader_manager::Find("default");
  if (Shd != nullptr)
  {
    Shd->Apply();
    BufferMtl->Apply();
    for (INT t = 0; t < 8; t++)
    {
      if (Tex[t] != nullptr)
      {
        glActiveTexture(GL_TEXTURE0 + t);
        Tex[t]->Apply();
      }
    }
  }
  return Shd;
} /* End of 'tse::material::Apply' function */

/* Update material buffer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::material::Update( VOID )
{
  BUF_MTL buf =
  {
    vec4(Ka),
    vec4(Kd, Trans),
    vec4(Ks, Ph)
  };
  for (INT i = 0; i < 8; i++)
    buf.TexFlags[i] = Tex[i] != nullptr;
  BufferMtl = anim::Get().buffer_manager::BufCreate(3, &buf);
} /* End of 'tse::material::Update' function */

/***
 * MATERIAL MANAGER FUNCTIONS
 ***/

/* Create material function.
 * ARGUMENTS:
 *   - material name:
 *       const std::string &NewName;
 *   - illumination coefficients (ambient. diffuse, specular):
 *       const color3 &NewKa, &NewKd, &NewKs;
 *   - Phong shininess coefficient:
 *       FLT NewPh;
 *   - transparency coefficient:
 *       FLT NewTrans;
 *   - shader pointer:
 *       shader *NewShd;
 *   - textures list:
 *       std::initializer_list<texture *> Textures;
 * RETURNS:
 *   (material *) created material interface.
 */
tse::material * tse::material_manager::MtlCreate( const std::string &NewName,
  const vec3 &NewKa, const vec3 &NewKd,
  const vec3 &NewKs, FLT NewPh, FLT NewTrans,
  shader *NewShd, std::initializer_list<texture *> Textures )
{
  return resource_manager::Add(material().Create(NewName, NewKa, NewKd, NewKs, NewPh, NewTrans, NewShd, Textures));
} /* End of 'tse::material_manager::MtlCreate' function */

/* Class constructor.
 * ARGUMENTS:
 *   - render instance reference:
 *       render &Rnd;
 */
tse::material_manager::material_manager( render &Rnd ) : resource_manager(Rnd)
{
} /* End of 'tse::material_manager::material_manager' function */

/* Class destructor */
tse::material_manager::~material_manager( VOID )
{
} /* End of 'tse::material_manager::~material_manager' function */

/* END OF 'mtl.cpp' FILE */
