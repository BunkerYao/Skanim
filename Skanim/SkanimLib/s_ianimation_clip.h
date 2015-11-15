#pragma once

#include "s_prerequisites.h"
#include "s_pose.h"

namespace Skanim
{
    /** Animation clip stores a skeleton's motion in a period of time.
     */
    class _SKANIM_EXPORT IAnimationClip
    {
    public:
        virtual ~IAnimationClip() = 0
        {}

        /** Get the total time length of the clip.
         */
        virtual long getLength() const = 0;

        /** Get the joint tracks of the clip.
         */
        virtual size_t getTrackCount() const = 0;

        /** Extract the pose at local time t.
         */
        virtual void extractPose(long t, Pose *extracted_pose) const = 0;
    };
};
