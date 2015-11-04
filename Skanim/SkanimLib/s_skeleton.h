#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_iteratorwrapper.h"

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

    private:

        typedef JointPtrList::iterator _JointPtrListItor;
        typedef JointPtrList::const_iterator _ConstJointPtrListItor;

        // A list that stores joints in pre-order.
        // The root joint always stays at front.
        JointPtrList m_joint_list;

        typedef map<const String, _JointPtrListItor> _JointNamesMap;
        typedef _JointNamesMap::iterator _JointNamesMapItor;
        typedef _JointNamesMap::const_iterator _ConstJointNamesMapItor;

        // A map that stores joint's name and its iterator in the joint list.
        // This makes finding joint by name faster.
        _JointNamesMap m_joint_names_map;
    };

    
};
