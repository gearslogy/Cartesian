### OPENSOURCE LIBS:
* boost::dll
* boost::filesystem
* sol3 & luajit
* GLM
* CGAL


# CartesianLuajitCore
* it's a core
* plugins based
* compiled the luajit(static),boost::filesystem(static) ... all to the core
* so if you write app based on cartesian, just link cartesian, only include there headers

# CartesianPlugins
* write the plugin for Cartesian core
* if you want write plugin for cartesian,just try this:
```C++
#include "CartesianEntryPoint.h"
#include "sol.hpp"
#include <cmath>

void newOp(void *lua){
    auto *luaPtr = static_cast<sol::state *> (lua);
    luaPtr->set_function("sin",static_cast<float(*) (float)> (&sin));
    luaPtr->set_function("cos",static_cast<float(*) (float)> (&cos));
    luaPtr->set_function("acos",static_cast<float(*) (float)> (&acos));
    luaPtr->set_function("asin",static_cast<float(*) (float)> (&asin));
    luaPtr->set_function("pow",static_cast<double(*) (double,double)> (&pow));
    luaPtr->set_function("ceil",static_cast<double(*) (double)> (&ceil));
    luaPtr->set_function("floor",static_cast<double(*) (double)> (&floor));
}
```
* if you use katana, maya, houdini with cartesian this functions all for your platform

* binding GLM::mat* glm::vec*
* binding CGAL::surfacemesh


### KatanaCartesianJIT
* the whole new lua function for katana
* Similar to Houdini vex function, BUT NOW IT WAS BASED LUAJIT

[!image](imgs/katana_01.png)


### Sandbox
* only for test 
* application based Cartesian


### Write your own application based the cartesian
* 1, see the Sandbox cmakeslist.txt
* 2, link the CartesianLuajit.lib and the dll should in env PATH 


### LUAJIT SPEED
* Table is highly recommended
* But if you like. You can also use my exposed UserData, as you know, luajit doesn't optimize it
* Even though you still want to use it, try to use all the C++ functions I exposed.

### Final
爱用不用
