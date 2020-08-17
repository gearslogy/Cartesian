#include <iostream>
#include "CartesianPluginLoader.h"
#include "CartesianLog.h"
#include "test_point_attib.h"
#include "test_point_attrib_table.h"
#include "test_point_add_set_get.h"
#include "test_point_vec2_vec3_vec4.h"
#include "test_point_get_attrib_values.h"
#include "test_point_add_set_get_matrix.h"
#include "test_point_set_attrib_values.h"
void testVector(sol::state* lua) {


    try {
        lua->safe_script(
            R"(
print(sin(2))
print(cos(2))

p1 = vector.new(1,1,1)                              -- OOP create a vector3
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

print('------------length() --------------------')
print(length(set(0,2,0)) )
print(length({0,2,0}) )                                      -- length a table vector
print(length({0,2,0,3,4,5,6,7,8}) )                          -- length a table vector

print('------------distance()-------------------')
print(distance(set(0,0,2), set(0,3,0) ));
print(distance({0,0,1},{0,1,0} ));


)");
    }
    catch (...) {

    }


}


void testmatrix(sol::state* lua) {


    try {
        lua->safe_script(
            R"(

print("-----create mat2 mat3 mat4 -------")
local mat2 = matrix2.new(1)
local mat3 = matrix3.new(1)
local mat4 = matrix.new(1)
print(mat2)
print(mat3)
print(mat4)

print('---------after change col=0 row=0----------')
setmatrix2(mat2,0,0,1000)
setmatrix3(mat3,0,0,1000)
setmatrix(mat4,0,0,1000)

print("-----mat2-----")
print(mat2)
print("-----mat3-----")
print(mat3)
print("-----mat4-----")
print(mat4)


print("----Inverse Matrix------")
local mat2_inverse = invert(mat2)
local mat3_inverse = invert(mat3)
local mat4_inverse = invert(mat4)
print(mat2_inverse)
print(mat3_inverse)
print(mat4_inverse)


print("--------- Gen Matrix Use vector----------------")
print(set( set(1,0),    set(0,1) )    )                    -------- matrix2
print(set( set(1,0,0),  set(0,1,0),  set(0,0,1) )   )      -------- matrix3 set() is a overload function
print(set({1,0},{0,1}))                                    -------- matrix2 with lua table
print(set({1,0,0},{0,1,0},{0,0,1}) )                       -------- matrix3 with lua table


print("-----Transform Matrix only support matrix4--------")
local mat4 = matrix.new(1.0)
mat4 = transform(mat4,set(0,1,0))
print(mat4)

print("-----Scale Matrix only support matrix4--------")
local mat4 = matrix.new(1.0)
mat4 = scale(mat4,set(0.5,0.5,0.2))
print(mat4)


print("-----Rotate Matrix only support matrix4--------")
local col1 = set(1,0,0,0)
local col2 = set(0,1,0,0)
local col3 = set(0,0,1,0)
local col4 = set(0,0,0,1)
local mat4 = set(col1,col2,col3,col4)    --- matrix4  --- also can create mat4 with : matrix.new(1.0) or matrix:new(1.0)
local mat4_rot = rotate(mat4,radians(60),set(0,1,0) )
print(mat4_rot)

print("-----Rotate Matrix only support matrix4,but use table--------")
local mat4_rot = rotate({1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1},radians(60), {0,1,0})
print(mat4_rot)

print("--------------- astable() ------------------------")
local mat2 = set({1,0},{0,1})  ----if you write set(1,0,0,1), it will return vector4 type
print(mat2)
local mat2_astable = astable(mat2)              --- return lua table
print(mat2_astable[1],mat2_astable[2],mat2_astable[3],mat2_astable[4])

local mat3 = matrix3.new(1.0)
print(astable(mat3))
local mat4 = matrix.new(1.0)
print(astable(mat4))


)");
    }
    catch (...) {

    }


}


void testMatrixTable(sol::state* lua) {


    try {
        lua->safe_script(
            R"(
print(table_to_matrix2({1,1, 2,2}) )
print(table_to_matrix3({1,1,1, 2,2,2, 3,3,3}) )
print(table_to_matrix({1,1,1,1, 2,2,2,2, 3,3,3,3, 4,4,4,4}) )

print("----- table mat*mat ----")
local table_mat22 = t_matrix2_mul_matrix2({1,2, 3,4},{4,2, 3,1})  -- 22 table matrix * 22 table matrix
local mat22 = table_to_matrix2(table_mat22)                       --- convert to normal matrix2


local table_mat33 = t_matrix3_mul_matrix3({1,2,3, 1,21,31, 21,10,13},{12,1,33, 15,0.8,25, 10,5,7})                                            -- 33 table matrix * 33 table matrix
local mat33 = table_to_matrix3(table_mat33)          -- convert to normal matrix3

local table_mat44 = t_matrix_mul_matrix({1,2,3,4, 1,21,31,13, 123,21,10,13, 76,3,1,5},{12,1,33,56, 15,0.8,10,25, 32,1,323,16, 12,1,323,16})   -- 44 table matrix * 44 table matrix
local mat44 = table_to_matrix(table_mat44)                   --convert to normal matrix


print(mat22)
print(mat33)
print(mat44)


print("------ mat table * vec table -------------")
local vec4_table = t_matrix_mul_vector4({1,2,3,4, 1,21,31,13, 123,21,10,13, 76,3,1,5},{12,1,33,56, 15,0.8,10,25, 32,1,323,16, 12,1,323,16} , {1,2,3,4})    -- 44 table matrix * vec4 table; return table
local vec3_table = t_matrix3_mul_vector({1,2,3, 1,21,31, 21,10,13},{12,1,33, 15,0.8,25, 10,5,7},{1,2,3})                                                   -- 33 table matrix * vec3 table; return table
local vec2_table = t_matrix2_mul_vector2({1,2, 3,4}, {1,2})                                                                                                -- 22 table matrix * vec2 table; return table

local vec4 = table_to_vector4(vec4_table)    --- return vector4 type
local vec3 = table_to_vector(vec3_table)     --- return vector type
local vec2 = table_to_vector2(vec2_table)    --- return vector2 type

print(vec4)
print(vec3)
print(vec2)
)");
    }
    catch (...) {

    }


}



int main(int argc, char** argv)
{
    // create state
    //auto *lua = new sol::state;
    auto* lua = new sol::state;
    lua->open_libraries(sol::lib::base, sol::lib::jit, sol::lib::ffi, sol::lib::package, sol::lib::coroutine, sol::lib::io);

    Cartesian::Log::initialize();
    CARTESIAN_CORE_WARN("Starting version = {0}.{1}", 0, 1);

    auto functions = Cartesian::PluginLoader::loadPlugins();
    Cartesian::PluginLoader::dispatch(functions, lua);
    CARTESIAN_CORE_INFO("running script");

    // TEST

    /*
    CARTESIAN_CORE_INFO("RUNNING POINT ATTRIBUTE FLOAT/INT/STRING GLM TEST");
    TEST_RUN_CASE(TEST_POINT_ATTRIB_FLOAT_STRING_INT, lua);

    CARTESIAN_CORE_INFO("RUNNING ATTRIBUTE VALUES TEST");
    TEST_RUN_CASE(TEST_POINT_ATTRIB_VALUES, lua);

    CARTESIAN_CORE_INFO("RUNNING ATTRIBUTE GLM TEST");
    TEST_RUN_CASE(TEST_POINT_ATTRIB_GLM, lua);


    CARTESIAN_CORE_INFO("RUNNING TEST_POINT_ATTRIB_TABLE");
    TEST_RUN_CASE(TEST_POINT_ATTRIB_TABLE, lua);


    CARTESIAN_CORE_INFO("RUNNING ADD SET GET");
    TEST_RUN_CASE(TEST_POINT_ADD_SET_GET, lua);


    CARTESIAN_CORE_INFO("RUNNING vector vector2 vector3");
    TEST_RUN_CASE(TEST_POINT_VEC2_VEC3_VEC4, lua);
    */


    //CARTESIAN_CORE_INFO("RUNNING get_*_pointattribvalues ");
    //TEST_RUN_CASE(TEST_GET_ATTRIB_VALUES, lua);



    //CARTESIAN_CORE_INFO("RUNNING ADD GET SET FOR MATRIX  ");
    //TEST_RUN_CASE(TEST_ADD_SET_GET_MATRIX, lua);

    CARTESIAN_CORE_INFO("RUNNING SET ATTRIB VALUES");
    TEST_RUN_CASE(TEST_POINT_SET_ATTRIB_VALUES, lua);



    std::cin.get();

    return 0;
}
