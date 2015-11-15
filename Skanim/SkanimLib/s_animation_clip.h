#pragma once

#include "s_prerequisites.h"
#include "s_ianimation_clip.h"
#include "s_pose.h"

namespace Skanim
{
    /** Key pose animation clips stores a pre-defined key pose sequence to 
     *  represet a skeleton's motion in a time period. The key poses have a 
     *  constant time inverval and a common joint transform count.
     */
    class _SKANIM_EXPORT KeyPoseAnimationClip : public IAnimationClip
    {
    public:
        explicit KeyPoseAnimationClip(size_t track_count) noexcept;

        KeyPoseAnimationClip(size_t track_count, const String &name,
            long interval) noexcept;

        /** Get the time length.
         */
        virtual long getLength() const override
        {
            return (m_key_pose_sequence.size() - 1) * m_key_pose_interval;
        }

        /** Get the number of tracks
         */
        virtual size_t getTrackCount() const override
        {
            return m_track_count;
        }

        /** Extract pose from this clip with local time.
         */
        virtual void extractPose(long local_time, Pose *extracted_pose) 
            const override;

        /** Get the number of key poses.
         */
        size_t getKeyPoseCount() const
        {
            return m_key_pose_sequence.size();
        }

        /** Get the key pose with key index
         */
        Pose getKeyPose(size_t key_index) const
        {
            assert(key_index < m_key_pose_sequence.size() &&
                "key index out of range");

            return m_key_pose_sequence[key_index];
        }

        /** Modify the key pose with key index.
         */
        void setKeyPose(size_t key_index, const Pose &key_pose)
        {
            assert(key_index < m_key_pose_sequence.size() &&
                "key index out of range");
            assert(m_track_count == key_pose.getJointCount() &&
                "key pose's joint count doesn't match clip's track count.");

            m_key_pose_sequence[key_index] = key_pose;
        }

        /** Add a key pose to the clip
         */
        void addKeyPose(const Pose &key_pose)
        {
            assert(m_track_count == key_pose.getJointCount() &&
                "key pose's joint count doesn't match clip's track count.");

            m_key_pose_sequence.push_back(key_pose);
        }

        /** Remove a key pose with key index.
         */
        void removeKeyPose(size_t key_index)
        {
            assert(key_index < m_key_pose_sequence.size() &&
                "key index out of range");

            m_key_pose_sequence.erase(m_key_pose_sequence.begin() + key_index);
        }

        /** Clear all key poses.
         */
        void clearKeyPoses()
        {
            if (!m_key_pose_sequence.empty())
                m_key_pose_sequence.clear();
        }

        /** Get key interval.
         */
        long getKeyPoseInterval() const
        {
            return m_key_pose_interval;
        }

        /** Modify the key interval.
         */
        void setKeyPoseInterval(long val)
        {
            m_key_pose_interval = val;
        }

        /** Get the name of this clip.
         */
        const String &getName() const
        {
            return m_name; 
        }

        /** Modify the name of this clip.
         */
        void setName(const String &val) 
        {
            m_name = val; 
        }

    private:
        typedef vector<Pose> _PosesArray;

        // The number of joint tracks.
        const size_t m_track_count;

        // Key pose sequence array. Key poses in this array must have the same
        // joint transform count which is equal to track count of the clip.
        _PosesArray m_key_pose_sequence;

        // The name of this clip
        String m_name;

        // The time interval between key poses.
        long m_key_pose_interval;
    };
};

