//
// Created by admin on 2020/8/2.
//

#ifndef BUILDINGCTSLUAJIT_CARTESIANPLUGINLOADER_H
#define BUILDINGCTSLUAJIT_CARTESIANPLUGINLOADER_H
#include "CartesianConfig.h"
#include <filesystem>
#include "sol.hpp"
#include <vector>

namespace Cartesian{
    using dllsymbolfunc = std::function<void (void *)>;
    class CARTESIAN_API PluginLoader{

    public:
        using pathdir = std::filesystem::path;
        static std::vector<dllsymbolfunc> loadPlugins();
        static dllsymbolfunc openlib(const pathdir &libPath);
        static void dispatch(const std::vector<dllsymbolfunc> &signals,sol::state *lua);


};



}






#endif //BUILDINGCTSLUAJIT_CARTESIANPLUGINLOADER_H
