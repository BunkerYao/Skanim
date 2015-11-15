#pragma once

#include "s_prerequisites.h"

namespace Skanim
{
    /**
     */
    class _SKANIM_EXPORT SkanimManager
    {
    public:

        /** Destroy the skanim manager.
         */
        void destroy();

        /** Create a skanim manager.
         */
        static SkanimManager* create();

    private:

        // The default constructor.
        SkanimManager() = default;

        /** Initialize the manager.
        */
        bool _initialize();

    };
};
