//
// Created by admin on 2020/8/3.
//

#ifndef BUILDINGCTSLUAJIT_KATANACARTESIAN_H
#define BUILDINGCTSLUAJIT_KATANACARTESIAN_H

#include <cstdlib>
#include <cmath>
#include <cassert>
// IMPORTANT MACRO
// https://sol2.readthedocs.io/en/latest/exceptions.html#exception-handling
#define SOL_EXCEPTIONS_SAFE_PROPAGATION  1

#if defined _WIN32 || defined __CYGWIN__
#undef interface
#undef GetCurrentTime
#endif //  __MSVC__

#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>
#include <FnGeolib/op/FnGeolibOp.h>
#include <FnGeolib/util/Path.h>
#include <FnPluginSystem/FnPlugin.h>
#include <pystring/pystring.h>
#include <FnGeolibServices/FnGeolibCookInterfaceUtilsService.h>
#include "sol.hpp"
#include "CartesianPluginLoader.h"
#include "CartesianLog.h"

class MesserOp : public Foundry::Katana::GeolibOp
{
public:
    static void setup(Foundry::Katana::GeolibSetupInterface & iface)
    {
        iface.setThreading(
                Foundry::Katana::GeolibSetupInterface::ThreadModeGlobalUnsafe);
    }
    static void InitCartesian(std::shared_ptr<sol::state> lua,  std::vector<Cartesian::dllsymbolfunc>& dllfuncs);

    // every katana cook life time, maybe update this function or vars;
    // Emp: Time/ type
    static void RegisterPerCookFunctionOrVar(Foundry::Katana::GeolibCookInterface& iface, const std::shared_ptr<sol::state>& lua);

    // Only register function once!
    static void RegisterOnceFunctionOrVar(Foundry::Katana::GeolibCookInterface& iface, const std::shared_ptr<sol::state>& lua);

    // katana cook function
    static void cook(Foundry::Katana::GeolibCookInterface & iface);


};





#endif //BUILDINGCTSLUAJIT_KATANACARTESIAN_H
