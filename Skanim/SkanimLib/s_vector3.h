#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_math.h"

namespace Skanim
{
	/** A class for vector in 3-dimensional space.
	 */
	class Vector3
	{
	public:
		SKANIM_API Vector3() = default;

		SKANIM_API Vector3(float x, float y, float z) noexcept
			: m_x(x), m_y(y), m_z(z)
		{}

		SKANIM_API explicit Vector3(float c[3]) noexcept
		{
			for (int i = 0; i < 3; ++i) m_c[i] = c[i];
		}

		SKANIM_API float getX() const
		{
			return m_x; 
		}

		SKANIM_API void setX(float val)
		{
			m_x = val; 
		}

		SKANIM_API float getY() const
		{
			return m_y;
		}

		SKANIM_API void setY(float val)
		{
			m_y = val; 
		}

		SKANIM_API float getZ() const
		{
			return m_z; 
		}

		SKANIM_API void setZ(float val)
		{
			m_z = val; 
		}

		/** Get the components array address of this vector.
		 */
		SKANIM_API float *getPtr()
		{
			return m_c;
		}

		SKANIM_API float &operator[](size_t i)
		{
			return m_c[i];
		}

		SKANIM_API Vector3 operator+(const Vector3 &other) const
		{
			return Vector3(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
		}

		SKANIM_API Vector3 operator-(const Vector3 &other) const
		{
			return Vector3(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
		}

		SKANIM_API Vector3 operator*(float s) const
		{
			return Vector3(m_x * s, m_y * s, m_z * s);
		}

		SKANIM_API Vector3 operator/(float s) const
		{
			return Vector3(m_x / s, m_y / s, m_z / s);
		}

		SKANIM_API Vector3 &operator+=(const Vector3 &other)
		{
			*this = (*this) + other;
			return *this;
		}

		SKANIM_API Vector3 &operator-=(const Vector3 &other)
		{
			*this = (*this) - other;
			return *this;
		}

		SKANIM_API bool operator==(const Vector3 &rhs) const
		{
			return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z;
		}

		SKANIM_API float squareMagnitude(void) const
		{
			return m_x * m_x + m_y * m_y + m_z * m_z;
		}

		SKANIM_API float magnitude(void) const
		{
			return sqrtf(magnitude());
		}

		/** Calculate and return a normalized vector of this vector.
		 */
		SKANIM_API Vector3 normalized(void) const
		{
			float InvMag = 1.0f / magnitude();
			return (*this) * InvMag;
		}

		/** Normalize this vector.
		*/
		SKANIM_API Vector3 normalize(void)
		{
			*this = normalized();
		}

		/** Get a string description of this vector.
		 */
		SKANIM_API std::wstring toString() const
		{
			std::wstring str;
			str = L"(" + std::to_wstring(m_x) + L", " +
				std::to_wstring(m_y) + L", " +
				std::to_wstring(m_z) + L")";
			return str;
		}

		/** Calculate the dot product of two vector.
		 */
		static SKANIM_API float dot(const Vector3 lhs, const Vector3 &rhs)
		{
			return lhs.m_x * rhs.m_x + lhs.m_y * rhs.m_y + lhs.m_z * rhs.m_z;
		}

		/** Calculate the cross product of two vector.
		 */
		static SKANIM_API Vector3 cross(const Vector3 &lhs, const Vector3 &rhs)
		{
			return Vector3(lhs.m_y * rhs.m_z - lhs.m_z * rhs.m_y,
				lhs.m_z * rhs.m_x - lhs.m_x * rhs.m_z,
				lhs.m_x * rhs.m_y - lhs.m_y * rhs.m_x);
		}

		/** Calculate a Lerped vector by factor t.
		 */
		static SKANIM_API Vector3 lerp(float t, const Vector3 &from, const Vector3 &to)
		{
			return Vector3(Math::lerp(t, from.m_x, to.m_x),
				Math::lerp(t, from.m_y, to.m_y),
				Math::lerp(t, from.m_z, to.m_z));
		}

		/** Calculate angle in radians between two vector.
		 * @param a Unit vector a.
		 * @param b Unit vector b.
		 */
		static SKANIM_API float angle(const Vector3 &a, const Vector3 &b)
		{
			return acosf(dot(a, b));
		}

		/** Get a zero vector which has all components set to zero.
		 */
		static SKANIM_API const Vector3 &ZERO()
		{
			static const Vector3 ZERO(0.0f, 0.0f, 0.0f);
			return ZERO;
		}

		/** Get a one vector which has all component set to one.
		 */
		static SKANIM_API const Vector3 &ONE()
		{
			static const Vector3 ONE(1.0f, 1.0f, 1.0f);
			return ONE;
		}

		/** Get unit vector on x-axis.
		 */
		static SKANIM_API const Vector3 &UNIT_X()
		{
			static const Vector3 UNIT_X(1.0f, 0.0f, 0.0f);
			return UNIT_X;
		}

		/** Get unit vector on y-axis.
		 */
		static SKANIM_API const Vector3 &UNIT_Y()
		{
			static const Vector3 UNIT_Y(0.0f, 1.0f, 0.0f);
			return UNIT_Y;
		}

		/** Get unit vector on z-axis.
		 */
		static SKANIM_API const Vector3 &UNIT_Z()
		{
			static const Vector3 UNIT_Z(0.0f, 0.0f, 1.0f);
			return UNIT_Z;
		}

		friend Vector3 operator-(const Vector3 &other);
		friend Vector3 operator*(float s, const Vector3 &other);
		friend Vector3 operator/(float s, const Vector3 &other);

	private:
		union 
		{
			struct
			{
				float m_x, m_y, m_z;
			};

			float m_c[3];
		};
	};

	inline SKANIM_API Vector3 operator-(const Vector3 &other)
	{
		return Vector3(-other.m_x, -other.m_y, -other.m_z);
	}

	inline SKANIM_API Vector3 operator*(float s, const Vector3 &other)
	{
		return Vector3(other.m_x * s, other.m_y * s, other.m_z * s);
	}

	inline SKANIM_API Vector3 operator/(float s, const Vector3 &other)
	{
		return Vector3(other.m_x / s, other.m_y / s, other.m_z / s);
	}
};