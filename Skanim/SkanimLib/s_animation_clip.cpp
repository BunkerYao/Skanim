#include "s_precomp.h"
#include "s_animation_clip.h"

namespace Skanim
{
    KeyPoseAnimationClip::KeyPoseAnimationClip(size_t track_count) noexcept
        : m_track_count(track_count),
          m_key_pose_interval(0)
    {}

    KeyPoseAnimationClip::KeyPoseAnimationClip(size_t track_count, const String &name,
        long interval) noexcept
        : m_track_count(track_count),
          m_name(name),
          m_key_pose_interval(interval)
    {}

    void KeyPoseAnimationClip::extractPose(long local_time, Pose *extracted_pose) const
    {
        assert(local_time >= 0 && local_time <= getLength() &&
            "local time out of range");

        const int key_index = local_time / m_key_pose_interval;
        // Calculate t for interpolation between key frames.
        const float t = (float)local_time / m_key_pose_interval - key_index;

        // Use t to lerp between the selected key pose and its next one. If key
        // index is exactly the last key pose's index, then return that key pose
        // directly since it's impossible to lerp between the key pose next to it.
        if (key_index != m_key_pose_sequence.size() - 1) {
            Pose::lerp(t, m_key_pose_sequence[key_index], 
                m_key_pose_sequence[key_index + 1], extracted_pose);
        }
        else {
            *extracted_pose = m_key_pose_sequence[key_index];
        }
    }

};