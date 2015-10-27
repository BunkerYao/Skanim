#pragma once

#include "s_precomp.h"
#include "s_prerequisites.h"
#include "s_iallocmanager.h"

namespace Skanim
{
    /** This is the default implementation for AllocManager.
     *  It simply uses malloc() and free() to manage physical memory.
     */
    class _SKANIM_EXPORT DefaultAllocManager : public IAllocManager
    {
    public:
        virtual ~DefaultAllocManager() = default;

        virtual void *allocateBytes(size_t count, 
            const wchar_t *file = nullptr, int line = 0, const wchar_t *func = nullptr) override
        {
            void *ptr = malloc(count);
            assert(ptr);
            return ptr;
        }

        virtual void deallocateBytes(void *ptr) override
        {
            free(ptr);
        }

        virtual size_t getMaxAllocationSize() override
        {
            return std::numeric_limits<size_t>::max();
        }
    };
};
