#pragma once

#ifdef SKANIM_DLL

#ifdef _MSC_VER

#ifdef SKANIMLIB_EXPORTS
#define _SKANIM_EXPORT __declspec(dllexport)
#else 
#define _SKANIM_EXPORT __declspec(dllimport)
#endif

#endif

#else

#define _SKANIM_EXPORT

#endif

namespace Skanim
{
    /** Pre-declaration for classes.
     */
	class MatrixUA4;
	class Quaternion;
    class Transform;
	class Vector3;
	class Vector4;
};


