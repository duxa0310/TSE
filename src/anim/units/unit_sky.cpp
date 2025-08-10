/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : unit_sky.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Animation unit samples module.
 *               Sky unit.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7)
 * LAST UPDATE : 04.08.2025.
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
      prim *Sky; // Sky triangle primitive

    public:
      /* Type constructor function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *NewAni;
       */
      unit_sample( anim *NewAni ) : Ani(NewAni)
      {
        std::vector<tse::vertex_std4> V = {
          {{3, -1, 0}, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
          {{-1, -1, 0}, {0, 0}, {0, 1, 0}, {0, 1, 0, 1}},
          {{-1, 3, 0}, {0, 0}, {0, 1, 0}, {0, 0, 1, 1}}};
        Sky = Ani->PrimCreate<tse::vertex_std4>(
          Ani->MtlCreate("Sky material", vec3(0.1), vec3(0.8), vec3(0.5), 30, 1,
            Ani->ShdCreate("sky"), {Ani->TexCreate("bin/textures/sky0.bmp")}),
          prim_type::TRIMESH, V, {});
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
        glDepthMask(FALSE);
        Ani->Draw(Sky);
        glDepthMask(TRUE);
      } /* End of 'Render' function */

    }; /* End of 'unit_sample' class */

    /* Call register */
    static tse::anim::unit_register<unit_sample> _("Sky");

  } /* end of anonymous namespace */

} /* end of 'tse' namespace */

/* END OF 'unit_sky.cpp' FILE */
