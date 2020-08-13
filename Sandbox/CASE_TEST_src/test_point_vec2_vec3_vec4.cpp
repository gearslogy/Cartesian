#include "test_point_vec2_vec3_vec4.h"
void TEST_POINT_VEC2_VEC3_VEC4::TEST(sol::state* lua)
{

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
	add_vector2_pointattrib(m,"foo_vector2",set(0,0) )
	add_vector_pointattrib(m,"foo_vector",set(0,1,0) )
	add_vector4_pointattrib(m,"foo_vector4",set(0,0,0,1) )
	printAttribs()
	
	set_vector2_pointattrib(m, "foo_vector2", vertex.new(0), set(1,1) )
	set_vector_pointattrib(m, "foo_vector", 0, set(1,1,1) )
	set_vector4_pointattrib(m, "foo_vector4", 0, set(1,1,1,1) )


	)");
	}
	catch (...) {

	}

}

