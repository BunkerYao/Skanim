#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_quaternion.h"
#include "s_vector3.h"
#include "s_vector4.h"

namespace Skanim
{
	/** 4x4 row major uniform scale affine transform matrix class.
	 */
	class _SKANIM_EXPORT MatrixUA4
	{
	public:
		MatrixUA4() = default;

		MatrixUA4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
			) noexcept
			: m_00(m00), m_01(m01), m_02(m02), m_03(m03),
			  m_10(m10), m_11(m11), m_12(m12), m_13(m13),
			  m_20(m20), m_21(m21), m_22(m22), m_23(m23),
			  m_30(m30), m_31(m31), m_32(m32), m_33(m33)
		{}

		explicit MatrixUA4(float m[16]) noexcept
		{
			std::memcpy(m_a, m, sizeof(float) * 16);
		}

		float *operator[](size_t r)
		{
			return &m_a[r][0];
		}

		float *getPtr()
		{
			return &m_a[0][0];
		}

		/** Affine transform matrices multiplication.
		 */
		MatrixUA4 operator*(const MatrixUA4 &rhs) const
		{
			return MatrixUA4(
				m_a[0][0] * rhs.m_a[0][0] + m_a[0][1] * rhs.m_a[1][0] + m_a[0][2] * rhs.m_a[2][0],
				m_a[0][0] * rhs.m_a[0][1] + m_a[0][1] * rhs.m_a[1][1] + m_a[0][2] * rhs.m_a[2][1],
				m_a[0][0] * rhs.m_a[0][2] + m_a[0][1] * rhs.m_a[1][2] + m_a[0][2] * rhs.m_a[2][2],
				0.0f,

				m_a[1][0] * rhs.m_a[0][0] + m_a[1][1] * rhs.m_a[1][0] + m_a[1][2] * rhs.m_a[2][0],
				m_a[1][0] * rhs.m_a[0][1] + m_a[1][1] * rhs.m_a[1][1] + m_a[1][2] * rhs.m_a[2][1],
				m_a[1][0] * rhs.m_a[0][2] + m_a[1][1] * rhs.m_a[1][2] + m_a[1][2] * rhs.m_a[2][2],
				0.0f,

				m_a[2][0] * rhs.m_a[0][0] + m_a[2][1] * rhs.m_a[1][0] + m_a[2][2] * rhs.m_a[2][0],
				m_a[2][0] * rhs.m_a[0][1] + m_a[2][1] * rhs.m_a[1][1] + m_a[2][2] * rhs.m_a[2][1],
				m_a[2][0] * rhs.m_a[0][2] + m_a[2][1] * rhs.m_a[1][2] + m_a[2][2] * rhs.m_a[2][2],
				0.0f,

				m_a[3][0] * rhs.m_a[0][0] + m_a[3][1] * rhs.m_a[1][0] + m_a[3][2] * rhs.m_a[2][0] + rhs.m_a[3][0],
				m_a[3][0] * rhs.m_a[0][1] + m_a[3][1] * rhs.m_a[1][1] + m_a[3][2] * rhs.m_a[2][1] + rhs.m_a[3][1],
				m_a[3][0] * rhs.m_a[0][2] + m_a[3][1] * rhs.m_a[1][2] + m_a[3][2] * rhs.m_a[2][2] + rhs.m_a[3][2],
				1.0f
				);
		}

		/** Affine transform matrices multiplication.
		*/
		MatrixUA4 operator*=(const MatrixUA4 &rhs)
		{
			*this = *this * rhs;
		}

		bool operator==(const MatrixUA4 &rhs) const
		{
			return m_a[0][0] == rhs.m_a[0][0] && m_a[0][1] == rhs.m_a[0][1] && m_a[0][2] == rhs.m_a[0][2] && m_a[0][3] == rhs.m_a[0][3] &&
				m_a[1][0] == rhs.m_a[1][0] && m_a[1][1] == rhs.m_a[1][1] && m_a[1][2] == rhs.m_a[1][2] && m_a[1][3] == rhs.m_a[1][3] &&
				m_a[2][0] == rhs.m_a[2][0] && m_a[2][1] == rhs.m_a[2][1] && m_a[2][2] == rhs.m_a[2][2] && m_a[2][3] == rhs.m_a[2][3] &&
				m_a[3][0] == rhs.m_a[3][0] && m_a[3][1] == rhs.m_a[3][1] && m_a[3][2] == rhs.m_a[3][2] && m_a[3][3] == rhs.m_a[3][3];
		}

		Vector3 transformPoint(const Vector3 &p) const
		{
			return Vector3(
				p.getX() * m_00 + p.getY() * m_10 + p.getZ() * m_20 + m_30,
				p.getX() * m_01 + p.getY() * m_11 + p.getZ() * m_21 + m_31,
				p.getX() * m_02 + p.getY() * m_12 + p.getZ() * m_22 + m_32
				);
		}

		Vector3 transformVector(const Vector3 &v) const
		{
			return Vector3(
				v.getX() * m_00 + v.getY() * m_10 + v.getZ() * m_20,
				v.getX() * m_01 + v.getY() * m_11 + v.getZ() * m_21,
				v.getX() * m_02 + v.getY() * m_12 + v.getZ() * m_22
				);
		}

		Vector4 transform(const Vector4 &v) const
		{
			return Vector4(
				v.getX() * m_00 + v.getY() * m_10 + v.getZ() * m_20 + v.getW() * m_30,
				v.getX() * m_01 + v.getY() * m_11 + v.getZ() * m_21 + v.getW() * m_31,
				v.getX() * m_02 + v.getY() * m_12 + v.getZ() * m_22 + v.getW() * m_32,
				v.getW()
				);
		}

		/** Calculate and return the transpose of this matrix.
		 */
		MatrixUA4 transpose() const
		{
			return MatrixUA4(
				m_a[0][0], m_a[1][0], m_a[2][0], m_a[3][0],
				m_a[0][1], m_a[1][1], m_a[2][1], m_a[3][1],
				m_a[0][2], m_a[1][2], m_a[2][2], m_a[3][2],
				m_a[0][3], m_a[1][3], m_a[2][3], m_a[3][3]
				);
		}

		/** Calculate and return the inverse of this affine transform matrix.
		 */
		MatrixUA4 inverse() const
		{
			// Fast invert an affine transform matrix:
			// M = | P  0 |      M^-1 = | P^-1		0 |
			//   = | v  1 |             | -v(P^-1)  1 |

			// Calculate square inverse scale from the first row since the matrix only contains uniform scale.
			float sqr_inv_scale = 1.0f / (m_a[0][0] * m_a[0][0] + m_a[0][1] * m_a[0][1] + m_a[0][2] * m_a[0][2]);

			// The upper-left corner 3x3 matrix contains rotation and uniform scale.
			// To calculate its inverse, first transpose it and then multiply it by square inverse scale.
			MatrixUA4 matrix(
				m_a[0][0], m_a[1][0], m_a[2][0],	0.0f,
				m_a[0][1], m_a[1][1], m_a[2][1],	0.0f,
				m_a[0][2], m_a[1][2], m_a[2][2],	0.0f,
				     0.0f,      0.0f,      0.0f,	1.0f
				);

			matrix.m_a[0][0] *= sqr_inv_scale;
			matrix.m_a[0][1] *= sqr_inv_scale;
			matrix.m_a[0][2] *= sqr_inv_scale;

			matrix.m_a[1][0] *= sqr_inv_scale;
			matrix.m_a[1][1] *= sqr_inv_scale;
			matrix.m_a[1][2] *= sqr_inv_scale;

			matrix.m_a[2][0] *= sqr_inv_scale;
			matrix.m_a[2][1] *= sqr_inv_scale;
			matrix.m_a[2][2] *= sqr_inv_scale;

			// Calculate translation, which is -v(P^-1).
			matrix.m_a[3][0] = -(m_a[3][0] * matrix.m_a[0][0] + m_a[3][1] * matrix.m_a[1][0] + m_a[3][2] * matrix.m_a[2][0]);
			matrix.m_a[3][1] = -(m_a[3][0] * matrix.m_a[0][1] + m_a[3][1] * matrix.m_a[1][1] + m_a[3][2] * matrix.m_a[2][1]);
			matrix.m_a[3][2] = -(m_a[3][0] * matrix.m_a[0][2] + m_a[3][1] * matrix.m_a[1][2] + m_a[3][2] * matrix.m_a[2][2]);

			return matrix;
		}

		/** Calculate and return the scale of this matrix.
		 */
		float getScale() const
		{
			return sqrtf(m_a[0][0] * m_a[0][0] + m_a[0][1] * m_a[0][1] + m_a[0][2] * m_a[0][2]);
		}

		/** Calculate and return a quaternion which represents the rotation in this matrix.
		 */
		Quaternion getQuaternion() const
		{
			// Re-normalize the upper-left corner 3x3 matrix first.
			MatrixUA4 matrix;
			float inv_scale = 1.0f / getScale();

			matrix.m_00 = m_00 * inv_scale;
			matrix.m_01 = m_01 * inv_scale;
			matrix.m_02 = m_02 * inv_scale;
			matrix.m_10 = m_10 * inv_scale;
			matrix.m_11 = m_11 * inv_scale;
			matrix.m_12 = m_12 * inv_scale;
			matrix.m_20 = m_20 * inv_scale;
			matrix.m_21 = m_21 * inv_scale;
			matrix.m_22 = m_22 * inv_scale;

			// Construct a quaternion from a rotation matrix.
			// Inspired from the OGRE engine source
			// OgreQuaternion.cpp: 
			// https://bitbucket.org/sinbad/ogre/src/2a79e4809a8d37df45fa6db461df6d6353696bd6/OgreMain/src/OgreQuaternion.cpp?at=default
			// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
			// article "Quaternion Calculus and Fast Animation".
			Quaternion q;
			float ftrace = matrix.m_a[0][0] + matrix.m_a[1][1] + matrix.m_a[2][2];
			float froot;

			if (ftrace > 0.0f) {
				// |w| > 1/2
				froot = sqrtf(ftrace + 1.0f);	// 2w
				q.setW(0.5f * froot);
				froot = 0.5f / froot;			// 1/(4w)
				q.setX((matrix.m_a[1][2] - matrix.m_a[2][1]) * froot);
				q.setY((matrix.m_a[2][0] - matrix.m_a[0][2]) * froot);
				q.setZ((matrix.m_a[0][1] - matrix.m_a[1][0]) * froot);
			}
			else {
				// |w| <= 1/2
				static size_t Next[3] = { 1, 2, 0 };
				size_t i = 0;
				if (matrix.m_a[1][1] > matrix.m_a[0][0])
					i = 1;
				if (matrix.m_a[2][2] > matrix.m_a[i][i])
					i = 2;
				size_t j = Next[i];
				size_t k = Next[j];

				froot = sqrtf(matrix.m_a[i][i] - matrix.m_a[j][j] - matrix.m_a[k][k] + 1.0f);
				float *apk_quat[3] = { &q[1], &q[2], &q[3] };
				*apk_quat[i] = 0.5f * froot;
				froot = 0.5f / froot;
				q.setW((matrix.m_a[j][k] - matrix.m_a[k][j]) * froot);
				*apk_quat[j] = (matrix.m_a[i][j] + matrix.m_a[j][i]) * froot;
				*apk_quat[k] = (matrix.m_a[i][k] + matrix.m_a[k][i]) * froot;
			}

			return q;
		}

		/** Calculate and return the translation of this matrix.
		 */
		Vector3 getTranslation() const
		{
			return Vector3(m_30, m_31, m_32);
		}

		/** Construct a matrix from scale.
		 */
		static MatrixUA4 fromScale(float s)
		{
			return MatrixUA4(
				   s, 0.0f, 0.0f, 0.0f,
				0.0f,    s, 0.0f, 0.0f,
				0.0f, 0.0f,    s, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
		}

		std::wstring toString() const
		{
			std::wstring str;
			str = L"(" +
				std::to_wstring(m_00) + L", " + std::to_wstring(m_01) + L", " + std::to_wstring(m_02) + L", " + std::to_wstring(m_03) + L"; " +
				std::to_wstring(m_10) + L", " + std::to_wstring(m_11) + L", " + std::to_wstring(m_12) + L", " + std::to_wstring(m_13) + L"; " +
				std::to_wstring(m_20) + L", " + std::to_wstring(m_21) + L", " + std::to_wstring(m_22) + L", " + std::to_wstring(m_23) + L"; " +
				std::to_wstring(m_30) + L", " + std::to_wstring(m_31) + L", " + std::to_wstring(m_32) + L", " + std::to_wstring(m_33) +
				L")";
			return str;
		}

		/** Construct a matrix from quaternion.
		 */
		static MatrixUA4 fromQuaternion(const Quaternion &q)
		{
			float fx = q.getX() + q.getX();
			float fy = q.getY() + q.getY();
			float fz = q.getZ() + q.getZ();
			float fwx = fx * q.getW();
			float fwy = fy * q.getW();
			float fwz = fz * q.getW();
			float fxx = fx * q.getX();
			float fxy = fy * q.getX();
			float fxz = fz * q.getX();
			float fyy = fy * q.getY();
			float fyz = fz * q.getY();
			float fzz = fz * q.getZ();

			return MatrixUA4(
				1.0f - (fyy + fzz),			 fxy + fwz,		     fxz - fwy,		0.0f,
						 fxy - fwz, 1.0f - (fxx + fzz),	         fyz + fwx,     0.0f,
						 fxz + fwy,		     fyz - fwx, 1.0f - (fxx + fyy),     0.0f,
				              0.0f,               0.0f,               0.0f,     1.0f
				);
		}

		/** Construct a matrix from translation.
		 */
		static MatrixUA4 fromTranslation(const Vector3 &t)
		{
			return MatrixUA4(
				    0.0f,     0.0f,     0.0f,     0.0f,
				    0.0f,     0.0f,     0.0f,     0.0f,
				    0.0f,     0.0f,     0.0f,     0.0f,
				t.getX(), t.getY(), t.getZ(),     1.0f
				);
		}

		/** Construct a matrix from scale, quaternion and translation.
		 */
		static MatrixUA4 fromSQT(float s, const Quaternion &q, const Vector3 &t)
		{
            MatrixUA4 matrix = MatrixUA4::fromQuaternion(q);

			matrix.m_a[0][0] *= s;
			matrix.m_a[0][1] *= s;
			matrix.m_a[0][2] *= s;
			matrix.m_a[1][0] *= s;
			matrix.m_a[1][1] *= s;
			matrix.m_a[1][2] *= s;
			matrix.m_a[2][0] *= s;
			matrix.m_a[2][1] *= s;
			matrix.m_a[2][2] *= s;

			matrix.m_30 = t.getX();
			matrix.m_31 = t.getY();
			matrix.m_32 = t.getZ();

			return matrix;
		}

		/** Get identity matrix
		 */
		static const MatrixUA4 &IDENTITY()
		{
			static const MatrixUA4 IDENTITY(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			return IDENTITY;
		}

	private:
		union
		{
			struct
			{
				float m_00, m_01, m_02, m_03;
				float m_10, m_11, m_12, m_13;
				float m_20, m_21, m_22, m_23;
				float m_30, m_31, m_32, m_33;
			};

			float m_a[4][4];
		};
	};
};
