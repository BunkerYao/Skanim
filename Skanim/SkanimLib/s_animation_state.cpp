#include "s_precomp.h"
#include "s_animation_state.h"
#include "s_ianimation_clip.h"

namespace Skanim
{
    AnimationState::AnimationState() noexcept
        : m_animation_clip(nullptr),
          m_speed(1.0f),
          m_current_local_time(0),
          m_is_looping(false),
          m_jump_flag(JUMP_FLAG_NONE),
          m_last_root_transform(Transform::IDENTITY())
    {}

    AnimationState::AnimationState(const String &name, 
        const IAnimationClip *animation_clip, float speed, bool loop_play) noexcept
        : m_name(name),
          m_animation_clip(animation_clip),
          m_speed(speed),
          m_current_local_time(0),
          m_is_looping(loop_play),
          m_jump_flag(JUMP_FLAG_NONE),
          m_last_root_transform(Transform::IDENTITY())
    {}

    void AnimationState::advanceTime(long elapsed_time)
    {
        assert(m_animation_clip && "no animation clip");

        // Update current local time.
        const long scaled_elapsed_time = (long)std::round(elapsed_time * m_speed);
        m_current_local_time += scaled_elapsed_time;

        // Current local time may exceeds the time range of the animation clip's
        // time. If loop play mode is off, we need to clamp current local time in
        // the animation clip's time range. Otherwise the current local time need
        // to be divide by animation's time length to keep it in valid range. Which 
        // causes "Jump" on local time line. Set the jump flag when jump happens 
        // so we can deal with this special situation in _updateCurrentPose() later.
        const long animation_clip_time_length = m_animation_clip->getLength();

        if (m_current_local_time < 0 || 
            m_current_local_time > animation_clip_time_length) {
            if (!m_is_looping) {
                m_current_local_time = Math::clamp(m_current_local_time, 0, 
                    animation_clip_time_length);
                m_jump_flag = JUMP_FLAG_NONE;
            }
            else {
                m_current_local_time /= animation_clip_time_length;

                if (m_current_local_time < 0) {
                    m_current_local_time += animation_clip_time_length;
                    // Jump to the front happens.
                    m_jump_flag = JUMP_FLAG_FORWARD;
                }
                else {
                    // Backward jump happens.
                    m_jump_flag = JUMP_FLAG_BACKWARD;
                }
            }
        }
        else {
            m_jump_flag = JUMP_FLAG_NONE;
        }

        // Update the current pose.
        _updateCurrentPose();
    }

    void AnimationState::reset()
    {
        // Set the current local time to 0.
        m_current_local_time = 0;

        // Update the current pose so the current pose is the first pose of the
        // animation.
        _updateCurrentPose();

        // Overwrite the delta root motion in the pose to be identity so
        // the root won't move.
        m_current_pose.setJointTransform(0, Transform::IDENTITY());
    }

    void AnimationState::setAnimationClip(const IAnimationClip *clip)
    {
        assert(clip && "animation clip can't be nullptr");

        m_animation_clip = clip;

        reset();

        _updateBoundaryRootTransforms();
    }

    void AnimationState::_updateCurrentPose()
    {
        // Extract the current pose from animation clip.
        m_animation_clip->extractPose(m_current_local_time, &m_current_pose);

        // Keep the current root transform and calculate the delta root transform.
        Transform current_root_transform = m_current_pose.getJointTransform(0);
        Transform delta_root_transform;

        // Calculate the delta root motion.
        // There could be local time jump due to loop playing, if jump happens we
        // should handle this special situation.
        if (m_jump_flag == JUMP_FLAG_NONE) {
            delta_root_transform = Transform::combine(current_root_transform,
                m_last_root_transform.inversed());
        }
        else {
            // If it's a forward jump, we need to get the pre-delta transform 
            // from last root transform to the begining root transform.
            // Otherwise get the pre-delta transform from last root transform to
            // the end root transform.
            Transform first_delta_root_transform, second_delta_root_transform;

            if (m_jump_flag == JUMP_FLAG_FORWARD) {
                first_delta_root_transform = Transform::combine(m_begining_root_transform,
                    m_last_root_transform.inversed());
                second_delta_root_transform = Transform::combine(current_root_transform,
                    m_end_root_transform.inversed());
            }
            else {
                first_delta_root_transform = Transform::combine(m_end_root_transform,
                    m_last_root_transform.inversed());
                second_delta_root_transform = Transform::combine(current_root_transform,
                    m_begining_root_transform.inversed());
            }

            // Conbine the first and the second delta root transform to get the final
            // delta root transform.
            delta_root_transform = Transform::combine(second_delta_root_transform, 
                first_delta_root_transform);
        }

        // Keep the current root transform and replace the root transform
        // in current pose with delta root transform.
        m_last_root_transform = current_root_transform;
        m_current_pose.setJointTransform(0, delta_root_transform);
    }

    void AnimationState::_updateBoundaryRootTransforms()
    {
        // Extract the root transform of the begin pose and the end pose in the
        // animation clip.
        Pose temp_pose;

        m_animation_clip->extractPose(0, &temp_pose);
        m_begining_root_transform = temp_pose.getJointTransform(0);

        m_animation_clip->extractPose(m_animation_clip->getLength(), &temp_pose);
        m_end_root_transform = temp_pose.getJointTransform(0);
    }

};
