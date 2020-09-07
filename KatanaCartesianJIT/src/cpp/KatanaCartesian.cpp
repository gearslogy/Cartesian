//
// Created by admin on 2020/8/3.
//
//#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1
//#define SOL_ALL_SAFETIES_ON 0
#undef interface
#include "KatanaCartesian.h"
#include "BindKatanaGlobal.h"
#include "BindKatanaData.h"
#include "BindKatanaCH.h"



#undef interface


bool isInitCartesian = false;
static bool __is_bind_once = false;
static bool __is_init_cartesian = false;

inline void my_panic(sol::optional<std::string> maybe_msg) {
    std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
    if (maybe_msg) {
        const std::string& msg = maybe_msg.value();
        std::cerr << "\terror message: " << msg << std::endl;
    }
    // When this function exits, Lua will exhibit default behavior and abort()
}


int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
    // L is the lua state, which you can wrap in a state_view if necessary
    // maybe_exception will contain exception, if it exists
    // description will either be the what() of the exception or a description saying that we hit the general-case catch(...)

    if (maybe_exception) {
        std::cout << "Found the exception, will QueryAbort Exception\n";
        //throw Foundry::Katana::GeolibCookInterface::QueryAbortException();
        //const std::exception& ex = *maybe_exception;
        //std::cout << ex.what() << std::endl;
    }
    else {
        std::cout << "(from the description parameter): ";
        std::cout.write(description.data(), description.size());
        std::cout << std::endl;
    }

    // you must push 1 element onto the stack to be 
    // transported through as the error object in Lua
    // note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
    // so we push a single string (in our case, the description of the error)
    return sol::stack::push(L, description);
}


static auto lua = std::make_shared<  sol::state   >();
static std::vector<Cartesian::dllsymbolfunc> dllfuncs;


void MesserOp::InitCartesian(std::shared_ptr<sol::state> lua, std::vector<Cartesian::dllsymbolfunc>& dllfuncs) {
    //lua->set_exception_handler(&my_exception_handler);
    //lua->set_panic(sol::c_call<decltype(&my_panic), &my_panic>);
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
    //CARTESIAN_CORE_INFO("Loading plugins for cartesian");
    dllfuncs = Cartesian::PluginLoader::loadPlugins();
    Cartesian::PluginLoader::dispatch(dllfuncs, lua.get());
}



void MesserOp::RegisterPerCookFunctionOrVar(Foundry::Katana::GeolibCookInterface& iface, const std::shared_ptr<sol::state>& lua)
{
    //std::cout << "RegisterPerCookFunctionOrVar:" << &iface << std::endl;
    // Bind Katana Global Variable , it's should every update when katana cooking
    Cartesian::BindGlobalVars::bind(iface, lua);

}

void MesserOp::RegisterOnceFunctionOrVar(Foundry::Katana::GeolibCookInterface& iface, const std::shared_ptr<sol::state>& lua) {
    //std::cout << "RegisterOnceFunctionOrVar:" << &iface << std::endl;
    Cartesian::BindKatanaCH::bind(iface, lua);
    Cartesian::BindKatanaFunction::bind(iface, lua);
}



void MesserOp::cook(Foundry::Katana::GeolibCookInterface& iface)
{

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




 

    if (!isInitCartesian) {
        InitCartesian(lua, dllfuncs);

        isInitCartesian = true;
    }
    if (!__is_bind_once) {
        RegisterPerCookFunctionOrVar(iface, std::move(lua));
        RegisterOnceFunctionOrVar(iface, std::move(lua));
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
        }
        catch (const std::runtime_error& e) {
            throw Foundry::Katana::GeolibCookInterface::QueryAbortException();
        }
    }
}


DEFINE_GEOLIBOP_PLUGIN(MesserOp)

void registerPlugins()
{
    REGISTER_PLUGIN(MesserOp, "CartesianScript", 0, 1);
}