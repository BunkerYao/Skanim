#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_transform.h"

namespace Skanim
{
    /** Joint is the building block of a skeleton. Joints can be organized in a hierarchy structure.
     *  Each child joint's global transform will derive from its parent's global transform.
     *  A joint's children is stored in a linked list. Each joint keeps the pointer of its parent,
     *  direct child and sibling.
     *  A joint is equivalent to a bone.
     */
    class _SKANIM_EXPORT Joint
    {
    public:
        Joint() noexcept;

        Joint(const Transform &transform, const Transform &binding_transform,
            const String &name, int id) noexcept;

        /** Get the local transform of this joint.
         */
        const Transform &getLclTransform() const
        {
            return m_lcl_transform;
        }

        /** Get the global transform of this joint.
         */
        const Transform &getGlbTransform() const
        {
            return m_glb_transform;
        }

        /** Get the local binding transform of this joint.
        */
        const Transform &getLclBindingTransform() const
        {
            return m_lcl_binding_transform;
        }

        /** Get the global binding transform of this joint.
        */
        const Transform &getGlbBindingTransform() const
        {
            return m_glb_binding_transform;
        }

        /** Get name of this joint.
         */
        const String &getName() const
        {
            return m_name;
        }

        /** Modify the name of this joint.
         */
        void setName(const String &name)
        {
            m_name = name;
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

    private:
        // Find the left sibling of this joint.
        Joint *_findLeftSibling() const;
         

    private:
        // The current local and global transform.
        Transform m_lcl_transform, m_glb_transform;

        // The local and global transform of this joint in binding pose.
        Transform m_lcl_binding_transform, m_glb_binding_transform;

        // The name of this joint. 
        // It must be unique among a skeleton's all joints.
        String m_name;

        // The skinning id of this joint.
        // If the joint is dummy the value will be a unique negative integer number 
        // since dummy bone has no useful skinning id. 
        int m_skinning_id;

        // Pointers to other joints.
        Joint *m_parent, *m_child, *m_sibling;
    };
};
