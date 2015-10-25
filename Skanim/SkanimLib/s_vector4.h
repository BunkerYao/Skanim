#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_math.h"

namespace Skanim
{
	/** A class for vector in 4-dimensional space.
	*/
	class _SKANIM_EXPORT Vector4
	{
	public:
		Vector4() = default;

		Vector4(float x, float y, float z, float w) noexcept
			: m_x(x), m_y(y), m_z(z), m_w(w)
		{}

		explicit Vector4(float c[4]) noexcept
		{
			for (int i = 0; i < 4; ++i) m_c[i] = c[i];
		}

		float getX() const
		{
			return m_x;
		}

		void setX(float val)
		{
			m_x = val;
		}

		float getY() const
		{
			return m_y;
		}

		void setY(float val)
		{
			m_y = val;
		}

		float getZ() const
		{
			return m_z;
		}

		void setZ(float val)
		{
			m_z = val;
		}

		float getW() const
		{
			return m_w;
		}

		void setW(float val)
		{
			m_w = val;
		}

		/** Get the components array address of this vector.
		 *  The order is x, y, z, w
		 */
		float *getPtr()
		{
			return m_c;
		}

		float &operator[](size_t i)
		{
			return m_c[i];
		}

		Vector4 operator+(const Vector4 &other) const
		{
			return Vector4(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
		}

		Vector4 operator-(const Vector4 &other) const
		{
			return Vector4(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
		}

		Vector4 operator*(float s) const
		{
			return Vector4(m_x * s, m_y * s, m_z * s, m_w * s);
		}

		Vector4 operator/(float s) const
		{
			return Vector4(m_x / s, m_y / s, m_z / s, m_w / s);
		}

		Vector4 &operator+=(const Vector4 &other)
		{
			*this = (*this) + other;
			return *this;
		}

		Vector4 &operator-=(const Vector4 &other)
		{
			*this = (*this) - other;
			return *this;
		}

		Vector4 operator-()
		{
			return Vector4(-m_x, -m_y, -m_z, -m_w);
		}

		bool operator==(const Vector4 &rhs) const
		{
			return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z && m_w == rhs.m_w;
		}

		float squareMagnitude(void) const
		{
			return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
		}

		float magnitude(void) const
		{
			return sqrtf(squareMagnitude());
		}

		/** Calculate and return a normalized vector of this vector.
		*/
		Vector4 normalized(void) const
		{
			float InvMag = 1.0f / magnitude();
			return (*this) * InvMag;
		}

		/** Normalize this vector.
		*/
		Vector4 normalize(void)
		{
			*this = normalized();
		}

		/** Get a string description of this vector.
		*/
		std::wstring toString() const
		{
			std::wstring str;
			str = L"(" + std::to_wstring(m_x) + L", " +
				std::to_wstring(m_y) + L", " +
				std::to_wstring(m_z) + L"," +
				std::to_wstring(m_w) + L")";
			return str;
		}

		/** Calculate the dot product of two vector.
		*/
		static float dot(const Vector4 lhs, const Vector4 &rhs)
		{
			return lhs.m_x * rhs.m_x + lhs.m_y * rhs.m_y + lhs.m_z * rhs.m_z + lhs.m_w * rhs.m_w;
		}

		/** Calculate a Lerped vector by factor t.
		*/
		static Vector4 lerp(float t, const Vector4 &from, const Vector4 &to)
		{
			return Vector4(Math::lerp(t, from.m_x, to.m_x),
				Math::lerp(t, from.m_y, to.m_y),
				Math::lerp(t, from.m_z, to.m_z),
				Math::lerp(t, from.m_w, to.m_w));
		}

		/** Get a zero vector which has all components set to zero.
		*/
		static const Vector4 &ZERO()
		{
			static const Vector4 ZERO(0.0f, 0.0f, 0.0f, 0.0f);
			return ZERO;
		}

		/** Get a one vector which has all component set to one.
		*/
		static const Vector4 &ONE()
		{
			static const Vector4 ONE(1.0f, 1.0f, 1.0f, 1.0f);
			return ONE;
		}

		/** Get unit vector on x-axis.
		*/
		static const Vector4 &UNIT_X()
		{
			static const Vector4 UNIT_X(1.0f, 0.0f, 0.0f, 0.0f);
			return UNIT_X;
		}

		/** Get unit vector on y-axis.
		*/
		static const Vector4 &UNIT_Y()
		{
			static const Vector4 UNIT_Y(0.0f, 1.0f, 0.0f, 0.0f);
			return UNIT_Y;
		}

		/** Get unit vector on z-axis.
		*/
		static const Vector4 &UNIT_Z()
		{
			static const Vector4 UNIT_Z(0.0f, 0.0f, 1.0f, 0.0f);
			return UNIT_Z;
		}

		friend Vector4 operator*(float s, const Vector4 &other);
		friend Vector4 operator/(float s, const Vector4 &other);

	private:
		union
		{
			struct
			{
				float m_x, m_y, m_z, m_w;
			};

			float m_c[4];
		};
	};

	inline _SKANIM_EXPORT Vector4 operator*(float s, const Vector4 &other)
	{
		return Vector4(other.m_x * s, other.m_y * s, other.m_z * s, other.m_w * s);
	}

	inline _SKANIM_EXPORT Vector4 operator/(float s, const Vector4 &other)
	{
		return Vector4(other.m_x / s, other.m_y / s, other.m_z / s, other.m_w / s);
	}
};