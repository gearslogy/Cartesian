#pragma once

#include "sol.hpp"
#include "test_base.h"

class TEST_POINT_ATTRIB_FLOAT_STRING_INT:public TEST_CASE{
public:

	static std::shared_ptr<TEST_POINT_ATTRIB_FLOAT_STRING_INT> creator() {
		return std::make_shared<TEST_POINT_ATTRIB_FLOAT_STRING_INT>();
	}

	void TEST(sol::state* lua) {

		try {
			lua->safe_script(
				R"(
local m = mesh.new();
local pt0 = addpoint(m, {0,0,0} );
local pt1 = addpoint(m, {1,0,0} );
local pt2 = addpoint(m, {1,1,0} );
addface(m,pt0,pt1,pt2)

print("current mesh have points num:", npoints(m) )

print("------- Debug current attrib names -------------")
local pnt_attrib_names = pointattribnames(m)
for k,v in ipairs(pnt_attrib_names) do
    print(v)
end
print("------- now add float attrib:foo  -------------")
add_float_pointattrib(m,"foo", 0.0)

print("------- after add attrib:foo -----------")
local pnt_attrib_names = pointattribnames(m)
for k,v in ipairs(pnt_attrib_names) do
    print(v)
end

print("------- change id=0 point attrib value -----------");
set_float_pointattrib(m,"foo",0,100.0); 

for k=1,npoints(m) do
    local id = k-1
    local vd = vertex.new(id)
    ---local foo_att_value = get_float_pointattrib(m,"foo",vd)  // Or use this overloaded function
    local foo_att_value = get_float_pointattrib(m,"foo",id)  
    print("id:",k, "foo:",foo_att_value );
end

print("--------------- Test The string attribute ------------------")

-- simple define print all attrib names func

printAttribs = function()
	local pnt_attrib_names = pointattribnames(m)
	for k,v in ipairs(pnt_attrib_names) do
		print(v)
	end
end


add_string_pointattrib(m,"foo", "default")
add_string_pointattrib(m,"foo", "default")
add_string_pointattrib(m,"foo", "default")
add_string_pointattrib(m,"foo", "default")
printAttribs()



)");
		}
		catch (...) {

		}
	}


};

class TEST_POINT_ATTRIB_VALUES :public TEST_CASE {
public:

	static std::shared_ptr<TEST_POINT_ATTRIB_VALUES> creator() {
		return std::make_shared<TEST_POINT_ATTRIB_VALUES>();
	}

	void TEST(sol::state* lua) {

		try {
			lua->safe_script(
				R"(
local m = mesh.new();
local pt0 = addpoint(m, {0,0,0} );
local pt1 = addpoint(m, {1,0,0} );
local pt2 = addpoint(m, {1,1,0} );
addface(m,pt0,pt1,pt2)

print("current mesh have points num:", npoints(m) )

print("current mesh have attib names:")
printAttribs = function()
	local pnt_attrib_names = pointattribnames(m)
	for k,v in ipairs(pnt_attrib_names) do
		print(v)
	end
end
printAttribs()

-------- add a float attribute------------------
add_float_pointattrib(m,"foo", 0.0)
set_float_pointattrib(m,"foo",0,100.0); 
local getfoovalues = get_float_pointattribvalues(m,"foo");  -- It's a table

for k=1,#getfoovalues do
	print(getfoovalues[k])
end

-------- get a exist attribute, but the type was not correct, so cannot get this attrib------------------
print("----------get not exist type attribute-----------------")
local getfoovalues = get_string_pointattribvalues(m,"foo");
print(#getfoovalues)

-------- add a string attribute and debug all points values------------------
print("----------------add a string attrib, name: foo------------------")
add_string_pointattrib(m,"foo", "default")      -- add the "foo string attrib", default value:"default"
set_string_pointattrib(m,"foo", 0 ,"Master");   -- change first vertex value = "Master"
local getfoovalues = get_string_pointattribvalues(m,"foo");

for k=1,#getfoovalues do
	print(getfoovalues[k])
end
-------- add a int attribute and debug all points values------------------
print("---------------add a int attrib, name: foo-----------------")
add_int_pointattrib(m,"foo", 100)      -- add the "foo int attrib", default value: 100
set_int_pointattrib(m,"foo", 0 ,1234);   -- change first vertex value = 1234
local getfoovalues = get_int_pointattribvalues(m,"foo");

for k=1,#getfoovalues do
	print(getfoovalues[k])
end





)");
		}
		catch (...) {

		}
	}
};


class TEST_POINT_ATTRIB_GLM :public TEST_CASE {
public:

	static std::shared_ptr<TEST_POINT_ATTRIB_GLM> creator() {
		return std::make_shared<TEST_POINT_ATTRIB_GLM>();
	}

	void TEST(sol::state* lua) {

		try {
			lua->safe_script(
				R"(
local m = mesh.new();
local pt0 = addpoint(m, {0,0,0} );
local pt1 = addpoint(m, {1,0,0} );
local pt2 = addpoint(m, {1,1,0} );
addface(m,pt0,pt1,pt2)

print("current mesh have points num:", npoints(m) )

printAttribs = function()
	local pnt_attrib_names = pointattribnames(m)
	for k,v in ipairs(pnt_attrib_names) do
		print(v)
	end
end

printAttribs()



)");
		}
		catch (...) {

		}
	}
};


