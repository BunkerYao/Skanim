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
     *  When a joint is deleted, all its children in hierarchy are deleted recursively.
     */
    class _SKANIM_EXPORT Joint
    {
    public:
        Joint() noexcept;

        /** Copy constructor is not allowed.
         */
        Joint(const Joint &other) = delete;

        /** Move constructor.
         */
        //Joint(Joint &&other) noexcept;

        Joint(const Transform &transform, const Transform &binding_transform,
            const String &name, int id) noexcept;

        Joint(const String &name, int id) noexcept;

        /** Operator = on joint class is not allowed.
         */
        Joint &operator=(const Joint &other) = delete;

        //Joint &operator=(Joint &&other);

        ~Joint();

        /** Get the local transform of this joint.
         */
        const Transform &getLclTransform() const
        {
            return m_lcl_transform;
        }

        /** Modify the local transform of this joint.
         */
        void setLclTransform(const Transform &transform);

        /** Get the global transform of this joint.
         */
        const Transform &getGlbTransform() const
        {
            return m_glb_transform;
        }

        /** Modify the global transform of this joint.
         */
        void setGlbTransform(const Transform &transform);

        /** Get the global binding transform of this joint.
        */
        const Transform &getGlbBindingTransform() const
        {
            return m_glb_binding_transform;
        }

        /** Modify the global binding transform of this joint.
         */
        void setGlbBindingTransform(const Transform &transform)
        {
            m_glb_binding_transform = transform;
        }

        /** Get name of this joint.
         */
        const String &getName() const
        {
            return m_name;
        }

        /** Modify the name of this joint.
         */
        void setName(const String &name);

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
            return m_skinning_id < 0;
        }

        /** Get the children count recursively.
         */
        size_t getChildrenCountRecursive() const;

        /** Get the children count of this joint.
         */
        size_t getChildrenCount() const;

        /** Get the i'th child of this joint.
         *  Return nullptr if it doesn't exist.
         */
        Joint *getChild(size_t i) const;

        /** Get parent joint of this joint.
         */
        Joint *getParent() const
        {
            return m_parent;
        }

        /** Add child to this joint.
         *  If the child being added already has a parent then this method
         *  will fail.
         */
        void addChild(Joint *child);

        /** Remove this joint from its parent joint.
         *  If this joint has no parent joint then calling this method has
         *  no effect.
         */
        void remove();

        /** Get the owner skeleton. It could be nullptr if joint doesn't belong to any skeleton.
         */
        Skeleton *getOwnerSkeleton() const
        {
            return m_owner_skeleton;
        }

        /** Set the owner skeleton.
         */
        void _setOwnerSkeleton(Skeleton *owner_skeleton)
        {
            m_owner_skeleton = owner_skeleton;
        }

    private:

        friend _SKANIM_EXPORT class Skeleton;

        // Find the left sibling of this joint.
        Joint *_findLeftSibling() const;

        // Update local transform from global transform.
        void _updateLclTransform();
        // Update global transform.
        void _updateGlbTransform();
        // Update all the child joints' global transformation in hierarchy.
        void _updateHierarchyGlbTransform();

    private:
        // The current local and global transform.
        Transform m_lcl_transform, m_glb_transform;

        // The global transform of this joint in binding pose.
        Transform m_glb_binding_transform;

        // The name of this joint. 
        // It must be unique among a skeleton's all joints.
        String m_name;

        // The skinning id of this joint.
        // If the joint is dummy the value will be a unique negative integer number 
        // since dummy bone has no useful skinning id. 
        int m_skinning_id;

        // Pointers to other joints.
        // Keep the pointer to the last child so we don't need to
        // traverse throungh the children list when a child is being added.
        Joint *m_parent, *m_child, *m_last_child, *m_sibling;

        // The owner skeleton of this joint. It could be nullptr
        // if the joint has not been added to any skeleton.
        // Keep this pointer is for notifing the owner skeleton when certain operations called.
        Skeleton *m_owner_skeleton;
    };
};
