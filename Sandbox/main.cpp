#include <iostream>
#include "CartesianPluginLoader.h"



void testVector(sol::state * lua){


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
print("-------------- cross ------------------")
print(cross(   {1,0,0},      {0,1,0})   )            -- cross(table,    table)
print(cross(   set(1,0,0),   {0,1,0})   )            -- cross(vector,   table)
print(cross(   vector.new(1,0,0),   {0,1,0})   )     -- cross(vector,   table)
print(cross(   {1,0,0}  ,set(0,1,0) ))               -- cross(table,   vector)

print('--------- normalize() ----------------')
print(normalize(1,2,0) )                             -- normalize(float,float,float) return vector
print(normalize({1,2,0}) )                           -- normalize(table) return vector
print(normalize(vector2.new(1,2)) )                  -- normalize(vector2) return vector2
print(normalize(vector.new(1,2,1)) )                 -- normalize(vector)  return vector
print(normalize(set(1,2,1) ) )                       -- normalize(vector)  return vector

)");
    }
    catch (...) {

    }


}



int main(int argc , char **argv)
{
    // create state
    //auto *lua = new sol::state;
    auto *lua = new sol::state;
    lua->open_libraries(sol::lib::base,sol::lib::jit,sol::lib::ffi,sol::lib::package,sol::lib::coroutine);



    auto functions = Cartesian::PluginLoader::loadPlugins();
    Cartesian::PluginLoader::dispatch(functions,lua);
    std::cout << "CARTESIAN::Run main example script\n";
    testVector(lua);

    return 0;
}
