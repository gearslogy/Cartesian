//
// Created by admin on 2020/8/2.
//

#include "CartesianPluginLoader.h"

#include <iostream>
#include <cstdlib>
#include <direct.h>
#include <boost/dll/import.hpp> // for import_alias



namespace dll = boost::dll;
namespace Cartesian{

    std::vector<dllsymbolfunc> PluginLoader::loadPlugins()
    {
        // FUNCTION VECTOR POOL
        std::vector<dllsymbolfunc> dllSignalPools;


        pathdir program_path(std::filesystem::current_path());
        // check parent is document
        std::filesystem::directory_entry entry(program_path);
        assert(entry.status().type() == std::filesystem::file_type::directory);
        std::cout << "CARTESIAN::Loading plugins: " << program_path << std::endl;

        // going to the program docment, then find a DIR: dso
        std::filesystem::directory_iterator files(program_path) ;
        for(auto &it: files){
            if(it.status().type()!= std::filesystem::file_type::directory)
                continue;

            // In DSO dir, load the dyn libs *.dll
            const pathdir& dsoDir= it.path();
            std::filesystem::directory_iterator dsoFiles(dsoDir) ;
            for(auto &dso : dsoFiles){
                const pathdir& dsopath = dso.path();

                // if not a file
                if(dso.status().type() == std::filesystem::file_type::directory)
                    continue;

                // if not a dll
                if(dsopath.generic_string().find(".dll") == -1)
                    continue;
                std::cout << "CARTESIAN::Trying to open lib:" << dsopath << std::endl;
                auto dllsig = openlib(dsopath);
                if(dllsig)
                    dllSignalPools.emplace_back(dllsig);
            }
        }
        // Try to find env path
        if(const char* env_p = std::getenv("CARTESIAN_PLUGINS"))
        {
            std::cout << "CARTESIAN::Loading plugins: " << env_p << std::endl;
            std::filesystem::directory_iterator customPluginFiles(env_p);
            for(auto &dso : customPluginFiles){
                const pathdir& dsopath = dso.path();
                if(dso.status().type() == std::filesystem::file_type::directory)
                    continue;
                if(dsopath.generic_string().find(".dll") == -1)
                    continue;
                std::cout << "CARTESIAN::Trying to open lib:" << dsopath << std::endl;
                auto dllsig = openlib(dsopath);
                if(dllsig)
                    dllSignalPools.emplace_back(dllsig);
            }
        }

        return std::move(dllSignalPools);
    } // end of loading plugins

    dllsymbolfunc PluginLoader::openlib(const pathdir &libPath) {
        auto path = libPath.generic_string();
        // try to search the function object
        // OUR SIGNAL IS:
        // extern "C" CARTESIAN_EXPORT void newOpFunction( void *table);

        dll::shared_library lib(path);
        std::string symbol=  "newOp";

        if(lib.has(symbol))
        {
            std::cout <<"CARTESIAN::Register function from:" <<libPath << std::endl;
            std::function<void (void *)> newOpSignal = dll::import<void(void *)>(lib,symbol);
            //newOpSignal(lua); // we do not eval in here
            return std::move(newOpSignal);
        }
        return nullptr;
    }

    void PluginLoader::dispatch(const std::vector<dllsymbolfunc> &signals, sol::state *lua) {
        for(auto &sym: signals){
            sym(lua);
        }
    }

}



