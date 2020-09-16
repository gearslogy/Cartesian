//
// Created by admin on 2020/8/3.
//
//#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1
//#define SOL_ALL_SAFETIES_ON 0
#include<mutex>
#undef interface
#include "KatanaCartesian.h"
#include "BindKatanaGlobal.h"
#include "BindKatanaData.h"
#include "BindKatanaCH.h"



static bool isInitCartesian = false;
static bool __is_bind_once = false;
static bool __is_init_cartesian = false;
static std::mutex mutex;

static std::shared_ptr<sol::state> luaPtr = nullptr;
static std::vector<Cartesian::dllsymbolfunc> dllfuncs;


void MesserOp::InitCartesian(sol::state* lua, std::vector<Cartesian::dllsymbolfunc>& dllfuncs) {
    //lua->set_exception_handler(&my_exception_handler);
    //lua->set_panic(sol::c_call<decltype(&my_panic), &my_panic>);
    //std::cout << "----------- open lub libs -------------------\n";
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
    //std::cout << "----------- loading cartesian plugins -------------------\n";
    dllfuncs = Cartesian::PluginLoader::loadPlugins();
    Cartesian::PluginLoader::dispatch(dllfuncs, lua);
}



void MesserOp::RegisterPerCookFunctionOrVar(Foundry::Katana::GeolibCookInterface& iface, sol::state* lua)
{
    //std::cout << "RegisterPerCookFunctionOrVar:" << &iface << std::endl;
    // Bind Katana Global Variable , it's should every update when katana cooking
    Cartesian::BindGlobalVars::bind(iface, lua);

}

void MesserOp::RegisterOnceFunctionOrVar(Foundry::Katana::GeolibCookInterface& iface, sol::state* lua) {
    //std::cout << "RegisterOnceFunctionOrVar:" << &iface << std::endl;
    Cartesian::BindKatanaCH::bind(iface, lua);
    Cartesian::BindKatanaFunction::bind(iface, lua);
}



void MesserOp::cook(Foundry::Katana::GeolibCookInterface& iface)
{


 
    mutex.lock();
    if (!isInitCartesian) { // only create at root
        luaPtr = std::make_shared<sol::state>();
        InitCartesian(luaPtr.get(), dllfuncs);
        isInitCartesian = true;
    }
    mutex.unlock();
    const int inputIndex = int(FnAttribute::FloatAttribute(
        iface.getOpArg("inputIndex")).getValue(1.0, false));

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


    mutex.lock();
    RegisterPerCookFunctionOrVar(iface, luaPtr.get());
    RegisterOnceFunctionOrVar(iface, luaPtr.get());
    mutex.unlock();

    try {
        FnAttribute::StringAttribute scriptAttr = iface.getOpArg("script");
        luaPtr->safe_script(scriptAttr.getValue());
  
    }
    catch (const std::runtime_error& e) {
        throw Foundry::Katana::GeolibCookInterface::QueryAbortException();
    }






    /*
    if (!isInitCartesian) {
        //InitCartesian(lua.get(), dllfuncs);
        //auto dllfuncs = dllfuncsptrs.get();
        static std::vector<Cartesian::dllsymbolfunc> dllfuncs;
        InitCartesian(lua.get(), dllfuncs);

        isInitCartesian = true;
       

    }
    std::cout << "start bind cartesian\n";
    if (!__is_bind_once) {
        RegisterPerCookFunctionOrVar(iface, lua.get());
        RegisterOnceFunctionOrVar(iface, lua.get());
        __is_bind_once = true;
    }

    FnAttribute::StringAttribute scriptAttr = iface.getOpArg("script");
    if (!scriptAttr.isValid()) {
        Foundry::Katana::ReportError(iface, " Can not get script");
    }
    if (isInitCartesian && __is_bind_once)
    {
        try {
       
            lua->safe_script(scriptAttr.getValue());
            //lua.safe_script(scriptAttr.getValue());
        }
        catch (const std::runtime_error& e) {
            throw Foundry::Katana::GeolibCookInterface::QueryAbortException();
        }
    }
    //std::cout << "release lua\n";
    //ReleaseCartesian(lua, dllfuncs);

    */
   
}


DEFINE_GEOLIBOP_PLUGIN(MesserOp)

void registerPlugins()
{
    REGISTER_PLUGIN(MesserOp, "CartesianScript", 0, 1);
}