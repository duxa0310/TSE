/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_vec3.h
 * PURPOSE     : Tough Space Exploration project.
 *               Mathematics library.
 *               3D vector handle module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 31.07.2025.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec3_h_
#define __mth_vec3_h_

#include "mth_def.h"

/* Space math namespace */
namespace mth
{
  /* 3D vector representation type */
  template<typename type>
    class vec3
    {
    public:
      type X, Y, Z; // Vector coordinates

      /* Default class constructor.
       * ARGUMENTS: None.
       */
      vec3( VOID )
      {
      } /* End of 'vec3' function */

      /* Class constructor function.
       * ARGUMENTS:
       *   - general coordinate value:
       *       type A;
       */
      explicit vec3( type A ) : X(A), Y(A), Z(A)
      {
      } /* End of 'vec3' function */

      /* Class constructor function.
       * ARGUMENTS:
       *   - coordinates values:
       *       type A, B, C;
       */
      vec3( type A, type B, type C ) : X(A), Y(B), Z(C)
      {
      } /* End of 'vec3' function */

      /* Get vector length function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) vector length.
       */
      type operator!( VOID ) const
      {
        return sqrt(X * X + Y * Y + Z * Z);
      } /* End of 'operator!' function */

      /* Get square of vector length function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) vector length squared.
       */
      type Length2( VOID ) const
      {
        return *this & *this;
      } /* End of 'Length2' function */

      /* Get vector sum function.
       * ARGUMENTS:
       *   - vector to add reference:
       *       vec3 &V;
       * RETURNS:
       *   (vec3) sum of vectors.
       */
      vec3 operator+( const vec3 &V ) const
      {
        return vec3(X + V.X, Y + V.Y, Z + V.Z);
      } /* End of 'operator+' function */

      /* Add vector function.
       * ARGUMENTS:
       *   - vector to add reference:
       *       vec3 &V;
       * RETURNS:
       *   (vec3 &) sum of vectors reference.
       */
      vec3 & operator+=( const vec3 &V )
      {
        X += V.X;
        Y += V.Y;
        Z += V.Z;
        return *this;
      } /* End of 'operator+=' function */

      /* Get vector difference function.
       * ARGUMENTS:
       *   - vector to subtract reference:
       *       vec3 &V;
       * RETURNS:
       *   (vec3) difference of vectors.
       */
      vec3 operator-( const vec3 &V ) const
      {
        return vec3(X - V.X, Y - V.Y, Z - V.Z);
      } /* End of 'operator-' function */

      /* Subtract vector function.
       * ARGUMENTS:
       *   - vector to subtract reference:
       *       vec3 &V;
       * RETURNS:
       *   (vec3 &) difference of vectors reference.
       */
      vec3 & operator-=( const vec3 &V )
      {
        X -= V.X;
        Y -= V.Y;
        Z -= V.Z;
        return *this;
      } /* End of 'operator-=' function */

      /* Get negatived vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) negatived vector.
       */
      vec3 operator-( VOID ) const
      {
        return vec3(-X, -Y, -Z);
      } /* End of 'operator-' function */

      /* Get vector and number product function.
       * ARGUMENTS:
       *   - number to multiply by:
       *       type N;
       * RETURNS:
       *   (vec3) vector and number product.
       */
      vec3 operator*( type N ) const
      {
        return vec3(X * N, Y * N, Z * N);
      } /* End of 'operator*' function */

      /* Multiply vector coordinates by number function.
       * ARGUMENTS:
       *   - number to multiply by:
       *       type N;
       * RETURNS:
       *   (vec3 &) product of vector and number reference.
       */
      vec3 & operator*=( type N )
      {
        X *= N;
        Y *= N;
        Z *= N;
        return *this;
      } /* End of 'operator*=' function */

      /* Get vector coordinate product function.
       * ARGUMENTS:
       *   - vector to multiply with reference:
       *       vec3 &V;
       * RETURNS:
       *   (vec3) vector coordinate product.
       */
      vec3 operator*( const vec3 &V ) const
      {
        return vec3(X * V.X, Y * V.Y, Z * V.Z);
      } /* End of 'operator*' function */

      /* Multiply vector coordinates function.
       * ARGUMENTS:
       *   - vector to multiply coordinates reference:
       *       vec3 &V;
       * RETURNS:
       *   (vec3 &) coordinate product of vectors reference.
       */
      vec3 & operator*=( const vec3 &V )
      {
        X *= V.X;
        Y *= V.Y;
        Z *= V.Z;
        return *this;
      } /* End of 'operator*=' function */

      /* Get vector dot product function.
       * ARGUMENTS:
       *   - vector to multiply with reference:
       *       vec3 &V;
       * RETURNS:
       *   (type) vector dot product.
       */
      type operator&( const vec3 &V ) const
      {
        return X * V.X + Y * V.Y + Z * V.Z;
      } /* End of 'operator&' function */

      /* Get vector cross product function.
       * ARGUMENTS:
       *   - vector to multiply with reference:
       *       vec3 &V;
       * RETURNS:
       *   (vec3) vector cross product.
       */
      vec3 operator%( const vec3 &V ) const
      {
        return vec3(Y * V.Z - Z * V.Y,
                    Z * V.X - X * V.Z,
                    X * V.Y - Y * V.X);
      } /* End of 'operator%' function */

      /* Get vector by number division result function.
       * ARGUMENTS:
       *   - number to divide by:
       *       type N;
       * RETURNS:
       *   (vec3) vector and number division result.
       */
      vec3 operator/( type N ) const
      {
        if (N == 0)
          return vec3(0);
        return vec3(X / N, Y / N, Z / N);
      } /* End of 'operator/' function */

      /* Divide vector coordinates by number function.
       * ARGUMENTS:
       *   - number to divide by:
       *       type N;
       * RETURNS:
       *   (vec3) division result of vector and number.
       */
      vec3 & operator/=( type N )
      {
        if (N == 0)
          X = 0, Y = 0, Z = 0;
        else
          X /= N, Y /= N, Z /= N;
        return *this;
      } /* End of 'operator/=' function */

      /* Normalize vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3 &) normalized vector reference.
       */
      vec3 & Normalize( VOID )
      {
        *this /= !*this;
        return *this;
      } /* End of 'Normalize' function */

      /* Get normalized vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) normalized vector.
       */
      vec3 Normalizing( VOID ) const
      {
        return *this / !*this;
      } /* End of 'Normalizing' function */

      /* Get distance between vectors function.
       * ARGUMENTS:
       *   - second vector to calculate distance reference:
       *       vec3 &P;
       * RETURNS:
       *   (type) distance between vectors;
       */
      type Distance( const vec3 &P )
      {
        return !(*this - P);
      } /* End of 'Distance' function */
      
      /* Get vector coordinate by index function.
       * ARGUMENTS:
       *   - coordinate index:
       *       INT I;
       * RETURNS:
       *   (type) coordinate value.
       */
      type operator[]( INT I ) const
      {
        if (I == 0)
          return X;
        if (I == 1)
          return Y;
        return Z;
      } /* End of 'operator[]' function */

      /* Get vector coordinates pointer function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type *) first coordinate pointer;
       */
      operator type *( VOID )
      {
        return &X;
      } /* End of 'operator type *' function */

      /* Get minimum-componented vector function.
       * ARGUMENTS:
       *   - vector to compare reference:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) minimum vector;
       */
      vec3 Min( const vec3 &V ) const
      {
        return vec3(min(X, V.X), min(Y, V.Y), min(Z, V.Z));
      } /* End of 'Min' function */

      /* Get maximum-componented vector function.
       * ARGUMENTS:
       *   - vector to compare reference:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) maximum vector;
       */
      vec3 Max( const vec3 &V ) const
      {
        return vec3(max(X, V.X), max(Y, V.Y), max(Z, V.Z));
      } /* End of 'Max' function */

      /* Check being in AABB boundaries function.
       * ARGUMENTS:
       *   - AABB boundary vector references:
       *       const vec3 &MinBB, &MaxBB;
       * RETURNS:
       *   (BOOL) TRUE, if point is inside AABB, otherwise FALSE.
       */
      BOOL IsBoxInside( const vec3<type> &MinBB, const vec3<type> &MaxBB ) const
      {
        if (X < MinBB.X || X > MaxBB.X ||
            Y < MinBB.Y || Y > MaxBB.Y ||
            Z < MinBB.Z || Z > MaxBB.Z)
          return FALSE;
        return TRUE;
      } /* End of 'IsBoxInside' function */

      /* Get vector with zero coordinates function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) vector with zero coordinates.
       */
      static vec3 Zero( VOID )
      {
        return vec3(0);
      } /* End of 'Zero' function */

      /* Get vector with random coordinates (range [0; 1]) function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) vector with random coordinates.
       */
      static vec3 Rnd0( VOID )
      {
        return vec3(static_cast<type>(rand()) / RAND_MAX,
                    static_cast<type>(rand()) / RAND_MAX,
                    static_cast<type>(rand()) / RAND_MAX);
      } /* End of 'Rnd0' function */

      /* Get vector with random coordinates (range [-1; 1]) function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) vector with random coordinates.
       */
      static vec3 Rnd1( VOID )
      {
        return vec3(2 * (static_cast<type>(rand()) / RAND_MAX) - 1,
                    2 * (static_cast<type>(rand()) / RAND_MAX) - 1,
                    2 * (static_cast<type>(rand()) / RAND_MAX) - 1);
      } /* End of 'Rnd1' function */

    }; /* End of 'vec3' class */

} /* end of 'mth' namespace */

#endif /* __mth_vec3_h_ */

/* END OF 'mth_vec3.h' FILE */
