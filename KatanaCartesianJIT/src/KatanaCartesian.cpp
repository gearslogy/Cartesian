//
// Created by admin on 2020/8/3.
//

#include "KatanaCartesian.h"
#include "sol.hpp"
#include "BindKatanaGlobal.h"
#include "BindKatanaData.h"
#include "BindKatanaCH.h"
#include "CartesianPluginLoader.h"

// -----
static bool __is__init__katana__resources = false;
static bool __is__cartesin__load__plugins = false;
static bool __is__opened__jit__lib = false;
static auto lua = std::make_shared<sol::state>();
static std::vector<Cartesian::dllsymbolfunc> dllfuncs;

void MesserOp::cook(Foundry::Katana::GeolibCookInterface &interface) {



    // In order to run the Op we need a valid CEL statement
    FnAttribute::StringAttribute celAttr = interface.getOpArg("CEL");
    if (!celAttr.isValid())
    {
        interface.stopChildTraversal();
        return;
    }



    // If a CEL attribute was provided (and so it's valid), check
    // our current output location against the CEL. If it doesn't match
    // the CEL, then don't continue cooking at this location.
    // Use the utility function matchesCEL() to populate a
    // MatchesCELInfo object that contains useful attributes we can
    // query
    FnGeolibServices::FnGeolibCookInterfaceUtils::MatchesCELInfo info;

    FnGeolibServices::FnGeolibCookInterfaceUtils::matchesCEL(info,
                                                             interface,
                                                             celAttr);

    // If there's no chance that the CEL expression matches any child
    // locations, stop the Op from continuing its recursion past this
    // point in the hierarchy. This isn't required, but improves
    // efficiency.
    if (!info.canMatchChildren)
    {
        interface.stopChildTraversal();
    }

    // If the CEL doesn't match the current location, stop cooking
    if (!info.matches)
    {
        return;
    }

    if(!__is__opened__jit__lib){
        lua->open_libraries(sol::lib::base,sol::lib::jit,sol::lib::ffi,sol::lib::package,sol::lib::coroutine);
        __is__opened__jit__lib = true;
    }
    Foundry::Katana::CreateLocationInfo cinfo;
    FnGeolibServices::CreateLocation(cinfo,interface,"/root/world/FFFFFFFFFFFF");

    // Bind Katana Global Variable , it's should every update when katana cooking
    Cartesian::BindGlobalVars::bind(interface, lua);
    Cartesian::BindGlobalFunction::bind(interface,lua);



    // Only bind to lua once !
    if(!__is__init__katana__resources){
        std::cout << "[KATANA::CARTESIAN]: now register KATANA resources to JIT \n";
        Cartesian::BindKatanaFunction::bind(interface,lua);
        Cartesian::BindKatanaCH::bind(interface,lua);
        __is__init__katana__resources = true;
    }

    // only bind to lua once !
    if(!__is__cartesin__load__plugins){
        std::cout << "[KATANA::CARTESIAN]: now init internal plugins\n";
        dllfuncs = Cartesian::PluginLoader::loadPlugins();
        Cartesian::PluginLoader::dispatch(dllfuncs,lua.get());
        __is__cartesin__load__plugins = true;
    }




    // Bind Katana interface





    // Get the Katana-UI scripts
    FnAttribute::StringAttribute scriptAttr = interface.getOpArg("script");
    if(scriptAttr.isValid()){
        try {
            lua->safe_script(scriptAttr.getValue());
        }
        catch (std::runtime_error &e) {
            std::cout << "[KATANA::CARTESIAN ERROR]:" << e.what() << std::endl;
        }
    }
    else{
        Foundry::Katana::ReportError(interface,"KATANA::ERROR::CARTESIAN can not get the script code\n");
    }




    /*
    FnAttribute::FloatAttribute displacementAttr =
            interface.getOpArg("displacement");

    const float displacement =
            displacementAttr.getValue(DEFAULT_DISPLACEMENT, false);

    FnAttribute::FloatAttribute currPointsAttr =
            interface.getAttr("geometry.point.P");

    if (currPointsAttr.isValid())
    {
        // Initialize the pseudo-random number generator
        srand(2501);

        const int64_t tupleSize = currPointsAttr.getTupleSize();
        // Only vectors with tuple size = 3 are supported
        if (tupleSize > 4)
        {
            // If the tuple size is not supported we stop the Ops execution
            // and notify the user
            Foundry::Katana::ReportError(interface,
                                         "Unsupported tuple size for 'geometry.point.P'.");
            interface.stopChildTraversal();
            return;
        }

        FnAttribute::FloatConstVector currPointsVec =
                currPointsAttr.getNearestSample(0.0f);
        const int64_t numValues = currPointsVec.size();
        const float * currPoints = currPointsVec.data();

        float * newPoints = new float[numValues];
        for (int64_t i = 0; i < numValues; i += tupleSize)
        {
            makeNewPoint(&newPoints[i], &currPoints[i], tupleSize,
                         displacement);
        }

        // Create and set the new point positions attribute
        FnAttribute::FloatAttribute newPointsAttr(newPoints, numValues,
                                                  tupleSize);
        interface.setAttr("geometry.point.P", newPointsAttr, false);

        // Clean-up
        delete [] newPoints;
    }*/
}



DEFINE_GEOLIBOP_PLUGIN(MesserOp)

void registerPlugins()
{
    REGISTER_PLUGIN(MesserOp, "CartesianScript", 0, 1);
}