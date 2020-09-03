//
// Created by admin on 2020/8/3.
//
//#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1
//#define SOL_NO_EXCEPTIONS 1
//#define SOL_ALL_SAFETIES_ON 0
#undef interface
#include "KatanaCartesian.h"
#include "sol.hpp"
#include "BindKatanaGlobal.h"
#include "BindKatanaData.h"
#include "BindKatanaCH.h"
#include "CartesianPluginLoader.h"
#include "CartesianLog.h"


#undef interface

// -----
static bool __is_bind_once = false;
static bool __is_init_cartesian = false;

static auto lua = std::make_shared<sol::state>();
static std::vector<Cartesian::dllsymbolfunc> dllfuncs;

void MesserOp::InitCartesian() {
    lua->open_libraries(sol::lib::table,
        sol::lib::base,
        sol::lib::jit,
        sol::lib::ffi,
        sol::lib::package,
        sol::lib::io,
        sol::lib::math,
        sol::lib::os,
        sol::lib::utf8,
        sol::lib::coroutine);
    Cartesian::Log::initialize();
    CARTESIAN_CORE_INFO("Loading plugins for cartesian");
    dllfuncs = Cartesian::PluginLoader::loadPlugins();
    Cartesian::PluginLoader::dispatch(dllfuncs, lua.get());
    __is_init_cartesian = true;
}



void MesserOp::RegisterPerCookFunctionOrVar(Foundry::Katana::GeolibCookInterface& iface, const std::shared_ptr<sol::state>& lua)
{

    // Bind Katana Global Variable , it's should every update when katana cooking
    Cartesian::BindGlobalVars::bind(iface, lua);
    
}

void MesserOp::RegisterOnceFunctionOrVar(Foundry::Katana::GeolibCookInterface& iface, const std::shared_ptr<sol::state>& lua) {
   
    Cartesian::BindKatanaCH::bind(iface, lua);
    Cartesian::BindKatanaFunction::bind(iface, lua);
}



void MesserOp::cook(Foundry::Katana::GeolibCookInterface & iface) {

    const int inputIndex = int(FnAttribute::FloatAttribute(
        iface.getOpArg("inputIndex")).getValue(1.0, false));



    //Foundry::Katana::CreateLocationInfo infof;
    //Foundry::Katana::CreateLocation(infof, iface, "/root/world/geo/houdini");


    /*
    // In order to run the Op we need a valid CEL statement
    FnAttribute::StringAttribute celAttr = iface.getOpArg("CEL");
    if (!celAttr.isValid())
    {
        std::cout << "Can not find CEL\n";
        iface.stopChildTraversal();
        return;
    }
    //Foundry::Katana::GeolibOp::flush();



    // If a CEL attribute was provided (and so it's valid), check
    // our current output location against the CEL. If it doesn't match
    // the CEL, then don't continue cooking at this location.
    // Use the utility function matchesCEL() to populate a
    // MatchesCELInfo object that contains useful attributes we can
    // query
    FnGeolibServices::FnGeolibCookInterfaceUtils::MatchesCELInfo info;

    FnGeolibServices::FnGeolibCookInterfaceUtils::matchesCEL(info,
        iface,
                                                             celAttr);

    // If there's no chance that the CEL expression matches any child
    // locations, stop the Op from continuing its recursion past this
    // point in the hierarchy. This isn't required, but improves
    // efficiency.
    if (!info.canMatchChildren)
    {
        iface.stopChildTraversal();
    }

    // If the CEL doesn't match the current location, stop cooking
    if (!info.matches)
    {
        return;
    }
    */

    if (!__is_init_cartesian) {
        InitCartesian();
    }
    
    RegisterPerCookFunctionOrVar(iface, std::move(lua));
    // Only bind to lua once !
    if(!__is_bind_once){
        RegisterOnceFunctionOrVar(iface, std::move(lua));
        __is_bind_once = true;
    }
    
    FnAttribute::StringAttribute scriptAttr = iface.getOpArg("script");
    if (!scriptAttr.isValid()) {
        Foundry::Katana::ReportError(iface, " Can not get script");
    }

    lua->safe_script(scriptAttr.getValue());

}


DEFINE_GEOLIBOP_PLUGIN(MesserOp)

void registerPlugins()
{
    REGISTER_PLUGIN(MesserOp, "CartesianScript", 0, 1);
}