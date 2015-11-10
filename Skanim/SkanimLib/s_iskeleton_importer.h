#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_transform.h"

namespace Skanim
{
    /** 
     */
    class _SKANIM_EXPORT ISkeletonImporter
    {
    public:
        virtual ~ISkeletonImporter() = 0
        {}

        /** Open file.
         */
        virtual bool openFile(const String &file_name) = 0;

        /** Close file.
         */
        virtual void closeFile() = 0;

        /** Check if this file has skeleton data.
         */
        virtual bool hasSkeleton() = 0;

        /** Get the name of skeleton in file.
         */
        virtual String getSkeletonName(size_t skeleton_index) = 0;

        /** Get the number of joints of the skeleton in file.
         */
        virtual size_t getSkeletonJointCount() = 0;

        /** Get the root joint's index.
         */
        virtual size_t getRootJointIndex() = 0;

        /** Get the joint's child index.
         */
        virtual size_t getChildJointIndex(size_t parent_joint_index, size_t i_child) = 0;

        /** Get a joint's child number..
         */
        virtual size_t getChildCount(size_t parent_joint_index) = 0;

        /** Get a joint's name.
         */
        virtual String getJointName(size_t joint_index) = 0;

        /** Get a joint's skinning id.
         */
        virtual int getJointSkinningId(size_t joint_index) = 0;

        /** Check if a joint is dummy.
         */
        virtual bool isJointDummy(size_t joint_index) = 0;

        /** Get joint's global binding transform.
         */
        virtual Transform getJointGlobalBindingTransform(size_t joint_index) = 0;
    };
};
