#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"

namespace Skanim
{
	/** Quaternion class
	 */
	class Quaternion
	{
	public:
		SKANIM_API Quaternion() = default;

		SKANIM_API Quaternion(float w, float x, float y, float z) noexcept
			: m_w(w), m_x(x), m_y(y), m_z(z)
		{}

		SKANIM_API explicit Quaternion(float c[4]) noexcept
		{
			for (int i = 0; i < 4; ++i) m_c[i] = c[i];
		}

		SKANIM_API float getW() const
		{
			return m_w; 
		}

		SKANIM_API void setW(float val)
		{
			m_w = val; 
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

		/** Get the components array address of this quaternion.
		 *  The order is w, x, y, z
		 */
		SKANIM_API float *getPtr()
		{
			return m_c;
		}

		SKANIM_API float &operator[](size_t i)
		{
			return m_c[i];
		}

		SKANIM_API Quaternion operator*(float s) const
		{
			return Quaternion(m_w * s, m_x * s, m_y * s, m_z * s);
		}

		SKANIM_API Quaternion operator*(const Quaternion &rhs) const
		{
			return Quaternion(m_w * rhs.m_z + m_z * rhs.m_w + m_x * rhs.m_y - m_y * rhs.m_x,
				m_w * rhs.m_w - m_x * rhs.m_x - m_y * rhs.m_y - m_z * rhs.m_z,
				m_w * rhs.m_x + m_x * rhs.m_w + m_y * rhs.m_z - m_z * rhs.m_y,
				m_w * rhs.m_y + m_y * rhs.m_w + m_z * rhs.m_x - m_x * rhs.m_z
				);
		}

		SKANIM_API bool operator==(const Quaternion &rhs) const
		{
			return m_w == rhs.m_w && m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z;
		}

		/** Calculate and return the inverse of this quaternion.
		 */
		SKANIM_API Quaternion inversed() const
		{
			float norm = this->norm();
			float inv_norm = 1.0f / norm;
			return Quaternion(m_w * inv_norm, -m_x * inv_norm, -m_y * inv_norm, -m_z * inv_norm);
		}

		/** Inverse this quaternion.
		 */
		SKANIM_API void inverse()
		{
			*this = inversed();
		}

		/** Calculate the norm of this quaternion.
		 */
		SKANIM_API float norm() const
		{
			return  m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
		}

		/** Calculate and return the normalized quaternion of this quaternion.
		 */
		SKANIM_API Quaternion normalized() const
		{
			float norm = this->norm();
			float inv_norm = 1.0f / sqrtf(norm);
			return *this * inv_norm;
		}

		/** Normalize this quaternion.
		 */
		SKANIM_API void normalize()
		{
			*this = normalized();
		}

		SKANIM_API Quaternion conjugate() const
		{
			return Quaternion(m_w, -m_x, -m_y, -m_z);
		}



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
};
