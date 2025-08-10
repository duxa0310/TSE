/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : buf.h
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Buffers declaration module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __buf_h_
#define __buf_h_

#include "resources.h"

/* Main program namespace */
namespace tse
{
  class render;
 
  /* Buffer handle class */
  class buffer : public resource
  {
    friend class render;

  public:
    UINT BufId = 0;        // OpenGL buffer Id
    UINT BindingPoint = 0; // Binding point
    UINT BufSize = 0;      // Number of buffer quads
 
    /* Buffer create function.
     * ARGUMENTS:
     *   - buffer binding point:
     *       UINT BufBindingPoint;
     *   - buffer data:
     *       const data_type *Data;
     * RETURNS:
     *   (buffer &) self reference.
     */
    template<typename data_type>
      buffer & Create( UINT BufBindingPoint, const data_type *Data  = {} )
      {
        Free();
        BufSize = sizeof(data_type);
        if (BufSize == 0)
          return *this;
 
        BindingPoint = BufBindingPoint;
 
        glGenBuffers(1, &BufId);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, BufId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data_type), Data,
                     GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        Apply();
        tse::logger::Info(std::format("BUFFER created: binding = {}, {} bytes", BindingPoint, sizeof(data_type)));
        return *this;
      } /* End of 'Create' function */
 
    /* Buffer create function.
     * ARGUMENTS:
     *   - buffer binding point:
     *       UINT BufBindingPoint;
     *   - number of data elements:
     *       INT NumOfElements;
     *   - buffer data:
     *       const data_type *Data;
     * RETURNS:
     *   (buffer &) self reference.
     */
    template<typename data_type>
      buffer & Create( UINT BufBindingPoint, INT NumOfElements,
                       const data_type *Data  = {} )
      {
        Free();
        BufSize = sizeof(data_type) * NumOfElements;
        if (BufSize == 0)
          return *this;
 
        BindingPoint = BufBindingPoint;
        glGenBuffers(1, &BufId);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, BufId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data_type) * NumOfElements,
                     Data, GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        Apply();
        return *this;
      } /* End of 'Create' function */
 
    /* Buffer update function.
     * ARGUMENTS:
     *   - buffer data:
     *       const data_type *Data;
     *   - start of elements update:
     *       INT Start;
     *   - length of elements, which will be updated:
     *       INT Size;
     * RETURNS: None.
     */
    template<typename data_type>
      VOID Update( const data_type *Data, INT Start = 0, INT Size = -1 )
      {
        if (BufId == 0)
          return;
        if (Size < 0)
          Size = BufSize;
        if (Start < 0)
          Start = 0;
        if (Start + Size > BufSize)
          Size = BufSize - Start;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, BufId);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, Start, Size, Data);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
      } /* End of 'Update' finction */

    /* Buffer free function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Apply buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Apply( VOID );

  }; /* End of 'buffer' class */
 
  /* Buffer manager */
  class buffer_manager : public resource_manager<buffer>
  {
  public:
    /* Create buffer function.
     * ARGUMENTS:
     *   - buffer binding point:
     *       UINT BufBindingPoint;
     *   - buffer data:
     *       const data_type *Data;
     * RETURNS:
     *   (buffer &) self reference.
     */
    template<typename data_type>
      buffer * BufCreate( UINT BufBindingPoint, const data_type *Data  = {} )
      {
        return resource_manager::Add(buffer().Create(BufBindingPoint, Data));
      } /* End of 'BufCreate' function */

    /* Class constructor.
     * ARGUMENTS:
     *   - render instance reference:
     *       render &Rnd;
     */
    buffer_manager( render &Rnd );
 
    /* Class destructor */
    ~buffer_manager( VOID );

  }; /* End of 'buffer_manager' class */

} /* end of  'tse' namespace */

#endif /* __buf_h_ */

/* END OF 'buf.h' FILE */
