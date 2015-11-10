#include "s_precomp.h"
#include "s_skeleton.h"
#include "s_joint.h"
#include "s_pose.h"

namespace Skanim
{
    Skeleton::Skeleton(Joint *root) noexcept
        : m_is_root_motion_enabled(true)
    {
        // The joint being added shouldn't has parent.
        assert(!root->m_parent && "the root has a parent");

        _addJointRecursive(root);
    }

    Skeleton::~Skeleton()
    {
        _clear();
    }

    Skeleton::JointPtrIterator Skeleton::getRootIterator()
    {
        return JointPtrIterator(m_joint_list.begin(), m_joint_list.end());
    }

    Joint *Skeleton::findJoint(const String &name) const
    {
        _ConstJointNamesMapItor itor_joint_pair = m_joint_names_map.find(name);
        if (m_joint_names_map.cend() == itor_joint_pair)
            return nullptr;
        else
            return *itor_joint_pair->second;
    }

    Joint *Skeleton::getRoot() const
    {
        return m_joint_list.front();
    }

    size_t Skeleton::getJointCount() const
    {
        return m_joint_list.size();
    }

    void Skeleton::setPose(const Pose &pose)
    {
        const size_t joint_transform_count = pose.getJointCount();

        if (0 == joint_transform_count)
            return;

        // Update root's transform.
        if (m_is_root_motion_enabled) 
            _updateRootTransform(pose[0]);

        // Update other joints' transforms.
        _updateOtherJointsFromLclPose(pose);
    }

    void Skeleton::_onJointAdded(Joint *new_joint)
    {
        _addJointRecursive(new_joint);
    }

    void Skeleton::_onJointRemoved(Joint *removed_joint)
    {
        _removeJointRecursive(removed_joint);
    }

    void Skeleton::_onJointRenamed(const String &old_name, const String &current_name)
    {
        _renameJointImpl(old_name, current_name);
    }

    void Skeleton::_clear()
    {
        if (!m_joint_list.empty()) {
            SKANIM_DELETE_T(Joint, m_joint_list.front());
            m_joint_list.clear();
            m_joint_names_map.clear();
        }
    }

    void Skeleton::_addJointRecursive(Joint *new_joint)
    {
        // The joint being added shouldn't belongs to other skeleton.
        assert(!new_joint->getOwnerSkeleton() && "the joint belongs to another skeleton");
        // Set new joint's owner skeleton.
        new_joint->_setOwnerSkeleton(this);

        // If the new joint has no parent then it must be the root joint and add it to the list directly.
        // Otherwise we need to insert the new joint to the proper position.
        _JointPtrListItor itor_new_joint;
        if (!new_joint->m_parent) {
            m_joint_list.push_back(new_joint);
            itor_new_joint = m_joint_list.end();
            --itor_new_joint;
        }
        else {
            // The fact is that the joint follows its left sibling or parent in pre-order.
            const Joint *p_left_sibling = new_joint->_findLeftSibling();
            // The name of the previous joint the new joint follows, it should be either parent's name
            // or the most right bottom joint of its left sibling.
            String prev_joint_name;
            if (p_left_sibling) {
                const Joint *p_prev = p_left_sibling;
                while (p_prev->m_last_child) {
                    p_prev = p_prev->m_last_child;
                }
                prev_joint_name = p_prev->getName();
            }
            else {
                prev_joint_name = new_joint->m_parent->getName();
            }
            // Find the previous joint's iterator in the names map use prev_joint_name.
            _JointPtrListItor itor_prev_joint = m_joint_names_map.find(prev_joint_name)->second;
            // Insert the new joint right behind.
            itor_new_joint = itor_prev_joint;
            ++itor_new_joint;
            itor_new_joint = m_joint_list.insert(itor_new_joint, new_joint);
        }

        // Add the new joint to the names map.
        // Check if there is a name collision.
        assert(m_joint_names_map.find(new_joint->getName()) == m_joint_names_map.end() && "joint name collision");
        m_joint_names_map.insert(std::make_pair(new_joint->getName(), itor_new_joint));

        // Recursively add its children.
        Joint *p_joint = new_joint->m_child;
        while (p_joint) {
            _addJointRecursive(p_joint);
            p_joint = p_joint->m_sibling;
        }
    }

    void Skeleton::_removeJointRecursive(Joint *remove_joint)
    {
        // find the iterator of the joint need to be removed by searching the names map.
        _JointNamesMapItor itor_remove_joint_pair = m_joint_names_map.find(remove_joint->getName());
        _JointPtrListItor itor_remove_joint = itor_remove_joint_pair->second;
        // Remove the joint in the joint list.
        m_joint_list.erase(itor_remove_joint);
        // Remove the record in the names map.
        m_joint_names_map.erase(itor_remove_joint_pair);

        // Update the owner skeleton of this joint.
        remove_joint->_setOwnerSkeleton(nullptr);

        // Recursively remove its children.
        Joint *p_joint = remove_joint->m_child;
        while (p_joint) {
            _removeJointRecursive(p_joint);
            p_joint = p_joint->m_sibling;
        }
    }

    void Skeleton::_renameJointImpl(const String &old_name, const String &current_name)
    {
        // Need to delete the old record and insert a new one.
        assert(m_joint_names_map.find(current_name) == m_joint_names_map.end() && "joint name collision");
        _JointNamesMapItor itor_renamed_joint_pair = m_joint_names_map.find(old_name);
        _JointPtrListItor itor_renamed_joint = itor_renamed_joint_pair->second;
        m_joint_names_map.erase(itor_renamed_joint_pair);
        m_joint_names_map.insert(std::make_pair(current_name, itor_renamed_joint));
    }

    void Skeleton::_updateRootTransform(const Transform &delta_transform)
    {
        // Update root's transform by combining the delta root transform in the given pose with
        // the root joint's old transform.
        Transform &root_lcl_transform = m_joint_list.front()->m_lcl_transform;
        Transform &root_glb_transform = m_joint_list.front()->m_glb_transform;
        root_lcl_transform = Transform::combine(delta_transform, root_lcl_transform);
        root_glb_transform = root_lcl_transform;
    }

    void Skeleton::_updateOtherJointsFromLclPose(const Pose &pose)
    {
        // Iterate each joint through the list to update thier transform.
        _JointPtrListItor itor_joint = m_joint_list.begin();
        // Skip the root joint.
        itor_joint++;

        // The number of joint transform in the given pose might be fewer, more or just the same compared to
        // the number of joints in joint list.
        size_t i_joint = 1;
        while (i_joint < pose.getJointCount() && itor_joint != m_joint_list.end()) {
            Joint *p_joint = *itor_joint;
            // Update the local transform.
            p_joint->m_lcl_transform = pose[i_joint];
            // Update the global transform.
            p_joint->m_glb_transform = Transform::combine(p_joint->m_lcl_transform,
                p_joint->m_parent->m_glb_transform);

            ++i_joint;
            ++itor_joint;
        }

        // If there are more joints in the list be left, update their global transforms.
        while (itor_joint != m_joint_list.end()) {
            Joint *p_joint = *itor_joint;
            // Update the global transform.
            p_joint->m_glb_transform = Transform::combine(p_joint->m_lcl_transform,
                p_joint->m_parent->m_glb_transform);
            ++itor_joint;
        }
    }
};