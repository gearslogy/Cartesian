#pragma once

#include "sol.hpp"
#include "test_base.h"

class TEST_POINT_ATTRIB:public TEST_CASE{
public:

	static std::shared_ptr<TEST_POINT_ATTRIB> creator() {
		return std::make_shared<TEST_POINT_ATTRIB>();
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
