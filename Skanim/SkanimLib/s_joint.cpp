#include "s_precomp.h"
#include "s_joint.h"
#include "s_skeleton.h"

namespace Skanim
{
    Joint::Joint(const String &name) noexcept
        : m_lcl_transform(Transform::IDENTITY()),
          m_glb_transform(Transform::IDENTITY()),
          m_inv_glb_binding_transform(Transform::IDENTITY()),
          m_skinning_id(SKINNING_ID_NULL),
          m_parent(INDEX_NULL),
          m_name(name)
    {}

    Joint::Joint(const Transform &transform, const Transform &binding_transform, const String &name, int skinning_id) noexcept
        : m_lcl_transform(transform),
          m_glb_transform(transform),
          m_inv_glb_binding_transform(transform),
          m_name(name),
          m_skinning_id(skinning_id),
          m_parent(INDEX_NULL)
    {}

    Joint::Joint(const String &name, int skinning_id) noexcept
        : m_lcl_transform(Transform::IDENTITY()),
          m_glb_transform(Transform::IDENTITY()),
          m_inv_glb_binding_transform(Transform::IDENTITY()),
          m_name(name),
          m_skinning_id(skinning_id),
          m_parent(INDEX_NULL)
    {}
};