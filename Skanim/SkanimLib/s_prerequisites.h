#pragma once

#include "s_platform.h"

#define SKANIM_STRING_USE_CUSTOM_ALLOCATOR 1

#define SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR 1

namespace Skanim
{
    // Pre-declaration for classes.
    // Decrease dependencies between files.
    class MatrixUA4;
    class Quaternion;
    class Transform;
    class Vector3;
    class Vector4;
};

#include "s_allocator.h"

namespace Skanim
{
// Defines for string types.
#if SKANIM_STRING_USE_CUSTOM_ALLOCATOR == 1
    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, Allocator<wchar_t>> _BasicString;
    typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, Allocator<wchar_t>> _BasicStringStream;
#else
    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>> _BasicString;
    typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>> _BasicStringStream;
#endif

    typedef _BasicString String;
    typedef _BasicStringStream StringStream;

// Defines for stl containers.
    template <typename T, typename A = Allocator<T>>
    struct vector
    {
#if SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR == 1
        typedef typename std::vector<T, A> type;
        typedef typename std::vector<T, A>::iterator iterator;
        typedef typename std::vector<T, A>::const_iterator const_iterator;
#else
        typedef typename std::vector<T> type;
        typedef typename std::vector<T>::iterator iterator;
        typedef typename std::vector<T>::const_iterator const_iterator;
#endif
    };

    template <typename T, typename A = Allocator<T>>
    struct deque
    {
#if SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR == 1
        typedef typename std::deque<T, A> type;
        typedef typename std::deque<T, A>::iterator iterator;
        typedef typename std::deque<T, A>::const_iterator const_iterator;
#else
        typedef typename std::deque<T> type;
        typedef typename std::deque<T>::iterator iterator;
        typedef typename std::deque<T>::const_iterator const_iterator;
#endif
    };

    template <typename T, typename A = Allocator<T>>
    struct list
    {
#if SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR == 1
        typedef typename std::list<T, A> type;
        typedef typename std::list<T, A>::iterator iterator;
        typedef typename std::list<T, A>::const_iterator const_iterator;
#else
        typedef typename std::list<T> type;
        typedef typename std::list<T>::iterator iterator;
        typedef typename std::list<T>::const_iterator const_iterator;
#endif
    };

    template <typename K, typename V, typename P = std::less<K>, typename A = Allocator<std::pair<const K, V>> >
    struct map
    {
#if SKANIM_CONTAINSERS_USE_CUSTOM_ALLOCATOR == 1
        typedef typename std::map<K, V, P, A> type;
        typedef typename std::map<K, V, P, A>::iterator iterator;
        typedef typename std::map<K, V, P, A>::const_iterator const_iterator;
#else
        typedef typename std::map<K, V, P> type;
        typedef typename std::map<K, V, P>::iterator iterator;
        typedef typename std::map<K, V, P>::const_iterator const_iterator;
#endif
    };


};


