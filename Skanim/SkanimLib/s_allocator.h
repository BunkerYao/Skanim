#pragma once

#include "s_precomp.h"
#include "s_memoryconfig.h"

namespace Skanim
{
    /** The allocator used by std containers and other memory
     *  allocation/deallocation operations.
     */
    template <typename T>
    class Allocator
    {
    public:
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
        typedef T           *pointer;
        typedef const T     *const_pointer;
        typedef T           &reference;
        typedef const T     &const_reference;
        typedef T           value_type;

        Allocator() = default;

        Allocator(const Allocator &) = default;

        template <typename U>
        Allocator(const Allocator<U> &) noexcept
        {}

        ~Allocator() = default;

        template <typename U>
        struct rebind
        {
            typedef Allocator<U> other;
        };

        pointer address(reference r) const
        {
            return &r;
        }

        const_pointer address(const_reference c) const
        {
            return &c;
        }

        size_type max_size() const
        {
            return MemoryConfig::getGlobalAllocManager()->getMaxAllocationSize();
        }

        /** Allocate memory for n number object T but doesn't construct them.
         *  The second argumant is not used.
         */
         pointer allocate(size_type n, const void *p = nullptr)
         {
             assert(n > 0);
             return static_cast<pointer>(MemoryConfig::getGlobalAllocManager()->allocateBytes(n * sizeof(T)));
         }

        /** Deallocate the memory which is allocated by allocate()
         *  The second argumant is not used.
         */
         void deallocate(pointer p, size_type size = 0)
         {
             assert(p != nullptr);
             MemoryConfig::getGlobalAllocManager()->deallocateBytes(static_cast<void*>(p));
         }

        /** Construct object on given memory.
         */
        void construct(pointer p, const_reference c)
        {
            // Call placement new.
            new (static_cast<void*>(p)) T(c);
        }

        /** Destroy object on given memory.
         */
        void destroy(pointer p)
        {
            // Call the destructor.
            (p)->~T();
        }
    };

    /** Determine allocator equality.
     */
    template <typename T1, typename T2>
    bool operator==(const Allocator<T1> &, const Allocator<T2> &) noexcept
    {
        // Memory from other allocator can be released by this allocator.
        return true;
    }

    /** Determine allocator equality.
    */
    template <typename T1, typename T2>
    bool operator!=(const Allocator<T1> &, const Allocator<T2> &) noexcept
    {
        // Memory from other allocator can be released by this allocator.
        return false;
    }
};
