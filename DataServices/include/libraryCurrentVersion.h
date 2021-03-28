#pragma once

#include "version.h"

class LibraryCurrentVersion
{
public:
    static Version &getInstance()
    {
        static Version instance(1, 1, 0);
        return instance;
    }
    LibraryCurrentVersion(LibraryCurrentVersion const&) = delete;
    void operator=(LibraryCurrentVersion const&) = delete;
};