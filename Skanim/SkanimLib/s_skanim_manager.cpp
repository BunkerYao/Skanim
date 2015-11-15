#include "s_precomp.h"
#include "s_skanim_manager.h"
#include "s_default_alloc_manager.h"
#include "s_memory_config.h"

namespace Skanim
{
    bool SkanimManager::_initialize()
    {
        // Create a default alloc manager.
        DefaultAllocManager *alloc_manager = new DefaultAllocManager();
        // Set the default alloc manager.
        MemoryConfig::setGlobalAllocManager(alloc_manager);

        return true;
    }

    void SkanimManager::destroy()
    {
        IAllocManager *alloc_manager = MemoryConfig::getGlobalAllocManager();
        // Delete the alloc manager.
        delete alloc_manager;
        MemoryConfig::setGlobalAllocManager(nullptr);
    }

    SkanimManager* SkanimManager::create()
    {
        SkanimManager *manager = new SkanimManager;

        if (manager->_initialize())
            return manager;
        else
            return nullptr;
    }

};