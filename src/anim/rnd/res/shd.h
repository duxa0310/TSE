/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : shd.h
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Shaders declaration module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 02.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __shd_h_
#define __shd_h_

#include "resources.h"

/* Main program namespace */
namespace tse
{
  class render;

  /* Shader handle class */
  class shader : public resource
  {
    friend class render;

  private:
    dir_watcher Watcher; // Directory watcher

    /* Save text to log file function.
     * ARGUMENTS:
     *   - text 1 to save:
     *       const std::string &Stage;
     *   - text 2 to save:
     *       const std::string &Text;
     * RETURNS: None.
     */
    VOID Log( const std::string &Stage, const std::string &Text );
 
    /* Text file load to memory function.
     * ARGUMENTS:
     *   - file name:
     *       const std::string &FileName;
     * RETURNS:
     *   (std::string) load text.
     */
    std::string LoadTextFile( const std::string &FileName );
 
  public:
    /* Load shader function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (shader &) self reference.
     */
    shader & Load( VOID );

    /* Free shader function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );
 
  public:
    std::string Name; // Shader name
    UINT ProgId;      // Shader program Id
 
    /* Shader input attributes and uniforms information structure */
    struct ATTR_INFO
    {
      std::string SType; // String type like shader
      INT Components;    // Number of components
      BOOL IsFloat;      // Float value flag (otherwise integer)
      INT MatrRows;      // For matrix number of rows
      INT Loc;           // Shader attribute location
    }; /* End of 'ATTR_INFO' structure */
 
    /* Shader buffer block information structure */
    struct BLOCK_INFO
    {
      std::string SType; // String type like shader
      INT Index;         // Buffer block index
      INT Bind;          // Buffer bind point
    }; /* End of 'BLOCK_INFO' structure */
 
    // Attributes and uniforms information
    std::map<std::string, ATTR_INFO> Attributes, Uniforms;
    // Shader storage blocks informations (type, index, bind point)
    std::map<std::string, BLOCK_INFO> SSBOBuffers;
 
    /* Class default constructor */
    shader( VOID );
 
    /* Class constructor.
     * ARGUMENTS:
     *   - shader file name prefix:
     *       const std::string &FileNamePrefix;
     */
    shader( const std::string &FileNamePrefix );
 
    /* Class destructor */
    ~shader( VOID );
 
    /* Apply shader function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (UINT) shader program Id;
     */
    UINT Apply( VOID );
 
    /* Update shader function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Update( VOID );
 
    /* Update shader internal data info function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateInfo( VOID );

  }; /* End of 'shader' class */
 
  /* Shader manager */
  class shader_manager : public resource_manager<shader, std::string>
  {
  private:
    dir_watcher Watcher;

  public:
    /* Create shader function.
     * ARGUMENTS:
     *   - shader file name prefix:
     *       const std::string &FileNamePrefix;
     * RETURNS:
     *   (shader *) created primitive interface.
     */
    shader * ShdCreate( const std::string &FileNamePrefix );
 
    /* Class constructor.
     * ARGUMENTS:
     *   - render instance reference:
     *       render &Rnd;
     */
    shader_manager( render &Rnd );
 
    /* Class destructor */
    ~shader_manager( VOID );
 
    /* Update all shaders in stock function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Update( VOID );

  }; /* End of 'shader_manager' class */

} /* end of 'tse' namespace */

#endif /* __shd_h_ */

/* END OF 'shd.h' FILE */
