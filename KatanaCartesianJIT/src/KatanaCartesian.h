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
    static void setup(Foundry::Katana::GeolibSetupInterface &interface)
    {
        interface.setThreading(
                Foundry::Katana::GeolibSetupInterface::ThreadModeConcurrent);
    }
    static void cook(Foundry::Katana::GeolibCookInterface &interface);


};





#endif //BUILDINGCTSLUAJIT_KATANACARTESIAN_H
