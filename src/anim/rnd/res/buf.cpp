/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : buf.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Buffers implementation module.
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
 * BUFFER FUNCTIONS
 ***/

/* Buffer free function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::buffer::Free( VOID )
{
  glDeleteBuffers(1, &BufId);
} /* End of 'tse::buffer::Free' function */

/* Apply buffer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::buffer::Apply( VOID )
{
  if (BufId != 0)
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BindingPoint, BufId);
} /* End of 'tse::buffer::Apply' function */

/***
 * BUFFER MANAGER FUNCTIONS
 ***/

/* Class constructor function.
 * ARGUMENTS:
 *   - render instance reference:
 *       render &Rnd;
 */
tse::buffer_manager::buffer_manager( render &Rnd ) : resource_manager(Rnd)
{
} /* End of 'tse::buffer_manager::buffer_manager' function */

/* Class destructor fucntion */
tse::buffer_manager::~buffer_manager( VOID )
{
} /* End of 'tse::buffer_manager::~buffer_manager' function */

/* END OF 'buf.cpp' FILE */
