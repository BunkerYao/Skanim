#include "s_precomp.h"
#include "s_joint.h"

namespace Skanim
{
    Joint::Joint() noexcept
        : m_parent(nullptr),
          m_child(nullptr),
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
          m_id(id),
          m_parent(nullptr),
          m_child(nullptr),
          m_sibling(nullptr)
    {
        // Do nothing
    }

};