#include "s_precomp.h"
#include "s_joint.h"

namespace Skanim
{
    Joint::Joint() noexcept
        : m_parent(nullptr),
          m_child(nullptr),
          m_last_child(nullptr),
          m_sibling(nullptr)
    {
        // Do nothing
    }

    Joint::Joint(const Transform &transform, const Transform &binding_transform, const String &name, int id) noexcept
        : m_lcl_transform(transform),
          m_glb_transform(transform),
          m_lcl_binding_transform(binding_transform),
          m_glb_binding_transform(transform),
          m_name(name),
          m_skinning_id(id),
          m_parent(nullptr),
          m_child(nullptr),
          m_last_child(nullptr),
          m_sibling(nullptr)
    {
        // Do nothing
    }

    size_t Joint::getChildrenCount() const
    {
        // Go through the children list and count the number of joints.
        Joint *p_joint = m_child;
        size_t children_count = 0;
        while (p_joint) {
            children_count++;
            p_joint = p_joint->m_sibling;
        }
        return children_count;
    }

    Joint *Joint::getChild(size_t i) const
    {
        Joint *p_joint = m_child;
        size_t index = 0;
        while (p_joint) {
            if (index == i) break;
            index++;
            p_joint = p_joint->m_sibling;
        }
        return p_joint;
    }

    bool Joint::addChild(Joint *child)
    {
        // Check if the child being added already has a parent.
        if (child->m_parent)
            return false;

        // Whether this joint has children or not.
        if (!m_child) {
            // Joint has no children.
            m_child = child;
            m_last_child = child;
        }
        else {
            // Joint has children.
            m_last_child->m_sibling = child;
            m_last_child = child;
        }

        child->m_parent = this;

        // Update the local transform of this joint so it stays where is was in global space.
        _updateLclTransform();

        return true;
    }

    void Joint::remove()
    {
        // Check if parent joint exists.
        if (!m_parent)
            return;

        Joint *p_left_sibling = _findLeftSibling();;

        // If the joint is the first child of its parent.
        if (!p_left_sibling) {
            // This joint is the first child.
            m_parent->m_child = nullptr;
            m_parent->m_last_child = nullptr;
        }
        else {
            // This joint is not the first child.
            p_left_sibling->m_sibling = m_sibling;
            // If this joint is the last child of its parent then update parent's
            // pointer to the last child.
            if (!m_sibling)
                m_parent->m_last_child = p_left_sibling;
        }

        m_sibling = nullptr;
        m_parent = nullptr;

        // Update the local transform of this joint so it stays where is was in global space.
        _updateLclTransform();
    }

    Joint *Joint::_findLeftSibling() const
    {
        // If the joint doesn't have a parent, then return nullptr.
        if (!m_parent) 
            return nullptr;

        // Check if the first child is this joint. If it is then return nullptr since it
        // has no left sibling.
        if (m_parent->m_child == this)
            return nullptr;

        // Go through the children list to find the previouse joint of this one.
        Joint *p_joint = m_parent->m_child;
        while (p_joint) {
            Joint *p_next = p_joint->m_sibling;
            // if the next joint is this joint, then the current one is the left sibling.
            // Else keep traversing.
            if (p_next == this) break;
            else p_joint = p_next;
        }

        return p_joint;
    }

    void Joint::_updateLclTransform()
    {
        if (m_parent)
            m_lcl_transform = Transform::combine(m_glb_transform, m_parent->m_glb_transform.inversed());
        else
            m_lcl_transform = m_glb_transform;
    }

    void Joint::_updateGlbTransform()
    {
        if (m_parent)
            m_glb_transform = Transform::combine(m_lcl_transform, m_parent->m_glb_transform);
        else
            m_glb_transform = m_lcl_transform;
    }

    void Joint::_updateHierarchyGlbTransform()
    {
        std::stack<Joint*, list<Joint*>::type> stack;
        Joint *p_joint = m_child;
        stack.push(p_joint);

        while (!stack.empty() || p_joint) {
            while (p_joint) {
                Joint *parent = p_joint->m_parent;
                p_joint->m_glb_transform = Transform::combine(m_lcl_transform, parent->m_glb_transform);
                stack.push(p_joint);
                p_joint = p_joint->m_child;
            }

            if (!stack.empty()) {
                p_joint = stack.top();
                stack.pop();
                p_joint = p_joint->m_sibling;
            }
        }
    }

};