/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : render.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Render system module.
 *               Main implementation module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tse.h"

#pragma comment(lib, "opengl32")

/* Type constructor function.
 * ARGUMENTS:
 *   - window handle reference:
 *       HWND &hNewWndRef
 */
tse::render::render( HWND &hNewWndRef ) : hWndRef(hNewWndRef), IsRenderInit(FALSE),
  primitive_manager(*this), shader_manager(*this), material_manager(*this), 
  buffer_manager(*this), texture_manager(*this), font_manager(*this)
{
} /* End of 'tse::render::render' function */

/* Type destructor function */
tse::render::~render( VOID )
{
} /* End of 'tse::render::~render' function */

/* Initialize OpenGL function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::render::Init( VOID )
{
  PIXELFORMATDESCRIPTOR pfd = {0};
  HGLRC hRC;
  INT 
    PixelAttribs[] =
    {
      WGL_DRAW_TO_WINDOW_ARB, TRUE,                    /* Window draw support */
      WGL_SUPPORT_OPENGL_ARB, TRUE,                    /* OpenGL support */
      WGL_DOUBLE_BUFFER_ARB, TRUE,                     /* Double buffering support */
      WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,           /* Pixel format */
      WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB, /* GPU max acceleration */
      WGL_COLOR_BITS_ARB, 32,                          /* Color buffer bits count */
      WGL_DEPTH_BITS_ARB, 32,                          /* Depth buffer bits count */
      0                                                /* End of parameters */
    },
    ContextAttribs[] =
    {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 4, /* Version */
      WGL_CONTEXT_MINOR_VERSION_ARB, 6, /*   4.6   */
      WGL_CONTEXT_PROFILE_MASK_ARB,     /* Compatibility handle */
      WGL_CONTEXT_CORE_PROFILE_BIT_ARB, /* COMPATIBILITY / CORE */
      0
    };

  if (ContextAttribs[5] == WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB)
  {
    tse::logger::Warn("WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB enabled!");
    tse::logger::Warn("Consider switching to WGL_CONTEXT_CORE_PROFILE_BIT_ARB");
  }

  /* Prepare suitable device context */
  hDC = GetDC(hWndRef);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  INT i = ChoosePixelFormat(hDC, &pfd);
  DescribePixelFormat(hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(hDC, i, &pfd);
  tse::logger::LogLn("Pixel format: " + std::to_string(i));

  /* OpenGL init: rendering context setup */
  hGLRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hGLRC);

  /* Glew initialization */
  if (glewInit() != GLEW_OK || !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    tse::logger::Err("GLEW was not initialized!");
    return;
  }

  /* Enable a new OpenGL profile support */
  UINT nums;
  wglChoosePixelFormatARB(hDC, PixelAttribs, NULL, 1, &i, &nums);
  hRC = wglCreateContextAttribsARB(hDC, NULL, ContextAttribs);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hGLRC);
  hGLRC = hRC;
  wglMakeCurrent(hDC, hGLRC);

  wglSwapIntervalEXT(0);

  tse::logger::Sys("OpenGL initialized");
  tse::logger::LogLn((CHAR *)glGetString(GL_VERSION));
  tse::logger::LogLn((CHAR *)glGetString(GL_VENDOR));
  tse::logger::LogLn((CHAR *)glGetString(GL_RENDERER));

#ifndef NDEBUG
  OutputDebugString((LPSTR)glGetString(GL_VERSION));
  OutputDebugString("\n");
  OutputDebugString((LPSTR)glGetString(GL_VENDOR));
  OutputDebugString("\n");
  OutputDebugString((LPSTR)glGetString(GL_RENDERER));
  OutputDebugString("\n");
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback((GLDEBUGPROC)glDebugOutput, NULL);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif /* __NDEBUG_ */

  /* Default OpenGL render parameters setup */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  tse::logger::Sys("Render system initialized");

  /* Create buffers */
  BufCam = buffer_manager::BufCreate<BUF_CAM>(0, nullptr);
  BufSync = buffer_manager::BufCreate<BUF_SYNC>(1, nullptr);
  BufPrim = buffer_manager::BufCreate<BUF_PRIM>(2, nullptr);

  anim::Get().ShdCreate("default");
  anim::Get().MtlCreate("default");

  IsRenderInit = TRUE;
} /* End of 'tse::render::Init' function */

/* Deinitialize OpenGL function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::render::Close( VOID )
{
  IsRenderInit = FALSE;
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hGLRC);
  tse::logger::Sys("Render system closed");
}  /* End of 'tse::render::Close' function */

/* Resize frame buffer function.
 * ARGUMENTS:
 *   - new size:
 *       INT NewW, NewH;
 * RETURNS: None.
 */
VOID tse::render::Resize( INT NewW, INT NewH )
{
  Cam.Resize(NewW, NewH);
  glViewport(0, 0, NewW, NewH);
} /* End of 'tse::render::Resize' function */

/* On frame render start function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::render::FrameStart( VOID )
{
  FLT clear_color[4] {0.30, 0.47, 0.8, 1}, clear_depth = 1;
  glClearBufferfv(GL_COLOR, 0, clear_color);
  glClearBufferfv(GL_DEPTH, 0, &clear_depth);

  shader_manager::Update();
 
  BUF_CAM bc
  {
    Cam.View,
    Cam.Proj,
    Cam.VP,
    vec4(Cam.Loc, Cam.FrameW),
    vec4(Cam.Dir, Cam.ProjDist),
    vec4(Cam.Right, Cam.Wp),
    vec4(Cam.Up, Cam.Hp),
    vec4(Cam.At, Cam.FrameH)
  };
  BufCam->Update(&bc);

  anim *ani = anim::GetPtr();
  BUF_SYNC bs
  {
    vec4(ani->GlobalTime, ani->GlobalDeltaTime, ani->Time, ani->DeltaTime)
  };
  BufSync->Update(&bs);
} /* End of 'tse::render::FrameStart' function */

/* On frame render end function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::render::FrameEnd( VOID )
{
  glFinish();
  SwapBuffers(hDC);
} /* End of 'tse::render::FrameEnd' function */

/* Primitive draw function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       prim *Pr;
 *   - transformation matrix:
 *       const matr &World;
 * RETURNS: None.
 */
VOID tse::render::Draw( const prim *Pr, const matr &World )
{
  UINT type =
    Pr->Type == prim_type::TRIMESH ? GL_TRIANGLES :
    Pr->Type == prim_type::STRIP ? GL_TRIANGLE_STRIP :
    Pr->Type == prim_type::POINTS ? GL_POINTS :
    Pr->Type == prim_type::LINES ? GL_LINES :
    GL_POINTS;
  matr
    w = World * Pr->Transform,
    wvp = w * Cam.VP,
    invw = w.Inverse().Transpose();
 
  Pr->UpdateVA();
 
  BUF_PRIM bp =
  {
    wvp, w, invw, vec4(0, 0, 0, 0), {}
  };

  material *Mtl = Pr->Mtl;
  if (Mtl == nullptr)
    Mtl = material_manager::Find("default");
  Mtl->Apply();
 
  BufPrim->Update(&bp);

  glBindVertexArray(Pr->VA);
  if (Pr->IBuf == 0)
    glDrawArrays(type, 0, Pr->NumOfElements);
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(type, Pr->NumOfElements, GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);
  glUseProgram(0);
} /* End of 'tse::render::Draw' function */

/* Model draw function.
 * ARGUMENTS:
 *   - model pointer:
 *       model *Mdl;
 *   - transformation matrix:
 *       const matr &World;
 * RETURNS: None.
 */
VOID tse::render::Draw( const model *Mdl, const matr &World )
{
  for (INT i = 0; i < Mdl->Prims.size(); i++)
    if (Mdl->Prims[i]->Mtl->Trans == 1)
      Draw(Mdl->Prims[i], World);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (INT i = 0; i < Mdl->Prims.size(); i++)
    if (Mdl->Prims[i]->Mtl->Trans != 1)
      Draw(Mdl->Prims[i], World);
  glCullFace(GL_BACK);
  for (INT i = 0; i < Mdl->Prims.size(); i++)
    if (Mdl->Prims[i]->Mtl->Trans != 1)
      Draw(Mdl->Prims[i], World);
  glDisable(GL_CULL_FACE);
} /* End of 'tse::render::Draw' function */

/* END OF 'render.cpp' FILE */
