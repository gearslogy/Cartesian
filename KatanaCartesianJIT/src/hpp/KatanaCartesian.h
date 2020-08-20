//
// Created by admin on 2020/8/3.
//

#ifndef BUILDINGCTSLUAJIT_KATANACARTESIAN_H
#define BUILDINGCTSLUAJIT_KATANACARTESIAN_H
#include <cstdlib>
#include <cmath>
#include <cassert>


#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>
#include <FnGeolib/op/FnGeolibOp.h>
#include <FnGeolib/util/Path.h>
#include <FnPluginSystem/FnPlugin.h>
#include <pystring/pystring.h>
#include <FnGeolibServices/FnGeolibCookInterfaceUtilsService.h>

class MesserOp : public Foundry::Katana::GeolibOp
{
public:
    static void setup(Foundry::Katana::GeolibSetupInterface & interface)
    {
        interface.setThreading(
                Foundry::Katana::GeolibSetupInterface::ThreadModeConcurrent);
    }
    static void InitCartesian();

    // every katana cook life time, maybe update this function or vars;
    // Emp: Time/ type
    static void RegisterPerCookFunctionOrVar(Foundry::Katana::GeolibCookInterface& interface);

    // Only register function once!
    static void RegisterOnceFunctionOrVar(Foundry::Katana::GeolibCookInterface& interface);      

    // katana cook function
    static void cook(Foundry::Katana::GeolibCookInterface & interface);


};





#endif //BUILDINGCTSLUAJIT_KATANACARTESIAN_H
