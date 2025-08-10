/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mtl.h
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Materials declaration module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mtl_h_
#define __mtl_h_

#include "resources.h"

/* Main program namespace */
namespace tse
{
  class render;
 
  /* Material handle class */
  class material : public resource
  {
    friend class render;
 
  private:
    /* Material buffer data type */
    struct BUF_MTL
    {
      vec4 Ka;         // Ambient color
      vec4 KdTrans;    // Diffuse color + Transparency factor
      vec4 KsPh;       // Specular color + Phong coefficient
      INT TexFlags[8]; // Textures flags
    }; /* End of 'BUF_MTL' structure */
 
  public:
    std::string Name; // Material name
 
    vec3 Ka, Kd, Ks;     // Illumination coefficients (anbient, diffuse, specular)
    FLT Ph, Trans;       // Phong coefficient and transparency value

    shader *Shd {};      // Shader pointer
    texture *Tex[8] {};  // Texture pointers
    
    buffer *BufferMtl {}; // Material data buffer
 
    /* Class default constructor */
    material( VOID );
 
    /* Class destructor */
    ~material( VOID );
 
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
    material & Create( const std::string &NewName,
                       const vec3 &NewKa = vec3(0.1),
                       const vec3 &NewKd = vec3(0.9),
                       const vec3 &NewKs = vec3(0.0),
                       FLT NewPh = 30, FLT NewTrans = 1,
                       shader *NewShd = nullptr,
                       std::initializer_list<texture *> Textures = {} );
 
    /* Material destroy function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );
 
    /* Apply material function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (shader *) shader interface;
     */
    shader * Apply( VOID );
 
    /* Update material buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Update( VOID );

  }; /* End of 'material' class */
 
  /* Material manager */
  class material_manager : public resource_manager<material, std::string>
  {
  public:
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
    material * MtlCreate( const std::string &NewName,
                          const vec3 &NewKa = vec3(0.1),
                          const vec3 &NewKd = vec3(0.9),
                          const vec3 &NewKs = vec3(0.0),
                          FLT NewPh = 30, FLT NewTrans = 1,
                          shader *NewShd = nullptr,
                          std::initializer_list<texture *> Textures = {} );
 
    /* Class constructor.
     * ARGUMENTS:
     *   - render instance reference:
     *       render &Rnd;
     */
    material_manager( render &Rnd );
 
    /* Class destructor */
    ~material_manager( VOID );

  }; /* End of 'material_manager' class */

} /* end of 'tse' namespace */

#endif /* __mtl_h_ */

/* END OF 'mtl.h' FILE */
