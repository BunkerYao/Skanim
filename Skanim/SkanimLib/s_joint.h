#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_transform.h"

namespace Skanim
{
    /** Joint is the building block of a skeleton. Joints can be organized in a
     *  hierarchy structure. Each child joint's global transform will derive from
     *  its parent's global transform. A joint's children indices is stored in a 
     *  linked list. Each joint alse keeps the index of its parent. A joint is 
     *  equivalent to a bone in some DCC packages. 
     */
    class _SKANIM_EXPORT Joint
    {
    public:

        // Invalid index for a joint.
        const int INDEX_NULL = -1;

    public:

        explicit Joint(const String &name) noexcept;

        Joint(const String &name, int skinning_id) noexcept;

        Joint(const Transform &transform, const Transform &binding_transform,
            const String &name, int skinning_id) noexcept;

        /** Get the local transform of this joint.
         */
        const Transform &getLclTransform() const
        {
            return m_lcl_transform;
        }

        /** Modify the local transform of this joint.
         */
        void setLclTransform(const Transform &transform)
        {
            m_lcl_transform = transform;
        }

        /** Get the global transform of this joint.
         */
        const Transform &getGlbTransform() const
        {
            return m_glb_transform;
        }

        /** Modify the global transform of this joint.
         */
        void setGlbTransform(const Transform &transform)
        {
            m_glb_transform = transform;
        }

        /** Get the inverse of global binding transform of this joint.
        */
        const Transform &getInvGlbBindingTransform() const
        {
            return m_inv_glb_binding_transform;
        }

        /** Modify the inverse of global binding transform of this joint.
         */
        void setInvGlbBindingTransform(const Transform &transform)
        {
            m_inv_glb_binding_transform = transform;
        }

        /** Get name of this joint.
         */
        const String &getName() const
        {
            return m_name;
        }

        /** Get skinning id of this joint.
         */
        int getSkinningId() const
        {
            return m_skinning_id;
        }

        /** Modify skinning id of this joint.
         *  A negative value indicates a dummy joint.
         */
        void setSkinningId(int id)
        {
            m_skinning_id = id;
        }

        /** Check if this joint is a dummy joint.
         */
        bool isDummy() const
        {
            return m_skinning_id == SKINNING_ID_NULL;
        }

        /** Get the children count of this joint.
         */
        size_t getChildrenCount() const
        {
            return m_children.size();
        }

        /** Get the i'th child's index of this joint.
         */
        int getChildIndex(size_t i) const
        {
            size_t child_index = INDEX_NULL;
            size_t i_joint = 0;
            for (auto &joint_index : m_children) {
                if (joint_index == i)
                    break;
            }
            return child_index;
        }

        /** Get parent joint's index of this joint.
         */
        int getParentIndex() const
        {
            return m_parent;
        }

        /** Modify the parent index.
         */
        void setParentIndex(int index)
        {
            m_parent = index;
        }

        /** Add a child to this joint.
         */
        void addChild(int child_index)
        {
            m_children.push_back(child_index);
        }

        /** Remove all children indices.
         */
        void removeAllChildren()
        {
            m_children.clear();
        }

        /** Check if this joint has a parent.
         */
        bool hasParent() const
        {
            return m_parent != INDEX_NULL;
        }

    private:
        // The current local and global transform.
        Transform m_lcl_transform, m_glb_transform;

        // The global transform of this joint in binding pose.
        Transform m_inv_glb_binding_transform;

        // The name of this joint. 
        // It must be unique among a skeleton's all joints.
        const String m_name;

        const int SKINNING_ID_NULL = -1;

        // The skinning id of this joint.
        // If the joint is dummy the value will be SKINNING_ID_NULL;
        int m_skinning_id;

        // The index of the parent joint in the array.
        int m_parent;

        // The indices of all children.
        list<int> m_children;
    };
};
