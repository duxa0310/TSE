/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : prim.h
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Primitive declaration module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 01.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __prim_h_
#define __prim_h_

#include "resources.h"

/* Main program namespace */
namespace tse
{
  class render;
 
  /* Primitive shape representation type */
  enum struct prim_type
  {
    TRIMESH, // Triangle mesh
    POINTS,  // Points collection
    STRIP,   // Strip of triangles
    LINES,   // Lines collection
  }; /* End of 'prim_type' enum */
 
  /* Primitive handle class */
  class prim : public resource
  {
    friend class render;
 
  private:
    prim_type Type {};    // Primitive type
    UINT VA {};           // Primitive vertex array
    UINT VBuf {};         // Vertex buffer
    UINT IBuf {};         // Index buffer
    INT NumOfElements {}; // Number of elements for OpenGL
    matr Transform {};    // Primitive transformation matrix
 
    mutable BOOL IsVAUpdated = FALSE; // Vertex array update flag
 
    // Vertex parameters map (vertex attribute field, offset)
    std::map<std::string, INT> VertexMap;
    INT VertexStride {}; // Vertex stride in bytes
 
  public:
    material *Mtl {};     // Material pointer
    vec3
      MinBB {0},          // Minimal primitive position
      MaxBB {0};          // Maximal primitive position
 
    /* Class constructor */
    prim( VOID );
 
    /* Class destructor */
    ~prim( VOID );
 
    /* Primitive destroy function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );
 
    /* Primitive creation function.
     * ARGUMENTS:
     *   - material pointer:
     *       material *Mat;
     *   - primitive type:
     *       prim_type NewType;
     *   - vertex array:
     *       const std::vector<vertex> &V;
     *   - index array:
     *       const std::vector<INT> &Ind;
     * RETURNS:
     *   (prim &) self reference.
     */
    template<class vertex>
      prim & Create( material *Mat, prim_type NewType,
                     const std::span<vertex> &V,
                     const std::span<INT> &Ind )
      {
        Free();
        Type = NewType;
        Transform = matr::Identity();
        NumOfElements = 0;
        VertexStride = sizeof(vertex);
        Mtl = Mat;
 
        if constexpr (requires{vertex::P;})
          VertexMap["InPosition"] = reinterpret_cast<INT_PTR>(&((vertex *)0)->P);
        if constexpr (requires{vertex::T;})
          VertexMap["InTexCoord"] = reinterpret_cast<INT_PTR>(&((vertex *)0)->T);
        if constexpr (requires{vertex::N;})
          VertexMap["InNormal"] = reinterpret_cast<INT_PTR>(&((vertex *)0)->N);
        if constexpr (requires{vertex::C;})
          VertexMap["InColor"] = reinterpret_cast<INT_PTR>(&((vertex *)0)->C);
        if constexpr (requires{vertex::Tangent;})
          VertexMap["InTangent"] =
            reinterpret_cast<INT_PTR>(&((vertex *)0)->Tangent);
        if constexpr (requires{vertex::Bitangent;})
          VertexMap["InBitangent"] =
            reinterpret_cast<INT_PTR>(&((vertex *)0)->Bitangent);
        /* Create OpenGL vertex array */
        glGenVertexArrays(1, &VA);
        if (V.size() != 0)
        {
          /* Collect min-max info */
          if constexpr (requires{vertex::P;})
          {
            MinBB = MaxBB = V[0].P;
            for (auto vrt : V)
              MinBB = vrt.P.Min(MinBB), MaxBB = vrt.P.Max(MaxBB);
          }
          /* Create OpenGL buffers */
          glGenBuffers(1, &VBuf);
          /* Activate vertex array */
          glBindVertexArray(VA);
          /* Activate vertex buffer */
          glBindBuffer(GL_ARRAY_BUFFER, VBuf);
          /* Store vertex data */
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * V.size(), V.data(),
            GL_STATIC_DRAW);
        }
        /* Disable vertex array */
        glBindVertexArray(0);
 
        /* Indices */
        if (Ind.size() > 0)
        {
          if (V.size() != 0)
          {
            glGenBuffers(1, &IBuf);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBuf);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * Ind.size(), 
              Ind.data(), GL_STATIC_DRAW);
            /* Disable index array */
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
          }
          NumOfElements = (INT)Ind.size();
        }
        else
          NumOfElements = (INT)V.size();
        tse::logger::Info(std::format("PRIMITIVE created: {} vertices, {} triangles", V.size(), NumOfElements / 3));
        return *this;
      } /* End of 'Create' function */
 
    /* Update primitive vertex array function.
     * ARGUMENTS: None.
     * RETURNS: Non.
     */
    VOID UpdateVA( VOID ) const;
 
    /* Primitive with empty vertex stream creation function.
     * ARGUMENTS:
     *   - primitive type:
     *       prim_type Type;
     *   - number of emmitted vertexes:
     *       INT NumOfV;
     * RETURNS:
     *   (prim &) self reference.
     */
    prim & Create( prim_type Type, INT NumOfV );

    /* Load primitive from .OBJ function.
     * ARGUMENTS:
     *   - file name (*.OBJ model) to be load:
     *       const std::string &FileName;
     *   - transform matrix:
     *       const matr &Transform;
     *   - fit size flag vector:
     *       const vec3 &FitSize;
     * RETURNS:
     *   (prim &) self reference.
     */
    prim & Load( const std::string &FileName,
                 const matr &Transform = matr::Identity(),
                 const vec3 &FitSize = vec3(1, 0, 0) );

  }; /* End of 'prim' class */
 
  /* Set of primitives class */
  class model
  {
  public:
    matr Transform;      // Transformation matrix
    stock<prim *> Prims; // Stock of primitives
    vec3
      MinBB {},          // Minimal primitives set position
      MaxBB {};          // Maximal primitives set position
 
    /* Default constructor function */
    model( VOID );
 
    /* Destructor function */
    ~model( VOID );
 
    /* Add new primitive to set function.
     * ARGUMENTS:
     *   - primitive to be add:
     *       primt *Pr;
     * RETURNS:
     *   (model &) self reference.
     */
    model & operator<<( prim *Pr );
 
    /* Load model from .G3DM function.
     * ARGUMENTS:
     *   - file name (*.G3DM model) to be load:
     *       const std::string &FileName;
     *   - transform matrix:
     *       const matr &Transform;
     *   - fit size flag vector:
     *       const vec3 &FitSize;
     * RETURNS:
     *   (model &) self reference.
     */
    model & Load( const std::string &FileName,
                  const matr &Transform = matr::Identity(),
                  const vec3 &FitSize = vec3(1, 0, 0) );

  }; /* End of model' class */
 
  /* Primitive manager */
  class primitive_manager : public resource_manager<prim>
  {
  public:
    /* Default type constructor function */
    primitive_manager( render &Rnd );

    /* Primitive creation function.
     * ARGUMENTS:
     *   - material pointer:
     *       material *Mat;
     *   - primitive type:
     *       prim_type NewType;
     *   - vertex array:
     *       const std::vector<vertex_type> &V;
     *   - index array:
     *       const std::vector<INT> &Ind;
     * RETURNS:
     *   (prim *) created primitive interface.
     */
    template<class vertex_type>
      prim * PrimCreate( material *Mat, prim_type NewType,
                         const std::span<vertex_type> &V,
                         const std::span<INT> &Ind )
      {
        return resource_manager::Add(prim().Create<vertex_type>(Mat, NewType, V, Ind));
      } /* End of 'PrimCreate' function */
 
    /* Create primitive with empty vertex stream function.
     * ARGUMENTS:
     *   - primitive type:
     *       prim_type Type;
     *   - number of emmitted vertexes:
     *       INT NumOfV;
     * RETURNS:
     *   (prim *) created primitive interface.
     */
    prim * PrimCreate( prim_type Type, INT NumOfV );
 
    /* Create primitive function.
     * ARGUMENTS:
     *   - primitive pointer:
     *       prim *Pr;
     * RETURNS: None.
     *   (prim *) created primitive interface.
     */
    VOID PrimFree( prim *Pr );

  }; /* End of 'primitive_manager' class */

} /* end of 'tse' namespace */

#endif /* __prim_h_ */

/* END OF 'prim.h' FILE */
