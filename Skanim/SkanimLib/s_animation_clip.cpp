#include "s_precomp.h"
#include "s_animation_clip.h"

namespace Skanim
{
    AnimationClip::AnimationClip(size_t key_count) noexcept
        : m_key_count(key_count),
          m_key_interval(0)
    {}

    AnimationClip::AnimationClip(size_t key_count, const String &name, long interval) noexcept
        : m_key_count(key_count),
          m_name(name),
          m_key_interval(interval)
    {}

    void AnimationClip::addTrack(const Track &track)
    {
        assert(track.getKeyCount() == m_key_count && 
            "the track's key count differs from animation clip");

        m_tracks.push_back(track);
    }

    void AnimationClip::removeTrack(size_t i)
    {
        assert(i < m_tracks.size() && "i out of range");

        m_tracks.erase(m_tracks.begin() + i);
    }

    Pose AnimationClip::extractPose(long local_time) const
    {
        assert(local_time >= 0 && local_time <= getTimeLength() &&
            "local time out of range");
        const int key = local_time / m_key_interval;
        const float t = (float)local_time / m_key_interval - key;

        Pose pose(m_tracks.size());
        size_t i_joint = 0;
        for (auto &track : m_tracks) {
            Transform joint_transform = track.takeSample(key, t);
            pose.setJointTransform(i_joint, joint_transform);
            i_joint++;
        }

        return pose;
    }

};