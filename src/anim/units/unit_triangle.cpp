/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : unit_triangle.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Animation unit samples module.
 *               Triangle test unit.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7)
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tse.h"

/* Main program namespace */
namespace tse
{
  /* Anonymous namespace for correct name mangling */
  namespace
  {
    /* Unit sample class representation */
    class unit_sample : public unit
    {
    private:
      anim *Ani; // Animation context pointer

    public:
      /* Type constructor function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *NewAni;
       */
      unit_sample( anim *NewAni ) : Ani(NewAni)
      {
      } /* End of ''unit_sample' function */

      /* Type destructor function */
      ~unit_sample( VOID )
      {
      } /* End of ''~unit_sample' function */

      /* Unit response function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Response( VOID ) override
      {
      } /* End of 'Response' function */

      /* Unit render function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Render( VOID ) override
      {
        static std::vector<tse::vertex_std4> V = {
          {{3, 0, 0}, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
          {{0, 0, 3}, {0, 0}, {0, 1, 0}, {0, 1, 0, 1}},
          {{0, 3, 0}, {0, 0}, {0, 1, 0}, {0, 0, 1, 1}}};
        static std::vector<INT> Ind = {};
        static prim *Pr = Ani->PrimCreate<tse::vertex_std4>(
          Ani->MtlCreate("Triangle material", vec3(0.1), vec3(0.8), vec3(0.5), 30, 1,
          Ani->shader_manager::Find("default")),
          prim_type::TRIMESH, V, Ind);
        Ani->Draw(Pr);
      } /* End of 'Render' function */

    }; /* End of 'unit_sample' class */

    /* Call register */
    static tse::anim::unit_register<unit_sample> _("Triangle");

  } /* end of anonymous namespace */

} /* end of 'tse' namespace */

/* END OF 'unit_triangle.cpp' FILE */
