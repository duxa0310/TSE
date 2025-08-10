/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : shd.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Shaders implementation module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 02.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tse.h"

/***
 * SHADER FUNCTIONS
 ***/

/* Class default constructor */
tse::shader::shader( VOID )
{
} /* End of 'tse::shader::shader' function */

/* Class constructor.
 * ARGUMENTS:
 *   - shader file name prefix:
 *       const std::string &FileNamePrefix;
 */
tse::shader::shader( const std::string &FileNamePrefix )
{
  Name = FileNamePrefix;
  Watcher.StopWatch();
  Watcher.StartWatch(anim::Path() + "bin/shaders/" + Name);
} /* End of 'tse::shader::shader' function */

/* Class destructor */
tse::shader::~shader( VOID )
{
} /* End of 'tse::shader::~shader' function */

/* Apply shader function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (UINT) shader program Id;
 */
UINT tse::shader::Apply( VOID )
{
  if (ProgId  != 0)
    glUseProgram(ProgId);
  return ProgId;
} /* End of 'tse::shader::Apply' function */

/* Update shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::shader::Update( VOID )
{
  if (Watcher.IsChanged())
  {
    Free();
    Load();
    Watcher.StartWatch("bin/shaders/" + Name);
  }
} /* End of 'tse::shader::Update' function */

/* Update shader internal data info function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::shader::UpdateInfo( VOID )
{
  // Map of OpenGL attribute types
  static std::map<INT, std::tuple<std::string, INT, BOOL, INT>> attr_types
  {
    {GL_BOOL, {"bool", 1, FALSE, 0}},
    {GL_BOOL_VEC2, {"bvec2", 1, FALSE, 0}},
    {GL_BOOL_VEC3, {"bvec3", 1, FALSE, 0}},
    {GL_BOOL_VEC4, {"bvec4", 1, FALSE, 0}},
    {GL_SAMPLER_1D, {"sampler1D", 1, FALSE, 0}},
    {GL_SAMPLER_2D, {"sampler2D", 1, FALSE, 0}},
    {GL_SAMPLER_3D, {"sampler3D", 1, FALSE, 0}},
    {GL_SAMPLER_CUBE, {"samplerCube", 1, FALSE, 0}},
    {GL_SAMPLER_1D_SHADOW, {"sampler1dShadow", 1, FALSE, 0}},
    {GL_SAMPLER_2D_SHADOW, {"sampler1dShadow", 1, FALSE, 0}},
    {GL_FLOAT, {"float", 1, TRUE, 0}},
    {GL_FLOAT_VEC2, {"vec2", 2, TRUE, 0}},
    {GL_FLOAT_VEC3, {"vec3", 3, TRUE, 0}},
    {GL_FLOAT_VEC4, {"vec4", 4, TRUE, 0}},
    {GL_FLOAT_MAT2, {"mat2", 4, TRUE, 2}},
    {GL_FLOAT_MAT3, {"mat3", 9, TRUE, 3}},
    {GL_FLOAT_MAT4, {"mat4", 16, TRUE, 4}},
    {GL_FLOAT_MAT2x3, {"mat2x3", 6, TRUE, 2}},
    {GL_FLOAT_MAT2x4, {"mat2x4", 8, TRUE, 2}},
    {GL_FLOAT_MAT3x2, {"mat3x2", 6, TRUE, 3}},
    {GL_FLOAT_MAT3x4, {"mat3x4", 12, TRUE, 3}},
    {GL_FLOAT_MAT4x2, {"mat4x2", 8, TRUE, 4}},
    {GL_FLOAT_MAT4x3, {"mat4x3", 12, TRUE, 4}},
    {GL_INT, {"int", 1, FALSE, 0}},
    {GL_INT_VEC2, {"ivec2", 2, FALSE, 0}},
    {GL_INT_VEC3, {"ivec3", 3, FALSE, 0}},
    {GL_INT_VEC4, {"ivec4", 4, FALSE, 0}},
    {GL_UNSIGNED_INT, {"uint", 1, FALSE, 0}},
    {GL_UNSIGNED_INT_VEC2, {"uvec2", 2, FALSE, 0}},
    {GL_UNSIGNED_INT_VEC3, {"uvec3", 3, FALSE, 0}},
    {GL_UNSIGNED_INT_VEC4, {"uvec4", 4, FALSE, 0}},
  };
 
  std::vector<std::tuple<INT, std::function<INT( INT, const CHAR * )>,
    std::function<VOID( UINT, UINT, INT, INT *, INT *,
                        GLenum *, CHAR * )>,
    std::map<std::string, ATTR_INFO>&>> attr_data
  {
    {GL_ACTIVE_ATTRIBUTES, glGetAttribLocation, glGetActiveAttrib, Attributes},
    {GL_ACTIVE_UNIFORMS, glGetUniformLocation, glGetActiveUniform, Uniforms},
  };
 
  /* Fill shader attributes/uniforms info */
  for (auto d : attr_data)
  {
    INT Count;
    glGetProgramiv(ProgId, std::get<0>(d), &Count);
 
    std::get<3>(d).clear();
    for (INT i = 0; i < Count; i++)
    {
      CHAR name[300];
      INT size;
      GLenum type;
      std::get<2>(d)(ProgId, i, sizeof(name), nullptr, &size, &type, name);
      auto attr_info = attr_types[type];
      INT loc = std::get<1>(d)(ProgId, name);
 
      std::get<3>(d)[name] =
      {
        std::get<0>(attr_info),
        std::get<1>(attr_info),
        std::get<2>(attr_info),
        std::get<3>(attr_info),
        loc
      };
    }
  }
  INT CountSSBO;
  glGetProgramInterfaceiv(ProgId, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES,
                          &CountSSBO);
  SSBOBuffers.clear();
  for (INT i = 0; i < CountSSBO; i++)
  {
    CHAR name[300];
    glGetProgramResourceName(ProgId, GL_SHADER_STORAGE_BLOCK, i, sizeof(name),
                             nullptr, name);
    INT idx = glGetProgramResourceIndex(ProgId, GL_SHADER_STORAGE_BLOCK, name);
    GLenum prop[] {GL_BUFFER_BINDING};
    INT bind;
    glGetProgramResourceiv(ProgId, GL_SHADER_STORAGE_BLOCK, idx,
      1, prop, 1, nullptr, &bind);
    SSBOBuffers[name] = {name, idx, bind};
  }
} /* End of 'tse::shader::UpdateInfo' function */

/* Save text to log file function.
 * ARGUMENTS:
 *   - text 1 to save:
 *       const std::string &Stage;
 *   - text 2 to save:
 *       const std::string &Text;
 * RETURNS: None.
 */
VOID tse::shader::Log( const std::string &Stage, const std::string &Text )
{
  tse::logger::LogLn(Name + "\\" + Stage + ":\n" + Text);
  tse::logger::LogLn(Name + "\\" + Stage + ":\n" + Text);
  std::ofstream(anim::Path() + "bin/shaders/~SHD{AB7}30.LOG", std::ios_base::app) <<
    Name << "\n" << Stage << ":\n" << Text << "\n";
} /* End of 'tse::shader::Log' function */

/* Text file load to memory function.
 * ARGUMENTS:
 *   - file name:
 *       const std::string &FileName;
 * RETURNS:
 *   (std::string) load text.
 */
std::string tse::shader::LoadTextFile( const std::string &FileName )
{
  std::ifstream f(anim::Path() + FileName);
 
  return std::string((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());
} /* End of 'tse::shader::LoadTextFile' function */

/* Load shader function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (shader &) self reference.
 */
tse::shader & tse::shader::Load( VOID )
{
  /* Load common shader include 'commondf.glsl' file */
  static std::string CommonShaderIncludes =
    LoadTextFile("bin/shaders/includes/commondf.glsl");

  /* Shader file load structure */
  struct
  {
    const CHAR *Name;   // Shader file suffix name
    UINT Type;          // Shader OpenGL type
    const CHAR *Define; // Mandatory define and shader version
    UINT Id;            // Shader id created by OpenGL
  } shds[] =
  {
    {"vert", GL_VERTEX_SHADER,          "#version 460\n#define VERTEX_SHADER 1\n",          0},
    {"ctrl", GL_TESS_CONTROL_SHADER,    "#version 460\n#define TESS_CONTROL_SHADER 1\n",    0},
    {"eval", GL_TESS_EVALUATION_SHADER, "#version 460\n#define TESS_EVALUATION_SHADER 1\n", 0},
    {"geom", GL_GEOMETRY_SHADER,        "#version 460\n#define GEOMETRY_SHADER 1\n",        0},
    {"comp", GL_COMPUTE_SHADER,         "#version 460\n#define COMPUTE_SHADER 1\n",         0},
    {"frag", GL_FRAGMENT_SHADER,        "#version 460\n#define FRAGMENT_SHADER 1\n",        0},
  };
  INT ns = sizeof(shds) / sizeof(shds[0]), res;
  BOOL is_ok = TRUE;
  CHAR Buf[3000];

  /* Load local shader include 'incl.glsl' file */
  std::string LocalShaderIncludes =
    LoadTextFile("bin/shaders/" + Name + "/incl.glsl");

  for (INT i = 0; i < ns; i++)
  {
    /* Load text file */
    std::string txt =
      LoadTextFile("bin/shaders/" + Name + "/" + shds[i].Name + ".glsl");
    if (txt == "")
    {
      if (i > 0 && i < ns - 1)
        continue;
      Log(shds[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    if ((shds[i].Id = glCreateShader(shds[i].Type)) == 0)
    {
      Log(shds[i].Name, "Error shader creation");
      is_ok = FALSE;
      break;
    }
    /* Attach text to shader */
    const CHAR *Src[] = 
    {
      shds[i].Define,
      CommonShaderIncludes.c_str(),
      LocalShaderIncludes.c_str(),
      txt.c_str()
    };
    /* Set shader program source text */
    glShaderSource(shds[i].Id, sizeof(Src) / sizeof(Src[0]), Src, nullptr);
    /* Compile shader */
    glCompileShader(shds[i].Id);
    glGetShaderiv(shds[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shds[i].Id, sizeof(Buf), &res, Buf);
      Log(shds[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }
  /* Create program */
  INT prg = 0;
  if (is_ok)
  {
    if ((prg = glCreateProgram()) == 0)
    {
      Log("PROG", "Error create program");
      is_ok = FALSE;
    }
    else
    {
      /* Attach shaders to program */
      for (INT i = 0; i < ns; i++)
        if (shds[i].Id != 0)
          glAttachShader(prg, shds[i].Id);
      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        Log(Name, Buf);
        is_ok = FALSE;
      }
    }
  }
  /* Error handle */
  if (!is_ok)
  {
    /* Remove all shaders */
    for (INT i = 0; i < ns; i++)
      if (shds[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shds[i].Id);
        glDeleteShader(shds[i].Id);
      }
    /* Remove program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = 0;
  }
  else
    tse::logger::Info("SHADER created: " + Name);
  ProgId = prg;
  UpdateInfo();
  return *this;
} /* End of 'tse::shader::Load' function */

/* Free shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::shader::Free( VOID )
{
  if (ProgId == 0 || !glIsProgram(ProgId))
    return;

  Watcher.StopWatch();
  UINT shdrs[5];
  INT n;
  glGetAttachedShaders(ProgId, 5, &n, shdrs);
  for (INT i = 0; i < n; i++)
    if (glIsShader(shdrs[i]))
    {
      glDetachShader(ProgId, shdrs[i]);
      glDeleteShader(shdrs[i]);
    }
  glDeleteProgram(ProgId);
} /* End of 'tse::shader::Free' function */

/***
 * SHADER MANAGER FUNCTIONS
 ***/

/* Create shader function.
 * ARGUMENTS:
 *   - shader file name prefix:
 *       const std::string &FileNamePrefix;
 * RETURNS:
 *   (shader *) created primitive interface.
 */
tse::shader * tse::shader_manager::ShdCreate( const std::string &FileNamePrefix )
{
  return resource_manager::Add(shader(FileNamePrefix).Load());
} /* End of 'tse::shader_manager::ShdCreate' function */

/* Class constructor.
 * ARGUMENTS:
 *   - render instance reference:
 *       render &Rnd;
 */
tse::shader_manager::shader_manager( render &Rnd ) : resource_manager(Rnd)
{
  Watcher.StartWatch(anim::Path() + "bin/shaders");
} /* End of 'tse::shader_manager::shader_manager' function */

/* Class destructor */
tse::shader_manager::~shader_manager( VOID )
{
  Watcher.StopWatch();
} /* End of 'tse::shader_manager::~shader_manager' function */

/* Update all shaders in stock function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::shader_manager::Update( VOID )
{
  if (Watcher.IsChanged())
  {
    for (auto &shd : Stock)
      shd.second.Update();
    Watcher.StartWatch(anim::Path() + "bin/shaders/");
  }
} /* End of 'tse::shader_manager::Update' function */

/* END OF 'shd.cpp' FILE */
