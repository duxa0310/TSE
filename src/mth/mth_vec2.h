/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_vec2.h
 * PURPOSE     : Tough Space Exploration project.
 *               Mathematics library.
 *               2D vector handle module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 31.07.2025.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec2_h_
#define __mth_vec2_h_

#include "mth_def.h"

/* Space math namespace */
namespace mth
{
  /* 3D vector representation type */
  template<typename type>
    class vec2
    {
    public:
      type X, Y; // Vector coordinates

      /* Default class constructor function.
       * ARGUMENTS: None.
       */
      vec2( VOID )
      {
      } /* End of 'vec2' function */

      /* Class constructor function.
       * ARGUMENTS:
       *   - general coordinate value:
       *       type A;
       */
      explicit vec2( type A ) : X(A), Y(A)
      {
      } /* End of 'vec2' function */

      /* Class constructor.
       * ARGUMENTS:
       *   - coordinates values:
       *       type A, B, C;
       */
      vec2( type A, type B ) : X(A), Y(B)
      {
      } /* End of 'vec2' function */

      /* Get vector length function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) vector length.
       */
      type operator!( VOID ) const
      {
        return sqrt(X * X + Y * Y);
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
       *       vec2 &V;
       * RETURNS:
       *   (vec2) sum of vectors.
       */
      vec2 operator+( const vec2 &V ) const
      {
        return vec2(X + V.X, Y + V.Y);
      } /* End of 'operator+' function */

      /* Add vector function.
       * ARGUMENTS:
       *   - vector to add reference:
       *       vec2 &V;
       * RETURNS:
       *   (vec2 &) sum of vectors reference.
       */
      vec2 & operator+=( const vec2 &V )
      {
        X += V.X;
        Y += V.Y;
        return *this;
      } /* End of 'operator+=' function */

      /* Get vector difference function.
       * ARGUMENTS:
       *   - vector to subtract reference:
       *       vec2 &V;
       * RETURNS:
       *   (vec2) difference of vectors.
       */
      vec2 operator-( const vec2 &V ) const
      {
        return vec2(X - V.X, Y - V.Y);
      } /* End of 'operator-' function */

      /* Subtract vector function.
       * ARGUMENTS:
       *   - vector to subtract reference:
       *       vec2 &V;
       * RETURNS:
       *   (vec2 &) difference of vectors reference.
       */
      vec2 & operator-=( const vec2 &V )
      {
        X -= V.X;
        Y -= V.Y;
        return *this;
      } /* End of 'operator-=' function */

      /* Get negatived vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) negatived vector.
       */
      vec2 operator-( VOID ) const
      {
        return vec2(-X, -Y);
      } /* End of 'operator-' function */

      /* Get vector and number product function.
       * ARGUMENTS:
       *   - number to multiply by:
       *       type N;
       * RETURNS:
       *   (vec2) vector and number product.
       */
      vec2 operator*( type N ) const
      {
        return vec2(X * N, Y * N);
      } /* End of 'operator*' function */

      /* Multiply vector coordinates by number function.
       * ARGUMENTS:
       *   - number to multiply by:
       *       type N;
       * RETURNS:
       *   (vec2 &) product of vector and number reference.
       */
      vec2 & operator*=( type N )
      {
        X *= N;
        Y *= N;
        return *this;
      } /* End of 'operator*=' function */

      /* Get vector coordinate product function.
       * ARGUMENTS:
       *   - vector to multiply with reference:
       *       vec2 &V;
       * RETURNS:
       *   (vec2) vector coordinate product.
       */
      vec2 operator*( const vec2 &V ) const
      {
        return vec2(X * V.X, Y * V.Y);
      } /* End of 'operator*' function */

      /* Multiply vector coordinates function.
       * ARGUMENTS:
       *   - vector to multiply coordinates reference:
       *       vec2 &V;
       * RETURNS:
       *   (vec2 &) coordinate product of vectors reference.
       */
      vec2 & operator*=( const vec2 &V )
      {
        X *= V.X;
        Y *= V.Y;
        return *this;
      } /* End of 'operator*=' function */

      /* Get vector dot product function.
       * ARGUMENTS:
       *   - vector to multiply with reference:
       *       vec2 &V;
       * RETURNS:
       *   (type) vector dot product.
       */
      type operator&( const vec2 &V ) const
      {
        return X * V.X + Y * V.Y;
      } /* End of 'operator&' function */

      /* Get vector by number division result function.
       * ARGUMENTS:
       *   - number to divide by:
       *       type N;
       * RETURNS:
       *   (vec2) vector and number division result.
       */
      vec2 operator/( type N ) const
      {
        if (N == 0)
          return vec2(0);
        return vec2(X / N, Y / N);
      } /* End of 'operator/' function */

      /* Divide vector coordinates by number function.
       * ARGUMENTS:
       *   - number to divide by:
       *       type N;
       * RETURNS:
       *   (vec2) division result of vector and number.
       */
      vec2 & operator/=( type N )
      {
        if (N == 0)
          X = 0, Y = 0;
        else
          X /= N, Y /= N;
        return *this;
      } /* End of 'operator/=' function */

      /* Normalize vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2 &) normalized vector reference.
       */
      vec2 & Normalize( VOID )
      {
        *this /= !*this;
        return *this;
      } /* End of 'Normalize' function */

      /* Get normalized vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) normalized vector.
       */
      vec2 Normalizing( VOID ) const
      {
        return *this / !*this;
      } /* End of 'Normalizing' function */

      /* Get distance between vectors function.
       * ARGUMENTS:
       *   - second vector to calculate distance reference:
       *       vec2 &P;
       * RETURNS:
       *   (type) distance between vectors;
       */
      type Distance( const vec2 &P )
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
        return Y;
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
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) minimum vector;
       */
      vec2 Min( const vec2 &V ) const
      {
        return vec2(min(X, V.X), min(Y, V.Y));
      } /* End of 'Min' function */

      /* Get maximum-componented vector function.
       * ARGUMENTS:
       *   - vector to compare reference:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) maximum vector;
       */
      vec2 Max( const vec2 &V ) const
      {
        return vec2(max(X, V.X), max(Y, V.Y));
      } /* End of 'Max' function */

      /* Check being in square boundaries function.
       * ARGUMENTS:
       *   - square boundary vector references:
       *       const vec2 &MinBB, &MaxBB;
       * RETURNS:
       *   (BOOL) TRUE, if point is inside square, otherwise FALSE.
       */
      BOOL IsSquareInside( const vec2<type> &MinBB, const vec2<type> &MaxBB ) const
      {
        if (X < MinBB.X || X > MaxBB.X ||
            Y < MinBB.Y || Y > MaxBB.Y)
          return FALSE;
        return TRUE;
      } /* End of 'IsBoxInside' function */

      /* Get vector with zero coordinates function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) vector with zero coordinates.
       */
      static vec2 Zero( VOID )
      {
        return vec2(0);
      } /* End of 'Zero' function */

      /* Get vector with random coordinates (range [0; 1]) function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) vector with random coordinates.
       */
      static vec2 Rnd0( VOID )
      {
        return vec2(static_cast<type>(rand()) / RAND_MAX,
                    static_cast<type>(rand()) / RAND_MAX);
      } /* End of 'Rnd0' function */

      /* Get vector with random coordinates (range [-1; 1]) function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) vector with random coordinates.
       */
      static vec2 Rnd1( VOID )
      {
        return vec2(2 * (static_cast<type>(rand()) / RAND_MAX) - 1,
                    2 * (static_cast<type>(rand()) / RAND_MAX) - 1);
      } /* End of 'Rnd1' function */

    }; /* End of 'vec2' class */

} /* end of 'mth' namespace */

#endif /* __mth_vec2_h_ */

/* END OF 'mth_vec2.h' FILE */
