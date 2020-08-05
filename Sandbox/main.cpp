#include <iostream>
#include "CartesianPluginLoader.h"


int main(int argc , char **argv)
{
    // create state
    //auto *lua = new sol::state;
    auto *lua = new sol::state;
    lua->open_libraries(sol::lib::base,sol::lib::jit,sol::lib::ffi,sol::lib::package,sol::lib::coroutine);



    auto functions = Cartesian::PluginLoader::loadPlugins();
    Cartesian::PluginLoader::dispatch(functions,lua);
    std::cout << "CARTESIAN::Run main example script\n";


    try {
        lua->safe_script(
                R"(
print(sin(2))
print(cos(2))

p1 = vector.new(1,1,1)
print("p1 lua:",p1.x, p1.y, p1.z)
p2 = vector.new(1,1,1)
p3 = p1 + p2;
p3 = p3 + 1;
print("p3 lua:",p3.x, p3.y, p3.z)
print(p3)
p4 = set(1,0,1)
print(p4)
)");
    }
    catch (...) {

    }

    return 0;
}
