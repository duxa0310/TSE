/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_vec4.h
 * PURPOSE     : Tough Space Exploration project.
 *               Mathematics library.
 *               4D vector handle module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 31.07.2025.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec4_h_
#define __mth_vec4_h_

#include "mth_def.h"

/* Space math namespace */
namespace mth
{
  /* 3D vector representation type */
  template<typename type>
    class vec4
    {
    public:
      type X, Y, Z, W; // Vector coordinates

      /* Default class constructor.
       * ARGUMENTS: None.
       */
      vec4( VOID )
      {
      } /* End of 'vec4' function */

      /* Class constructor fucntion.
       * ARGUMENTS:
       *   - general coordinate value:
       *       type A;
       */
      explicit vec4( type A ) : X(A), Y(A), Z(A), W(1)
      {
      } /* End of 'vec4' function */

      /* Class constructor fucntion.
       * ARGUMENTS:
       *   - coordinates values:
       *       type A, B, C;
       */
      vec4( type A, type B, type C ) : X(A), Y(B), Z(C), W(1)
      {
      } /* End of 'vec4' function */

      /* Class constructor fucntion.
       * ARGUMENTS:
       *   - 3D vector:
       *       vec3<type> V;
       *   - 4th coordinate value:
       *       type A;
       */
      vec4( vec3<type> V, type A = 1.0 ) : X(V.X), Y(V.Y), Z(V.Z), W(A)
      {
      } /* End of 'vec4' function */

      /* Class constructor fucntion.
       * ARGUMENTS:
       *   - coordinates values:
       *       type A, B, C, D;
       */
      vec4( type A, type B, type C, type D ) : X(A), Y(B), Z(C), W(D)
      {
      } /* End of 'vec4' function */

      /* Get vector length function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) vector length.
       */
      type operator!( VOID ) const
      {
        return sqrt(X * X + Y * Y + Z * Z + W * W);
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
       *       vec4 &V;
       * RETURNS:
       *   (vec4) sum of vectors.
       */
      vec4 operator+( const vec4 &V ) const
      {
        return vec4(X + V.X, Y + V.Y, Z + V.Z, W + V.W);
      } /* End of 'operator+' function */

      /* Add vector function.
       * ARGUMENTS:
       *   - vector to add reference:
       *       vec4 &V;
       * RETURNS:
       *   (vec4 &) sum of vectors reference.
       */
      vec4 & operator+=( const vec4 &V )
      {
        X += V.X;
        Y += V.Y;
        Z += V.Z;
        W += V.W;
        return *this;
      } /* End of 'operator+=' function */

      /* Get vector difference function.
       * ARGUMENTS:
       *   - vector to subtract reference:
       *       vec4 &V;
       * RETURNS:
       *   (vec4) difference of vectors.
       */
      vec4 operator-( const vec4 &V ) const
      {
        return vec4(X - V.X, Y - V.Y, Z - V.Z, W - V.W);
      } /* End of 'operator-' function */

      /* Subtract vector function.
       * ARGUMENTS:
       *   - vector to subtract reference:
       *       vec4 &V;
       * RETURNS:
       *   (vec4 &) difference of vectors reference.
       */
      vec4 & operator-=( const vec4 &V )
      {
        X -= V.X;
        Y -= V.Y;
        Z -= V.Z;
        W -= V.W;
        return *this;
      } /* End of 'operator-=' function */

      /* Get negatived vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec4) negatived vector.
       */
      vec4 operator-( VOID ) const
      {
        return vec4(-X, -Y, -Z, -W);
      } /* End of 'operator-' function */

      /* Get vector and number product function.
       * ARGUMENTS:
       *   - number to multiply by:
       *       type N;
       * RETURNS:
       *   (vec4) vector and number product.
       */
      vec4 operator*( type N ) const
      {
        return vec4(X * N, Y * N, Z * N, W * N);
      } /* End of 'operator*' function */

      /* Multiply vector coordinates by number function.
       * ARGUMENTS:
       *   - number to multiply by:
       *       type N;
       * RETURNS:
       *   (vec4 &) product of vector and number reference.
       */
      vec4 & operator*=( type N )
      {
        X *= N;
        Y *= N;
        Z *= N;
        W *= N;
        return *this;
      } /* End of 'operator*=' function */

      /* Get vector coordinate product function.
       * ARGUMENTS:
       *   - vector to multiply with reference:
       *       vec4 &V;
       * RETURNS:
       *   (vec4) vector coordinate product.
       */
      vec4 operator*( const vec4 &V ) const
      {
        return vec4(X * V.X, Y * V.Y, Z * V.Z, W * V.W);
      } /* End of 'operator*' function */

      /* Multiply vector coordinates function.
       * ARGUMENTS:
       *   - vector to multiply coordinates reference:
       *       vec4 &V;
       * RETURNS:
       *   (vec4 &) coordinate product of vectors reference.
       */
      vec4 & operator*=( const vec4 &V )
      {
        X *= V.X;
        Y *= V.Y;
        Z *= V.Z;
        W *= V.Z;
        return *this;
      } /* End of 'operator*=' function */

      /* Get vector dot product function.
       * ARGUMENTS:
       *   - vector to multiply with reference:
       *       vec4 &V;
       * RETURNS:
       *   (type) vector dot product.
       */
      type operator&( const vec4 &V ) const
      {
        return X * V.X + Y * V.Y + Z * V.Z + W * V.W;
      } /* End of 'operator&' function */

      /* Get vector by number division result function.
       * ARGUMENTS:
       *   - number to divide by:
       *       type N;
       * RETURNS:
       *   (vec4) vector and number division result.
       */
      vec4 operator/( type N ) const
      {
        if (N == 0)
          return vec4(0);
        return vec4(X / N, Y / N, Z / N, W / N);
      } /* End of 'operator/' function */

      /* Divide vector coordinates by number function.
       * ARGUMENTS:
       *   - number to divide by:
       *       type N;
       * RETURNS:
       *   (vec4) division result of vector and number.
       */
      vec4 & operator/=( type N )
      {
        if (N == 0)
          X = 0, Y = 0, Z = 0, W = 0;
        else
          X /= N, Y /= N, Z /= N, W /= N;
        return *this;
      } /* End of 'operator/=' function */

      /* Normalize vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec4 &) normalized vector reference.
       */
      vec4 & Normalize( VOID )
      {
        *this /= !*this;
        return *this;
      } /* End of 'Normalize' function */

      /* Get normalized vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec4) normalized vector.
       */
      vec4 Normalizing( VOID ) const
      {
        return *this / !*this;
      } /* End of 'Normalizing' function */

      /* Get distance between vectors function.
       * ARGUMENTS:
       *   - second vector to calculate distance reference:
       *       vec4 &P;
       * RETURNS:
       *   (type) distance between vectors;
       */
      type Distance( const vec4 &P )
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
        if (I == 2)
          return Z;
        return W;
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
       *       const vec4 &V;
       * RETURNS:
       *   (vec4) minimum vector;
       */
      vec4 Min( const vec4 &V ) const
      {
        return vec4(min(X, V.X), min(Y, V.Y), min(Z, V.Z), min(W, V.W));
      } /* End of 'Min' function */

      /* Get maximum-componented vector function.
       * ARGUMENTS:
       *   - vector to compare reference:
       *       const vec4 &V;
       * RETURNS:
       *   (vec4) maximum vector;
       */
      vec4 Max( const vec4 &V ) const
      {
        return vec4(max(X, V.X), max(Y, V.Y), max(Z, V.Z), max(W, V.W));
      } /* End of 'Max' function */

      /* Get vector with zero coordinates function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec4) vector with zero coordinates.
       */
      static vec4 Zero( VOID )
      {
        return vec4(0);
      } /* End of 'Zero' function */

      /* Get vector with random coordinates (range [0; 1]) function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec4) vector with random coordinates.
       */
      static vec4 Rnd0( VOID )
      {
        return vec4(static_cast<type>(rand()) / RAND_MAX,
                    static_cast<type>(rand()) / RAND_MAX,
                    static_cast<type>(rand()) / RAND_MAX,
                    static_cast<type>(rand()) / RAND_MAX);
      } /* End of 'Rnd0' function */

      /* Get vector with random coordinates (range [-1; 1]) function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec4) vector with random coordinates.
       */
      static vec4 Rnd1( VOID )
      {
        return vec4(2 * (static_cast<type>(rand()) / RAND_MAX) - 1,
                    2 * (static_cast<type>(rand()) / RAND_MAX) - 1,
                    2 * (static_cast<type>(rand()) / RAND_MAX) - 1,
                    2 * (static_cast<type>(rand()) / RAND_MAX) - 1);
      } /* End of 'Rnd1' function */

    }; /* End of 'vec4' class */

} /* end of 'mth' namespace */

#endif /* __mth_vec4_h_ */

/* END OF 'mth_vec4.h' FILE */
