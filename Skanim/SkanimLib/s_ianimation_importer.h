#pragma once

#include "s_prerequisites.h"

namespace Skanim
{
    /**
     */
    class _SKANIM_EXPORT IAnimationImporter
    {
    public:
        virtual ~IAnimationImporter() = 0
        {}

        /** Open file.
         */
        virtual bool openFile(const String &file_name) = 0;

        /** Close file.
         */
        virtual void closeFile() = 0;

        /** Get animation clips number in file.
         */
        virtual size_t getAnimationClipCount() = 0;

        /** Get specific animation clip's name.
         */
        virtual String getAnimationClipName(size_t clip_index) = 0;

        /** Get specific animation clip's length in millionseconds.
         */
        virtual long getAnimationClipTimeLength(size_t clip_index) = 0;

        /** Get specific animation clip's key number.
         */
        virtual size_t getAnimationClipKeyCount(size_t clip_index) = 0;

        /** Get specific animation clip's joint keys.
         */
        virtual vector<Transform> getAnimationClipJointKeys(size_t clip_index, 
            size_t joint_index) = 0;

    };
};
