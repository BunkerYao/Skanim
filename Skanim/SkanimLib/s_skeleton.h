#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_iteratorwrapper.h"
#include "s_space.h"

namespace Skanim
{
    /** A skeleton contains a hierarchy structure of joints.
     *  A skeleton object can accept a key pose and generate a matrices palette 
     *  which is used for skinning.
     */
    class _SKANIM_EXPORT Skeleton
    {
    public:
        /** Construct skeleton from a given root joint.
         */
        explicit Skeleton(Joint *root) noexcept;

        ~Skeleton();

        typedef list<Joint*> JointPtrList;
        typedef VectorConstIterator<JointPtrList> JointPtrIterator;

        /** This method returns an iterator of the root joint.
         *  The iteration of the joint hierarchy will be in pre-order.
         */
        JointPtrIterator getRootIterator();

        /** Find joint by name.
         *  @return Returns nullptr if no joint is found.
         */
        Joint *findJoint(const String &name) const;

        /** Get the root joint.
         */
        Joint *getRoot() const;

        /** Get joint count.
         */
        size_t getJointCount() const;

        /** Set a local/global pose to this skeleton.
         *  The skeleton will update all its joints' local and global transforms.
         *  If root motion is enabled, the root joint's transform stored in the given pose will be 
         *  regarded as delta transform and it will be added to the skeleton's last root joint's transform
         *  to get the current root joint transform.
         */
        void setPose(const Pose &pose, ESpace pose_space = ESpace::eLocal);

        /** Check if root motion is enabled.
         */
        bool isRootMotionEnabled() const {
            return m_is_root_motion_enabled;
        }

        /** Toggle root motion enable.
         *  If root motion is unabled, the skeleton's root transform won't change after a pose updating.
         */
        void setRootMotionEnable(bool val) {
            m_is_root_motion_enabled = val;
        }

        // When a new joint is added, the parent joint who added the new child is responsable 
        // to call this to notify the skeleton.
        void _onJointAdded(Joint *new_joint);

        // When a new joint is added, the joint being removed is responsable to call this
        // to notify the skeleton.
        void _onJointRemoved(Joint *removed_joint);

        // When a joint changed its name, that joint is responsable to call this method to notify the skeleton.
        void _onJointRenamed(const String &old_name, const String &current_name);
        
    private:
        // Clear all joints and free allocated memory.
        void _clear();

        // Add a joint to the joint list, 
        // If the joint being added has children, this method will call itself recursively.
        void _addJointRecursive(Joint *new_joint);

        // Remove a joint from joints list,
        // If the joint being removed has children, this method will call itself recursively.
        void _removeJointRecursive(Joint *remove_joint);

        // Update the names map if there is a joint renamed.
        void _renameJointImpl(const String &old_name, const String &current_name);

        // Update the root's transform.
        void _updateRootTransform(const Transform &delta_transform);

        // update skeleton's joint transform by given a local pose.
        void _updateOtherJointsFromLclPose(const Pose &pose);

        // update skeleton's joint transform by given a local pose.
        void _updateOtherJointsFromGlbPose(const Pose &pose);

    private:

        typedef JointPtrList::iterator _JointPtrListItor;
        typedef JointPtrList::const_iterator _ConstJointPtrListItor;

        // A list that stores joints in pre-order.
        // The root joint always stays at front.
        JointPtrList m_joint_list;

        typedef unordered_map<String, _JointPtrListItor> _JointNamesMap;
        typedef _JointNamesMap::iterator _JointNamesMapItor;
        typedef _JointNamesMap::const_iterator _ConstJointNamesMapItor;

        // A map that stores joint's name and its iterator in the joint list.
        // This makes finding joint by name faster.
        _JointNamesMap m_joint_names_map;

        // Indicate if root motion is enabled.
        bool m_is_root_motion_enabled;
    };

    
};
