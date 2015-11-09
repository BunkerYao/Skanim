#pragma once

#include "s_prerequisites.h"
#include "s_pose.h"
#include "s_track.h"

namespace Skanim
{
    /** Animation clip stores each joint's track in a time period.
     */
    class _SKANIM_EXPORT AnimationClip
    {
    public:
        explicit AnimationClip(size_t key_count) noexcept;

        AnimationClip(size_t key_count, const String &name, long interval) noexcept;

        /** Get the number of tracks.
         */
        size_t getTrackCount() const
        {
            return m_tracks.size();
        }

        /** Add a track to this clip.
         *  The track being added must has the same key count as this clip.
         */
        void addTrack(const Track &track);

        /** Remove the i'th track. 
         */
        void removeTrack(size_t i);

        /** Get the number of keys.
         */
        size_t getKeyCount() const
        {
            return m_key_count;
        }

        /** Get the time length.
         */
        long getTimeLength() const
        {
            return (m_key_count - 1) * m_key_interval;
        }

        /** Extract pose from this clip with local time.
         */
        Pose extractPose(long local_time) const;

        /** Get key interval.
         */
        long getKeyInterval() const
        {
            return m_key_interval;
        }

        /** Modify the key interval.
         */
        void setKeyInterval(long val)
        {
            m_key_interval = val;
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
        typedef vector<Track> _TracksArray;

        // Stores each joint's track.
        // Each track should have the same key count.
        _TracksArray m_tracks;
        // The name of this clip
        String m_name;
        // The time interval between keys.
        long m_key_interval;
        // The constant number of keys.
        const size_t m_key_count;
    };
};
