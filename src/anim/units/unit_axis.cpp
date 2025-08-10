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
      anim *Ani;     // Animation context pointer
      prim *Axis;    // Axis primitive (3 lines)
      BOOL DrawFlag; // Axis draw flag

    public:
      /* Type constructor function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *NewAni;
       */
      unit_sample( anim *NewAni ) : Ani(NewAni)
      {
        DrawFlag = FALSE;
        std::vector<tse::vertex_std4> V =
        {
          {{Ani->Cam.FarClip, 0, 0}, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
          {{0, 0, 0}, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
          {{0, Ani->Cam.FarClip, 0}, {0, 0}, {0, 1, 0}, {0, 1, 0, 1}},
          {{0, 0, 0}, {0, 0}, {0, 1, 0}, {0, 1, 0, 1}},
          {{0, 0, Ani->Cam.FarClip}, {0, 0}, {0, 1, 0}, {0, 0, 1, 1}},
          {{0, 0, 0}, {0, 0}, {0, 1, 0}, {0, 0, 1, 1}},
        };
        Axis = Ani->PrimCreate<tse::vertex_std4>(
          Ani->MtlCreate("Axis material", vec3(0), vec3(1), vec3(0), 0, 1,
            Ani->ShdCreate("axis")),
          prim_type::LINES, V, {});
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
        if (Ani->Keys[VK_SHIFT] && Ani->KeysClick['A'])
          DrawFlag = !DrawFlag;
      } /* End of 'Response' function */

      /* Unit render function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Render( VOID ) override
      {
        if (DrawFlag)
          Ani->Draw(Axis);
      } /* End of 'Render' function */

    }; /* End of 'unit_sample' class */

    /* Call register */
    static tse::anim::unit_register<unit_sample> _("Axis");

  } /* end of anonymous namespace */

} /* end of 'tse' namespace */

/* END OF 'unit_triangle.cpp' FILE */
