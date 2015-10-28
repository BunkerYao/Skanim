#include "s_precomp.h"
#include "s_memoryconfig.h"

namespace Skanim
{
    // Initialize the global alloc manager to nullptr.
    IAllocManager *MemoryConfig::_alloc_manager = nullptr;
};
