#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"

namespace Skanim
{
	/** Utility class for math computation.
	 */
	class _SKANIM_EXPORT Math
	{
	public:
		/** Linear interpolation
		 */
		static float lerp(float t, float from, float to)
		{
			return from + (to - from) * t;
		}

        /** Clamp the given value between the range.
         */
        static int clamp(int value, int lbound, int rbound)
        {
            if (value < lbound) return lbound;
            else if (value > rbound) return rbound;
            else return value;
        }

		/** Clamp the given value between the range.
		 */
		static float clamp(float value, float lbound, float rbound)
		{
			if (value < lbound) return lbound;
			else if (value > rbound) return rbound;
			else return value;
		}

		/** Clamp the given value between 0 and 1.
		 */
		static float clamp01(float value)
		{
			if (value < 0.0f) return 0.0f;
			else if (value > 1.0f) return 1.0f;
			else return value;
		}

		/** Get constant PI
		 */
		static float PI()
		{
			static const float PI = 3.141592654f;
			return PI;
		}

		/** Get half of PI
		 */
		static float HALF_PI()
		{
			static const float HALF_PI = 1.570796f;
			return HALF_PI;
		}

		/** Get PI multiplied by 2
		 */
		static float PI_2()
		{
			static const float PI_2 = 6.2831853f;
			return PI_2;
		}

        static float EPSILON()
        {
            static const float EPSILON = 1e-04f;
            return EPSILON;
        }

	};

};
