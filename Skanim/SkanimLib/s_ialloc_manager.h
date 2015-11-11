#pragma once

#include "s_prerequisites.h"

namespace Skanim
{
    /** AllocManager is the interface that used by SkanimAllocator to actually
     *  does the allocation and deallocation of physical memory.
     */
    class _SKANIM_EXPORT IAllocManager
    {
    public:
        virtual ~IAllocManager() = 0
        {}

        /** Allocate bytes and return the allocated memory pointer.
         */
        virtual void *allocateBytes(size_t count, 
            const wchar_t *file = nullptr, 
            int line = 0, 
            const wchar_t *func = nullptr) = 0;

        /** Deallocate bytes.
         */
        virtual void deallocateBytes(void *ptr) = 0;

        /** Get the maximum size of a single allocation. 
         */
        virtual size_t getMaxAllocationSize() = 0;
    };
};
