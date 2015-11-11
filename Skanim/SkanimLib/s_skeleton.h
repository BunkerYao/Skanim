#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_joint.h"
#include "s_matrixua4.h"

namespace Skanim
{
    /** A skeleton deforms a skinned mesh in games by generating a matrices 
     *  palette based on the current skeleton pose. A skeleton's pose is 
     *  extracted from animation clips. Skeleton contains joints which are
     *  organized in a hierarchy structure. Each joint's could be queried 
     *  from the skeleton.
     */
    class _SKANIM_EXPORT Skeleton
    {
    public:

        /** Construct skeleton.
         *  After the construction, the skeleton has a root created automatically.
         */
        Skeleton(const String &name, const String &root_name) noexcept;

        /** Check if root motion is enabled.
         */
        bool isRootMotionEnabled() const 
        {
            return m_is_root_motion_enabled;
        }

        /** Toggle root motion.
         *  If root motion is unabled, the skeleton's root transform won't change
         *  after a pose updating.
         */
        void setRootMotionEnable(bool val) 
        {
            m_is_root_motion_enabled = val;
        }

        /** Get the name of this skeleton. The name of the skeleton is unchangable
         *  since the AssetManager class uses the name to identify a skeleton.
         */
        const String &getName() const
        {
            return m_name;
        }

        /** Get the number of joint in this skeleton.
         */
        size_t getJointCount() const
        {
            return m_joint_hierarchy_array.size();
        }

        /** Get a joint by its index.
         */
        Joint *getJoint(size_t index)
        {
            assert(index < m_joint_hierarchy_array.size() && 
                "index out of range");
            return &m_joint_hierarchy_array[index];
        }

        /** Find a joint by its name.
         */
        Joint *findJoint(const String &name);

        /** Add a joint to this skeleton in pre-order and attach it to a parent 
         *  joint. The name of the joint being added must be unique in a skeleton.
         *  If the added joint has old parent or children indices then those indices 
         *  will be ignored.
         */
        void addJointPreOrder(const Joint &joint, int parent_index);

        /** Set the skeleton's current pose by given a local space pose.
         */
        void setPose(const Pose &local_pose);

        typedef vector<MatrixUA4> MatricesVector;
        typedef MatricesVector::iterator MatricesVectorIterator;
        typedef MatricesVector::const_iterator MatricesVectorConstIterator;

        /** Get the skinning matrices palette. The skinning matrices generation is 
         *  delayed until this function is called.
         */
        const MatricesVector &getSkinningMatricesPalette();

        /** Modify the root joint's global transform.
         */
        void setRootJointTransform(const Transform &transform);

    private:

        // Add a root joint.
        void _addRootJoint(const String &name);

        // Update the skinning matrices palette.
        void _updateSkinningMatricesPalette();

        // Update sub-part of the joint hierarchy which begins with begin_root_index.
        void _updateSubHierarchyGlbTransform(int begin_root_index);

    private:

        // The constant name of this skeleton.
        const String m_name;
        
        typedef vector<Joint> _JointVector;
        typedef _JointVector::iterator _JointVectorIterator;
        typedef _JointVector::const_iterator _JointVectorConstIterator;

        // The joint hierarchy is stored in an array.
        // Each joint's index is the pre-order number in a tree structure.
        _JointVector m_joint_hierarchy_array;

        typedef unordered_map<String, int> _JointNamesMap;
        typedef _JointNamesMap::iterator _JointNamesMapIterator;
        typedef _JointNamesMap::const_iterator _JointNamesMapConstIterator;

        // The names map that stores joints with its name as key and its index
        // as value. The map provides a convienient way to look up a joint by
        // its name.
        _JointNamesMap m_joint_names_map;

        // Indicate if root motion is enabled.
        bool m_is_root_motion_enabled;

        // The skinning matrices palette. 
        MatricesVector m_skinning_matrices_palette;
        // The skeleton delays the skinning matrices palette generation until it's
        // needed. So here is a boolean flag indicates if the palette need an update.
        bool m_palette_needs_update;
    };

    
};
