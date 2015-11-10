#pragma once

#include "s_prerequisites.h"
#include "s_iterator_wrapper.h"
#include "s_transform.h"

namespace Skanim
{
    /** A pose object stores each joint's transform in pre-order.
     */
    class _SKANIM_EXPORT Pose
    {
    public:
        Pose() = default;

        typedef vector<Transform> TransformVector;

        explicit Pose(size_t joint_count) noexcept
            : m_joint_transforms_array(joint_count)
        {}

        explicit Pose(const TransformVector &joint_transforms_array) noexcept
            : m_joint_transforms_array(joint_transforms_array)
        {}

        /** Get joint count of this pose.
         */
        size_t getJointCount() const
        {
            return m_joint_transforms_array.size();
        }

        /** Get the i'th joint's transform.
         */
        Transform &getJointTransform(size_t i)
        {
            assert(i < m_joint_transforms_array.size() && "i out of range");
            return m_joint_transforms_array[i];
        }

        /** Get the i'th joint's transform.
         */
        const Transform &getJointTransform(size_t i) const
        {
            assert(i < m_joint_transforms_array.size() && "i out of range");
            return m_joint_transforms_array[i];
        }

        /** Set the i'th joint's transform
         */
        void setJointTransform(size_t i, const Transform &transform)
        {
            assert(i < m_joint_transforms_array.size() && "i out of range");
            m_joint_transforms_array[i] = transform;
        }

        /** Get the i'th joint's transform by subscript. 
         */
        Transform &operator[](size_t i)
        {
            return getJointTransform(i);
        }

        /** Get the i'th joint's transform by subscript.
        */
        const Transform &operator[](size_t i) const
        {
            return getJointTransform(i);
        }

    private:
        typedef TransformVector::iterator _TransformVectorItor;
        typedef TransformVector::const_iterator _TransformVectorConstItor;

        // A vector stores joint transform in pre-order.
        TransformVector m_joint_transforms_array;
    };
};
