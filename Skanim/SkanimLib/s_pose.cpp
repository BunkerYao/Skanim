#include "s_precomp.h"
#include "s_pose.h"

namespace Skanim
{
    void Pose::lerp(float t, const Pose &a, const Pose &b, Pose *lerped_pose)
    {
        assert(a.getJointCount() == b.getJointCount() && "joint count differs");

        const size_t joint_count = a.getJointCount();

        lerped_pose->m_joint_transforms_array.resize(joint_count);

        for (size_t i_joint = 0; i_joint < joint_count; ++i_joint) {
            const Transform &ref_joint_transform_a = 
                a.m_joint_transforms_array[i_joint];
            const Transform &ref_joint_transform_b = 
                b.m_joint_transforms_array[i_joint];
            lerped_pose->m_joint_transforms_array[i_joint] =
                Transform::lerp(t, ref_joint_transform_a, ref_joint_transform_b);
        }
    }

};