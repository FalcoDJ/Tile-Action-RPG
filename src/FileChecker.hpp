#ifndef files_funcs
#define files_funcs

#include <string>
#include <vector>
#include "olcPixelGameEngine.h"

class file
{
    public:

    static bool doesExist(const std::string &sPath)
    {
        bool success = true;

        if (!_gfs::exists(sPath))
        {
            std::cout << '"' + sPath + '"' + " Doesn't exist" << std::endl;
            success = false;
        }
        else
        {
            std::cout << "found: " + sPath << std::endl;
        }

        std::cout << std::endl;

        return success;
    }

    static bool batchDoesExist(const std::vector<std::string> &sPaths)
    {
        bool success = true;

        for (const auto &file : sPaths)
        {
            if (!_gfs::exists(file))
            {
                std::cout << '"' + file + '"' + " Doesn't exist" << std::endl;
                success = false;
            }
            else
            {
                std::cout << "found: " + file << std::endl;
            }
        }

        std::cout << std::endl;

        return success;
    }
};

#endif
