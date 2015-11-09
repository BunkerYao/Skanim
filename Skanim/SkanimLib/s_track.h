#pragma once

#include "s_prerequisites.h"
#include "s_transform.h"

namespace Skanim
{
    /** A track stores a sequence of transform keys.
     *  The keys represents the trajectory of a joint's motion in space.
     */
    class _SKANIM_EXPORT Track
    {
    public:
        explicit Track(int key_count) noexcept
            : m_key_sequence(key_count)
        {}

        explicit Track(const vector<Transform> &key_sequence) noexcept
            : m_key_sequence(key_sequence)
        {}

        /** Get the numbers of keys.
         */
        size_t getKeyCount() const
        {
            return m_key_sequence.size();
        }

        /** Modify a specific key on this track.
         */
        void setKey(size_t key, const Transform &val)
        {
            assert(key < m_key_sequence.size() && "key out of range");

            m_key_sequence[key] = val;
        }

        /** Take sample on this channel.
         */
        Transform takeSample(size_t key, float t) const
        {
            assert(key < m_key_sequence.size() && "key out of range");

            if (key != m_key_sequence.size() - 1) 
                return Transform::lerp(t, m_key_sequence[key], m_key_sequence[key + 1]);
            else 
                // If the key is the last one then there is no way to interpolate.
                return m_key_sequence[key];
        }

    private:
        // A transform key sequence.
        vector<Transform> m_key_sequence;
    };
};
