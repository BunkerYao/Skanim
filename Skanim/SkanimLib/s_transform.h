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
            // Change translation based on B's quaternion & scale
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

