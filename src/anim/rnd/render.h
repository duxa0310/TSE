/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : render.h
 * PURPOSE     : Tough Space Exploration project.
 *               Render system module.
 *               Common definitions module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __render_h_
#define __render_h_

/* WinAPI and OpenGL depended files */
#define GLEW_STATIC
#include <glew.h>
#include <wglew.h>
#include <gl/wglext.h>

#include "res/shd.h"
#include "res/buf.h"
#include "res/tex.h"
#include "res/mtl.h"
#include "res/prim.h"
#include "res/fnt.h"

/* Main program namespace */
namespace tse
{
  /* Standard 4-component vertex representation structure */
  struct vertex_std4
  {
    vec3 P; // Position
    vec2 T; // Texture coordinate
    vec3 N; // Normal vector
    vec4 C; // Color
  }; /* End of 'vertex_std4' struct */

  /* Render representation class */
  class render : public primitive_manager, public shader_manager,
    public material_manager, public buffer_manager, public texture_manager,
    public font_manager
  {
  private:
    HWND &hWndRef;     // Window handle reference
    HDC hDC;           // Device context handle
    HGLRC hGLRC;       // OpenGL rendering context handle
    BOOL IsRenderInit; // Initial flag

  public:
    camera Cam;        // Render camera

    /* Camera buffer transfer structure */
    struct BUF_CAM
    {
      matr::matr_data MatrView; // Camera view matrix
      matr::matr_data MatrProj; // Camera projection matrix
      matr::matr_data MatrVP;   // Camera view by projection matrices production
      vec4 CamLocFrameW;        // Location + frame width
      vec4 CamDirProjDist;      // Direction + project distance
      vec4 CamRightWp;          // Right + project plane width
      vec4 CamUpHp;             // Up + project plane height
      vec4 CamAtFrameH;         // Look at point + frame height
    }; /* End of 'BUF_CAM' structure */
 
    /* Time buffer transfer structure */
    struct BUF_SYNC
    {
      // Global time + global delta time + time + delta time
      vec4 SyncGlobalTimeGlobalDeltaTimeTimeDeltaTime;
    }; /* End of 'BUF_SYNC' structure */
 
    /* Primitive buffer transfer structure */
    struct BUF_PRIM
    {
      matr::matr_data MatrWVP;       // World by view by projection matrices production
      matr::matr_data MatrW;         // World matrix
      matr::matr_data MatrWInvTrans; // Inverse transpose world matrix
      vec4 RndIsWireIsAny;           // Wireframe flag + not used X3
      INT TextureFlags[8];           // Texture usage flags
    }; /* End of 'BUF_PRIM' structure */
 
    /* OpenGL data buffers */
    buffer
      *BufCam,  // Camera buffer
      *BufSync, // Timer buffer
      *BufPrim; // Primitive buffer

    /* Type constructor function.
     * ARGUMENTS:
     *   - window handle reference:
     *       HWND &hNewWndRef
     */
    render( HWND &hNewWndRef );
 
    /* Type destructor function */
    virtual ~render( VOID );
 
    /* Initialize OpenGL function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Init( VOID );
 
    /* Deinitialize OpenGL function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID );
 
    /* Resize farme buffer function.
     * ARGUMENTS:
     *   - new size:
     *       INT NewW, NewH;
     * RETURNS: None.
     */
    VOID Resize( INT NewW, INT NewH );
 
    /* On frame render start function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FrameStart( VOID );
 
    /* On frame render end function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FrameEnd( VOID );

    /* Primitive draw function.
     * ARGUMENTS:
     *   - primitive pointer:
     *       prim *Pr;
     *   - transformation matrix:
     *       const matr &World;
     * RETURNS: None.
     */
    VOID Draw( const prim *Pr, const matr &World = matr::Identity() );

    /* Model draw function.
     * ARGUMENTS:
     *   - model pointer:
     *       model *Mdl;
     *   - transformation matrix:
     *       const matr &World;
     * RETURNS: None.
     */
    VOID Draw( const model *Mdl, const matr &World = matr::Identity() );

  private:
    /* Debug output function.
      * ARGUMENTS:
      *   - source APi or device:
      *       UINT Source;
      *   - error type:
      *       UINT Type;
      *   - error message id:
      *       UINT Id, 
      *   - message severity:
      *       UINT severity, 
      *   - message text length:
      *       INT Length, 
      *   - message text:
      *       CHAR *Message, 
      *   - user addon parameters pointer:
      *       VOID *UserParam;
      * RETURNS: None.
      */
    static VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                                        INT Length, const CHAR *Message, const VOID *UserParam );

  }; /* End of 'render' class */

} /* end of 'tse' namespace */

#endif /* __render_h_ */

/* END OF 'render.h' FILE */
