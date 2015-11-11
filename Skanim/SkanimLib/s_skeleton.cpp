#include "s_precomp.h"
#include "s_skeleton.h"
#include "s_joint.h"
#include "s_pose.h"

namespace Skanim
{
    Skeleton::Skeleton(const String &name, const String &root_name) noexcept
        : m_name(name),
          m_is_root_motion_enabled(true),
          m_palette_needs_update(false)
    {
        _addRootJoint(root_name);
    }

    Joint *Skeleton::findJoint(const String &name)
    {
        _JointNamesMapConstIterator itor_joint = m_joint_names_map.find(name);
        if (itor_joint != m_joint_names_map.end()) {
            return getJoint(itor_joint->second);
        }
        return nullptr;
    }

    void Skeleton::addJointPreOrder(const Joint &joint, int parent_index)
    {
        assert(joint.hasParent() == false && 
            "the joint being added already has a parent.");
        assert(joint.getChildrenCount() == 0 && 
            "the joint being added already has children.");
        assert(parent_index < 0 || parent_index >= (int)m_joint_hierarchy_array.size() &&
            "parent_index is not valid.");
        assert(m_joint_names_map.count(joint.getName()) == 0 &&
            "joint name collision.");
        
        Joint new_joint = joint;
        new_joint.setParentIndex(parent_index);
        if (new_joint.getChildrenCount() > 0)
            new_joint.removeAllChildren();
        
        int new_joint_index = m_joint_hierarchy_array.size() - 1;
        Joint &parent_joint = m_joint_hierarchy_array[parent_index];
        parent_joint.addChild(new_joint_index);

        m_joint_hierarchy_array.push_back(joint);
        m_joint_names_map.insert(std::make_pair(joint.getName(), new_joint_index));

        // If this joint is not a dummy one then we need to expand the room in
        // skinning matrices malette to fit in a new skinning matrix.
        if (new_joint.isDummy() == false) {
            m_skinning_matrices_palette.resize(m_skinning_matrices_palette.size() + 1);
            // Of course the palette need an update now.
            m_palette_needs_update = true;
        }
    }

    void Skeleton::setPose(const Pose &local_pose)
    {
        const size_t joint_count_in_pose = local_pose.getJointCount();
        if (joint_count_in_pose == 0)
            return;

        // Accumulate the root transform if root motion is enabled.
        if (m_is_root_motion_enabled) {
            Joint &ref_root_joint = m_joint_hierarchy_array.front();
            const Transform &ref_delta_root_transform_in_pose =
                local_pose.getJointTransform(0);

            Transform root_accumulated_transform = ref_root_joint.getGlbTransform();
            root_accumulated_transform =
                Transform::combine(ref_delta_root_transform_in_pose,
                    root_accumulated_transform);

            ref_root_joint.setLclTransform(root_accumulated_transform);
            ref_root_joint.setGlbTransform(root_accumulated_transform);
        }

        // Update other joints.
        const size_t joint_count_in_skeleton = m_joint_hierarchy_array.size();
        size_t i_joint = 1;
        for (; i_joint < joint_count_in_skeleton && i_joint < joint_count_in_pose;
        ++i_joint)
        {
            Joint &ref_current_joint = m_joint_hierarchy_array[i_joint];
            Joint &ref_parent_joint = 
                m_joint_hierarchy_array[ref_current_joint.getParentIndex()];

            const Transform &ref_lcl_transform_in_pose = 
                local_pose.getJointTransform(i_joint);

            // Combine the current joint's local transform with its parent's 
            // global transform.
            Transform glb_transform =
                Transform::combine(ref_lcl_transform_in_pose,
                    ref_parent_joint.getGlbTransform());

            // Update the joint.
            ref_current_joint.setLclTransform(ref_lcl_transform_in_pose);
            ref_current_joint.setGlbTransform(glb_transform);
        }

        // If there are other joint left then update the sub hierarchy whos root
        // is changed.
        if (i_joint < joint_count_in_skeleton) {
            _updateSubHierarchyGlbTransform(i_joint);
        }

        // The skinning matrices palette now need an update later.
        m_palette_needs_update = true;
    }

    const Skeleton::MatricesVector &Skeleton::getSkinningMatricesPalette()
    {
        if (m_palette_needs_update)
            _updateSkinningMatricesPalette();

        return m_skinning_matrices_palette;
    }

    void Skeleton::setRootJointTransform(const Transform &transform)
    {
        m_joint_hierarchy_array.front().setLclTransform(transform);

        // Update entire hierarchy immediately
        _updateSubHierarchyGlbTransform(0);

        m_palette_needs_update = true;
    }

    void Skeleton::_addRootJoint(const String &name)
    {
        Joint root(name);
        m_joint_hierarchy_array.push_back(root);
        m_joint_names_map.insert(std::make_pair(name, 0));
    }

    void Skeleton::_updateSkinningMatricesPalette()
    {
        for (size_t i_joint = 0; i_joint < m_joint_hierarchy_array.size(); ++i_joint) {
            const Joint &ref_joint = m_joint_hierarchy_array[i_joint];
            
            if (ref_joint.isDummy() == false) {
                Transform skinnig_transform =
                    Transform::combine(ref_joint.getInvGlbBindingTransform(),
                        ref_joint.getGlbTransform());

                m_skinning_matrices_palette[ref_joint.getSkinningId()] =
                    skinnig_transform.toMatrix();
            }
        }

        // The palette is up-to-date.
        m_palette_needs_update = false;
    }

    void Skeleton::_updateSubHierarchyGlbTransform(int begin_root_index)
    {
        Joint &begin_root_joint = m_joint_hierarchy_array[begin_root_index];

        const int parent_index_of_root = begin_root_joint.getParentIndex();

        // Update the root joint seperately.
        if (begin_root_index == 0) {
            // The begin root is the real root joint which has no parent if the 
            // begin_root_index is 0. 
            begin_root_joint.setGlbTransform(begin_root_joint.getLclTransform());
        }
        else {
            // Otherwise derive the begin root's global transform from its parent.
            Joint &ref_parent_joint =
                m_joint_hierarchy_array[parent_index_of_root];

            Transform glb_transform =
                Transform::combine(begin_root_joint.getLclTransform(),
                    ref_parent_joint.getGlbTransform());

            begin_root_joint.setGlbTransform(glb_transform);
        }

        // Update other joints under the begin root joint.
        size_t i_joint = begin_root_index + 1;
        for (; i_joint < m_joint_hierarchy_array.size(); ++i_joint) {
            Joint &ref_current_joint = m_joint_hierarchy_array[i_joint];
            const int parent_index = ref_current_joint.getParentIndex();

            // If the parent index of the current joint is equal to the parent 
            // index of the begin root joint then that means we are done traversing
            // the entire sub hierarchy.
            if (parent_index == parent_index_of_root)
                break;

            Joint &ref_parent_joint = m_joint_hierarchy_array[parent_index];

            Transform glb_transform =
                Transform::combine(ref_current_joint.getLclTransform(),
                    ref_parent_joint.getGlbTransform());

            ref_current_joint.setGlbTransform(glb_transform);
        }
    }

};