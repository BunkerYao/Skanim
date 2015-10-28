#pragma once

#include "s_iallocmanager.h"

namespace Skanim
{
    /** 
     */
    class _SKANIM_EXPORT MemoryConfig
    {
    public:
        /** Delete constructors to avoid instantiation.
         */
        MemoryConfig() = delete;

        /** Delete copy constructors to avoid instantiation.
        */
        MemoryConfig(const MemoryConfig &) = delete;

        /** Allocate a block of memory.
         */
        static void *_malloc(size_t n_bytes, const wchar_t *file = nullptr, int line = 0, const wchar_t *func = nullptr)
        {
            return _alloc_manager->allocateBytes(n_bytes, file, line, func);
        }

        /** Free the memory allocated by _malloc()
         */
        static void _free(void *ptr)
        {
            _alloc_manager->deallocateBytes(ptr);
        }

        /** Allocate memory and construct object T.
         */
        template <typename T>
        static void *_new_T(const wchar_t *file = nullptr, int line = 0, const wchar_t *func = nullptr)
        {
            return new (_alloc_manager->allocateBytes(sizeof(T), file, line, func)) T;
        }

        /** Destroy object T and deallocate memory.
         */
        template <typename T>
        static void _delete_T(void *ptr)
        {
            static_cast<T*>(ptr)->~T();
            _alloc_manager->deallocateBytes(ptr);
        }

        /** Allocate memory for array and construct n number of object T.
         */
        template <typename T>
        static void *_new_array_T(size_t n, const wchar_t *file = nullptr, int line = 0, const wchar_t *func = nullptr)
        {
            void *ptr = _alloc_manager->allocateBytes(sizeof(T) * n, file, line, func);
            T *ptrT = static_cast<T*>(ptr);
            // Construct all the objects with placement new.
            for (size_t i = 0; i < n; ++i)
                new (ptrT + i) T;
            return ptr;
        }

        /** Destroy n number of object T and deallocate the array's memory.
         */
        template <typename T>
        static void _delete_array_T(void *ptr, size_t n)
        {
            T *ptrT = static_cast<T*>(ptr);
            // Destroy all the objects by calling the destructor.
            for (size_t i = 0; i < n; ++i)
                (ptrT + i)->~T();

            _alloc_manager->deallocateBytes(ptr);
        }

        /** Set the global alloc manager.
         *  The alloc manager is initialized to nullptr. 
         *  So set a valid alloc manager before any function call of this file.
         */
        static void setGlobalAllocManager(IAllocManager *manager)
        {
            _alloc_manager = manager;
        }

        /** Get the global alloc manager.
         */
        static IAllocManager *getGlobalAllocManager()
        {
            return _alloc_manager;
        }

    private:
        // The alloc manager that be used globally to allocate and free physical memory.
        static IAllocManager *_alloc_manager;
    };
};

/** Define useful helper macros to simplify memory allocation.
 *  Don't use functions above directly.
 */

/** Allocate a block of memory.
 */
#   define SKANIM_MALLOC(n) Skanim::MemoryConfig::_malloc(n, __FILEW__, __LINE__, __FUNCTIONW__)

/** Free a block of memory allocated by SKANIM_MALLOC
 */
#   define SKANIM_FREE(p) Skanim::MemoryConfig::_free(p)

/** Allocate and construct object of type T.
 */
#   define SKANIM_NEW_T(T) static_cast<T*>(Skanim::MemoryConfig::_new_T<T>(__FILEW__, __LINE__, __FUNCTIONW__))

/** Destroy object of type T and free the memory.
 */
#   define SKANIM_DELETE_T(T, p) Skanim::MemoryConfig::_delete_T<T>(p)

/** Allocate a block of memory for array and construct n object of type T
 */
#   define SKANIM_NEW_ARRAY_T(T, n) static_cast<T*>(Skanim::MemoryConfig::_new_array_T<T>(n, __FILEW__, __LINE__, __FUNCTIONW__))

/** Destroy n objects of type T and free the memory.
 */
#   define SKANIM_DELETE_ARRAY_T(T, p, n) Skanim::MemoryConfig::_delete_array_T<T>(p, n)
