#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_math.h"
#include "s_vector3.h"

namespace Skanim
{
	/** Quaternion class
	 */
	class _SKANIM_EXPORT Quaternion
	{
	public:
		Quaternion() = default;

		Quaternion(float w, float x, float y, float z) noexcept
			: m_w(w), m_x(x), m_y(y), m_z(z)
		{}

		Quaternion(const Vector3 &axis, float angle) noexcept
		{
			float half_angle = 0.5f * angle;
			float f_sin = sinf(half_angle);
			m_w = cosf(half_angle);
			m_x = f_sin * axis.getX();
			m_y = f_sin * axis.getY();
			m_z = f_sin * axis.getZ();
		}

		explicit Quaternion(float c[4]) noexcept
		{
			for (int i = 0; i < 4; ++i) m_c[i] = c[i];
		}

		float getW() const
		{
			return m_w; 
		}

		void setW(float val)
		{
			m_w = val; 
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

		/** Get the components array address of this quaternion.
		 *  The order is w, x, y, z
		 */
		float *getPtr()
		{
			return m_c;
		}

		float &operator[](size_t i)
		{
			return m_c[i];
		}

		Quaternion operator+(const Quaternion &other) const
		{
			return Quaternion(m_w + other.m_w, m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
		}

		Quaternion operator*(float s) const
		{
			return Quaternion(m_w * s, m_x * s, m_y * s, m_z * s);
		}

        /** Multiplication.
         *  The equivalent rotation order is left to right.
         */
		Quaternion operator*(const Quaternion &rhs) const
		{
			return Quaternion(
				m_w * rhs.m_w - m_x * rhs.m_x - m_y * rhs.m_y - m_z * rhs.m_z,
				m_w * rhs.m_x + m_x * rhs.m_w + m_z * rhs.m_y - m_y * rhs.m_z,
				m_w * rhs.m_y + m_x * rhs.m_z + m_y * rhs.m_w - m_z * rhs.m_x,
				m_w * rhs.m_z + m_y * rhs.m_x + m_z * rhs.m_w - m_x * rhs.m_y
				);
		}

		Quaternion operator-() const
		{
			return Quaternion(-m_w, -m_x, -m_y, -m_z);
		}

		bool operator==(const Quaternion &rhs) const
		{
			return m_w == rhs.m_w && m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z;
		}

		/** Calculate and return the inverse of this quaternion.
		 */
		Quaternion inversed() const
		{
			float norm = this->norm();
			float inv_norm = 1.0f / norm;
			return Quaternion(m_w * inv_norm, -m_x * inv_norm, -m_y * inv_norm, -m_z * inv_norm);
		}

		/** Inverse this quaternion.
		 */
		void inverse()
		{
			*this = inversed();
		}

		/** Calculate the norm of this quaternion.
		 */
		float norm() const
		{
			return  m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
		}

		/** Calculate and return the normalized quaternion of this quaternion.
		 */
		Quaternion normalized() const
		{
			float norm = this->norm();
			float inv_norm = 1.0f / sqrtf(norm);
			return *this * inv_norm;
		}

		/** Normalize this quaternion.
		 */
		void normalize()
		{
			*this = normalized();
		}

		Quaternion conjugate() const
		{
			return Quaternion(m_w, -m_x, -m_y, -m_z);
		}

		/** Get a string description of this quaternion.
		 */
		std::wstring toString() const
		{
			std::wstring str = L"(" + std::to_wstring(m_w) + L", " +
				std::to_wstring(m_x) + L", " +
				std::to_wstring(m_y) + L", " +
				std::to_wstring(m_z) + L")";
			return str;
		}

		/** Calculate quaternion dot product.
		 */
		static float dot(const Quaternion &qa, const Quaternion &qb)
		{
			return qa.m_w * qb.m_w + qa.m_x * qb.m_x + qa.m_y * qb.m_y + qa.m_z * qb.m_z;
		}

		/** Quaternion spherical interpolation.
		 */
		static Quaternion slerp(float t, const Quaternion &from, const Quaternion &to)
		{
			float fcos = Quaternion::dot(from, to);
			Quaternion tq;
			// if the angle between two quaternions is larger than 90, inverse the second
            // quaternion.
			if (fcos < 0.0f) {
				fcos = -fcos;
				tq = -to;
			}
			else {
				tq = to;
			}

			if (fcos < 1.0f - Math::EPSILON()) {
				// Standard case.
				float fsin = sqrtf(1.0f - fcos * fcos);
				float angle = atan2(fsin, fcos);
				float inv_sin = 1.0f / fsin;
				float t0 = sinf((1.0f - t) * angle) * inv_sin;
				float t1 = sinf(t * angle) * inv_sin;
				return from * t0 + tq * t1;
			}
			else {
				// If the two quaternions are almost parallel, simply do linear interpolation.
				Quaternion rq = from * (1.0f - t) + tq * t;
				rq.normalize();
				return rq;
			}
		}

		/** Calculate the difference quaternion between two given unit quaternions.
		 */
		static Quaternion fromTo(const Quaternion &qfrom, const Quaternion &qto)
		{
			// qd = inv_qfrom * qto
			// Since qfrom is a unit quaternion, so its inverse is identical with its 
            // conjugate.
			Quaternion inv_qfrom = qfrom.conjugate();
			return inv_qfrom * qto;
		}

		/** Calculate the rotation quaternion for rotating vfrom to vto.
		 */
		static Quaternion fromTo(const Vector3 &vfrom, const Vector3 &vto)
		{
			Vector3 axis = Vector3::cross(vfrom, vto).normalized();
			float angle = Vector3::angle(vfrom, vto);
			return Quaternion(axis, angle);
		}

		/** Get the zero quaternion which has all components set to 0.
		 */
		static const Quaternion &ZERO()
		{
			static const Quaternion ZERO(0.0f, 0.0f, 0.0f, 0.0f);
			return ZERO;
		}

		/** Get the identity quaternion which represents no rotation.
		 */
		static const Quaternion &IDENTITY()
		{
			static const Quaternion IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);
			return IDENTITY;
		}

        _SKANIM_EXPORT friend Vector3 operator*(const Vector3 &v, const Quaternion &q);

	private:
		union
		{
			struct  
			{
				float m_w, m_x, m_y, m_z;
			};

			float m_c[4];
		};
	};

	/** Vector-Quaternion sandwich multiplication.
     *  q' = q * v * q^-1
	 *  This makes the vector rotated by the quaternion in 3d space.
	 */
	inline _SKANIM_EXPORT Vector3 operator*(const Vector3 &v, const Quaternion &q)
	{
        // The multiply order is inverted compared to the standard since the
        // quaternion multiplication is defined as left to right order in skanim.
        // So here q' = q^-1 * v * q 
		Quaternion vecq(v, Math::PI());
		vecq = q.conjugate() * vecq * q;
		return Vector3(vecq.getX(), vecq.getY(), vecq.getZ());
	}
};
