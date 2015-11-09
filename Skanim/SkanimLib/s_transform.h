#pragma once

#include "s_prerequisites.h"
#include "s_matrixua4.h"
#include "s_quaternion.h"
#include "s_vector3.h"

namespace Skanim
{
    /** A class that represents 3d world transformation.
     *  The transformation is stored in SQT style, which is 
     *  scale(uniform), quaternion and translation.
     */
    class _SKANIM_EXPORT Transform
    {
    public:
        Transform() = default;

        Transform(float s, const Quaternion &q, const Vector3 &t) noexcept
            : m_scale(s), m_rotation(q), m_translation(t)
        {}

        const Quaternion &getRotation() const 
        {
            return m_rotation; 
        }

        Quaternion &getRotation()
        {
            return m_rotation;
        }

        void setRotation(const Quaternion &val)
        {
            m_rotation = val; 
        }

        const Vector3 &getTranslation() const 
        {
            return m_translation; 
        }

        Vector3 &getTranslation()
        {
            return m_translation;
        }

        void setTranslation(const Vector3 &val) 
        {
            m_translation = val; 
        }

        float getScale() const 
        {
            return m_scale; 
        }

        void setScale(float val)
        {
            m_scale = val; 
        }

        /** Transform point with this transform.
         */
        Vector3 transformPoint(const Vector3 &p) const
        {
            return p * m_scale * m_rotation + m_translation;
        }

        /** Transform vector with this transform.
         */
        Vector3 transformVector(const Vector3 &v) const
        {
            return v * m_scale * m_rotation;
        }

        /** Get the inverse transform.
         */
        Transform inversed() const
        {
            return Transform(1.0f / m_scale, m_rotation.conjugate(), -m_translation);
        }

        /** Calculate and then return the matrix representation of this transform.
         */
        MatrixUA4 toMatrix() const
        {
            return MatrixUA4::fromSQT(m_scale, m_rotation, m_translation);
        }

        /** Construct a transform object from matrix.
         */
        static Transform fromMatrix(const MatrixUA4 &m)
        {
            Transform transform;
            transform.m_scale = m.getScale();
            transform.m_rotation = m.getQuaternion();
            transform.m_translation = m.getTranslation();
            return transform;
        }

        /** Linear Interpolation between two transforms
         */
        static Transform lerp(float t, const Transform &from, const Transform &to)
        {
            return Transform(Math::lerp(t, from.m_scale, to.m_scale),
                Quaternion::slerp(t, from.m_rotation, to.m_rotation),
                Vector3::lerp(t, from.m_translation, to.m_translation));
        }

        /** This method combines two transformation A and B. 
         *  The combined transformation is identical to transformation A followed by transformation B.
         */
        static Transform combine(const Transform &a, const Transform &b)
        {
            Transform combined;

            // Combine scales by multiplying A's scale by B's scale.
            combined.m_scale = a.m_scale * b.m_scale;
            // Combine rotations by multiplying A's quaternion by B's quaternion.
            combined.m_rotation = a.m_rotation * b.m_rotation;
            // Change translation based on B's quaternion & scale.
            combined.m_translation = a.m_translation * b.m_scale * b.m_rotation;
            // Add B's translation.
            combined.m_translation += b.m_translation;

            return combined;
        }

        /** Get an identity transform object which has no scale, no rotation 
         *  and contains zero translation.
         */
        static const Transform &IDENTITY()
        {
            static const Transform IDENTITY(1.0f, Quaternion::IDENTITY(), Vector3::ZERO());
            return IDENTITY;
        }

    private:
        Quaternion  m_rotation;
        Vector3     m_translation;
        float       m_scale;
    };
};

