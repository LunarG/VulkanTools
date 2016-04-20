/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Chia-I Wu <olvaffe@gmail.com>
 *
 */

#include "icd-utils.h"

/* stolen from Mesa */
uint16_t u_float_to_half(float f)
{
    union fi {
        float f;
        uint32_t ui;
    };

   uint32_t sign_mask  = 0x80000000;
   uint32_t round_mask = ~0xfff;
   uint32_t f32inf = 0xff << 23;
   uint32_t f16inf = 0x1f << 23;
   uint32_t sign;
   union fi magic;
   union fi f32;
   uint16_t f16;

   magic.ui = 0xf << 23;

   f32.f = f;

   /* Sign */
   sign = f32.ui & sign_mask;
   f32.ui ^= sign;

   if (f32.ui == f32inf) {
      /* Inf */
      f16 = 0x7c00;
   } else if (f32.ui > f32inf) {
      /* NaN */
      f16 = 0x7e00;
   } else {
      /* Number */
      f32.ui &= round_mask;
      f32.f  *= magic.f;
      f32.ui -= round_mask;

      /*
       * Clamp to max finite value if overflowed.
       * OpenGL has completely undefined rounding behavior for float to
       * half-float conversions, and this matches what is mandated for float
       * to fp11/fp10, which recommend round-to-nearest-finite too.
       * (d3d10 is deeply unhappy about flushing such values to infinity, and
       * while it also mandates round-to-zero it doesn't care nearly as much
       * about that.)
       */
      if (f32.ui > f16inf)
         f32.ui = f16inf - 1;

      f16 = f32.ui >> 13;
   }

   /* Sign */
   f16 |= sign >> 16;

   return f16;
}
