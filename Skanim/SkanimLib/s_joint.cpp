#include "s_precomp.h"
#include "s_joint.h"
#include "s_skeleton.h"

namespace Skanim
{
    Joint::Joint() noexcept
        : m_parent(nullptr),
          m_child(nullptr),
          m_last_child(nullptr),
          m_sibling(nullptr),
          m_owner_skeleton(nullptr)
    {
        // Do nothing
    }

    Joint::Joint(const Transform &transform, const Transform &binding_transform, const String &name, int id) noexcept
        : m_lcl_transform(transform),
          m_glb_transform(transform),
          m_glb_binding_transform(transform),
          m_name(name),
          m_skinning_id(id),
          m_parent(nullptr),
          m_child(nullptr),
          m_last_child(nullptr),
          m_sibling(nullptr),
          m_owner_skeleton(nullptr)
    {
        // Do nothing
    }

    /*Joint::Joint(Joint &&other) noexcept
    {
        *this = std::move(other);
    }*/

    Joint::Joint(const String &name, int id) noexcept
        : m_lcl_transform(Transform::IDENTITY()),
          m_glb_transform(Transform::IDENTITY()),
          m_glb_binding_transform(Transform::IDENTITY()),
          m_name(name),
          m_skinning_id(id),
          m_parent(nullptr),
          m_child(nullptr),
          m_last_child(nullptr),
          m_sibling(nullptr),
          m_owner_skeleton(nullptr)
    {
        // Do nothing
    }

    //Joint &Joint::operator=(Joint &&other)
    //{
    //    if (this != &other) {
    //        Joint *p_parent = other.m_parent;

    //        if (p_parent) {
    //            // Remove the old one from hierarchy.
    //            other.remove();
    //            // Add this one to the hierarchy.
    //            p_parent->addChild(this);
    //        }

    //        // Copy children pointers
    //        m_child = other.m_child;
    //        m_last_child = other.m_last_child;

    //        // Zero out the children pointers in the old one.
    //        other.m_child = nullptr;
    //        other.m_last_child = nullptr;

    //        // Copy properties.
    //        m_lcl_transform = other.m_lcl_transform;
    //        m_glb_transform = other.m_glb_transform;
    //        m_glb_binding_transform = other.m_glb_binding_transform;
    //        m_name = other.m_name;
    //        m_skinning_id = other.m_skinning_id;
    //        m_owner_skeleton = other.m_owner_skeleton;
    //    }

    //    return *this;
    //}

    Joint::~Joint()
    {
        // Delete children joints recursively.
        Joint *p_joint = m_child;
        while (p_joint) {
            Joint *p_next = p_joint->m_sibling;
            delete p_joint;
            p_joint = p_next;
        }
    }

    void Joint::setLclTransform(const Transform &transform)
    {
        m_lcl_transform = transform;
        _updateGlbTransform();
        _updateHierarchyGlbTransform();
    }

    void Joint::setGlbTransform(const Transform &transform)
    {
        m_glb_transform = transform;
        _updateLclTransform();
        _updateHierarchyGlbTransform();
    }

    void Joint::setName(const String &name)
    {
        // If the joint belongs to a skeleton and the name is actually changed,
        // then notify the owner skeleton.
        if (m_owner_skeleton && m_name != name) {
            m_owner_skeleton->_onJointRenamed(m_name, name);
        }

        m_name = name;
    }

    size_t Joint::getChildrenCountRecursive() const
    {
        size_t count = 0;
        const Joint *p_joint = m_child;
        while (p_joint) {
            count += p_joint->getChildrenCountRecursive();
            count++;
            p_joint = p_joint->m_sibling;
        }
        return count;
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

    void Joint::addChild(Joint *child)
    {
        // Check if the child being added already has a parent.
        assert(child->m_parent == nullptr && "child already has a parent.");

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

        // Update the local transform of child joint so it stays where is was in global space.
        child->_updateLclTransform();

        // Notify the owner skeleton.
        if (m_owner_skeleton)
            m_owner_skeleton->_onJointAdded(child);
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
            m_parent->m_child = m_sibling;
            m_parent->m_last_child = m_sibling;
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

        // Notify the owner skeleton.
        if (m_owner_skeleton)
            m_owner_skeleton->_onJointRemoved(this);
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
        if (m_child == nullptr) return;

        std::stack<Joint*, list<Joint*>> stack;
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