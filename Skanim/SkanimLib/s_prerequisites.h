#pragma once

#ifdef SKANIM_DLL

#ifdef _MSC_VER

#ifdef SKANIMLIB_EXPORTS
#define SKANIM_API __declspec(dllexport)
#else 
#define SKANIM_API __declspec(dllimport)
#endif

#endif

#else

#define SKANIM_API

#endif

namespace Skanim
{
	class Quaternion;
	class Vector3;
	class Vector4;
};


