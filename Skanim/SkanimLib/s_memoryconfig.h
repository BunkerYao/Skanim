#pragma once

#include "s_iallocmanager.h"

namespace Skanim
{
    /** 
     */
    class MemoryConfig
    {
    public:

        

    private:
        // The alloc manager that be used to allocate and free physical memory.
        static IAllocManager *_alloc_manager;
    };

};
