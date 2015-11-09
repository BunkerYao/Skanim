#pragma once

#include "s_prerequisites.h"
#include "s_pose.h"
#include "s_transform.h"

namespace Skanim
{
    /** An animation state could play a animation clip, just like a player.
     *  It keeps the current playback time of the animation clip and extract current pose from
     *  animation clip which is being used.
     *  It also converts the absolute root transform in the animation clip to delta root transform.
     */
    class _SKANIM_EXPORT AnimationState
    {
    public:
        AnimationState() noexcept;

        AnimationState(const String &name, const AnimationClip *animation_clip, 
            float speed, bool loop_play) noexcept;

        /** advance time of this state.
         *  The state extracts current pose from animation clip.
         */
        void advanceTime(long elapsed_time);

        /** Reset the animation state.
         *  This will set the current playback position to the begining of the animation clip.
         */
        void reset();

        /** Get the name of this animation state.
         */
        const String &getName() const
        {
            return m_name; 
        }

        /** Modify the name of this animation state.
         */
        void setName(const String &name) 
        {
            m_name = name;
        }

        /** Get the animation clip used by this animation state.
         */
        const AnimationClip *getAnimationClip() const 
        { 
            return m_animation_clip; 
        }

        /** Modify the animation clip used by this animation state.
         *  This will alse reset the animation state.
         *  The given animation clip can't be nullptr.
         */
        void setAnimationClip(const AnimationClip *clip);

        /** Get the playback speed of this animation state.
         */
        float getSpeed() const 
        {
            return m_speed; 
        }

        /** Set the playback speed of this animation state.
         */
        void setSpeed(float val) 
        {
            m_speed = val; 
        }

        /** Check if this animation state is looping the animation clip.
         */
        bool isLooping() const 
        {
            return m_is_looping; 
        }

        /** Toggle loop play mode.
         */
        void setLooping(bool enable) 
        {
            m_is_looping = enable;
        }

        /** Get the current pose extracted from animation clip.
         */
        const Pose &getCurrentPose() const
        {
            return m_current_pose;
        }

    private:
        // Update the current extracted pose.
        void _updateCurrentPose();

        // Update the begining root transform and the end root transform.
        void _updateBoundaryRootTransforms();

    private:
        // The name of this animation state.
        String m_name;
        // The animation clip used by this animation state.
        const AnimationClip *m_animation_clip;
        
        // The playback speed factor.
        float m_speed;
        // The current playback time.
        long m_current_local_time;
        // Looping flag.
        bool m_is_looping;
        // A flag indicate that if there is "Jump" happens due to
        // loop playing the animation clip.
        // If the integer is 1 means forward jump, -1 meas backward jump, 0 means there
        // is no jump happens.
        int m_jump_flag;

        // Jump flag constants
        const int JUMP_FLAG_FORWARD = 1;
        const int JUMP_FLAG_BACKWARD = -1;
        const int JUMP_FLAG_NONE = 0;

        // The the extracted root transform last time.
        Transform m_last_root_transform;
        // The first pose's root transform in the animation clip.
        Transform m_begining_root_transform;
        // The end pose's root transform in the animation clip.
        Transform m_end_root_transform;
        // Current pose.
        Pose m_current_pose;
    };
};