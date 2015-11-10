#pragma once

#include "s_platform.h"



namespace Skanim
{
    // Pre-declaration for classes.
    // Decrease dependencies between files.
    class AnimationClip;
    class Joint;
    class MatrixUA4;
    class Pose;
    class Quaternion;
    class Skeleton;
    class Transform;
    class Vector3;
    class Vector4;
};

#include <string>
#include "s_allocator.h"

// Switch that controlls if string uses custom allocators.
#define SKANIM_STRING_USE_CUSTOM_ALLOCATOR 1

// Switch that controlls if containers use custom allocators.
#define SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR 1

// Switch that controlls if wide character is used.
#define SKANIM_USE_WCHAR_T 1

namespace Skanim
{
// Definitions for string types.
#if SKANIM_USE_WCHAR_T == 1

#if SKANIM_STRING_USE_CUSTOM_ALLOCATOR == 1
    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, Allocator<wchar_t>> _BasicString;
    typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, Allocator<wchar_t>> _BasicStringStream;
#else
    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>> _BasicString;
    typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>> _BasicStringStream;
#endif

#else

#if SKANIM_STRING_USE_CUSTOM_ALLOCATOR == 1
    typedef std::basic_string<char, std::char_traits<char>, Allocator<char>> _BasicString;
    typedef std::basic_stringstream<char, std::char_traits<char>, Allocator<char>> _BasicStringStream;
#else
    typedef std::basic_string<char, std::char_traits<char>> _BasicString;
    typedef std::basic_stringstream<char, std::char_traits<char>> _BasicStringStream;
#endif

#endif

    typedef _BasicString String;
    typedef _BasicStringStream StringStream;

// Definitions for stl containers.
#if SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR == 1
    template <typename T>
    using vector = std::vector<T, Allocator<T>>;

    template <typename T>
    using deque = std::deque<T, Allocator<T>>;

    template <typename T>
    using list = std::list<T, Allocator<T>>;

    template <typename K, typename V, typename P = std::less<K>>
    using map = std::map<K, V, P, Allocator<std::pair<const K, V>>>;

    template <typename K, typename V, typename H = std::hash<K>, typename P = std::equal_to<K>>
    using unordered_map = std::unordered_map<K, V, H, P, Allocator<std::pair<const K, V>>>;
#else
    template <typename T>
    using vector = std::vector<T>;

    template <typename T>
    using deque = std::deque<T>;

    template <typename T>
    using list = std::list<T>;

    template <typename K, typename V, typename P = std::less<K>>
    using map = std::map<K, V, P>;

    template <typename K, typename V, typename H = std::hash<K>, typename P = std::equal_to<K>>
    using unordered_map = std::unordered_map<K, V, H, P>>;
#endif


//    template <typename T, typename A = Allocator<T>>
//    struct vector
//    {
//#if SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR == 1
//        typedef typename std::vector<T, A> type;
//        typedef typename std::vector<T, A>::iterator iterator;
//        typedef typename std::vector<T, A>::const_iterator const_iterator;
//#else
//        typedef typename std::vector<T> type;
//        typedef typename std::vector<T>::iterator iterator;
//        typedef typename std::vector<T>::const_iterator const_iterator;
//#endif
//    };
//
//    template <typename T, typename A = Allocator<T>>
//    struct deque
//    {
//#if SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR == 1
//        typedef typename std::deque<T, A> type;
//        typedef typename std::deque<T, A>::iterator iterator;
//        typedef typename std::deque<T, A>::const_iterator const_iterator;
//#else
//        typedef typename std::deque<T> type;
//        typedef typename std::deque<T>::iterator iterator;
//        typedef typename std::deque<T>::const_iterator const_iterator;
//#endif
//    };
//
//    template <typename T, typename A = Allocator<T>>
//    struct list
//    {
//#if SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR == 1
//        typedef typename std::list<T, A> type;
//        typedef typename std::list<T, A>::iterator iterator;
//        typedef typename std::list<T, A>::const_iterator const_iterator;
//#else
//        typedef typename std::list<T> type;
//        typedef typename std::list<T>::iterator iterator;
//        typedef typename std::list<T>::const_iterator const_iterator;
//#endif
//    };
//
//    template <typename K, typename V, typename P = std::less<K>, typename A = Allocator<std::pair<const K, V>> >
//    struct map
//    {
//#if SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR == 1
//        typedef typename std::map<K, V, P, A> type;
//        typedef typename std::map<K, V, P, A>::iterator iterator;
//        typedef typename std::map<K, V, P, A>::const_iterator const_iterator;
//#else
//        typedef typename std::map<K, V, P> type;
//        typedef typename std::map<K, V, P>::iterator iterator;
//        typedef typename std::map<K, V, P>::const_iterator const_iterator;
//#endif
//    };

};


