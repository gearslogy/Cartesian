#include "test_point_add_set_get_matrix.h"

void TEST_ADD_SET_GET_MATRIX::TEST(sol::state* lua)
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
	print("------------- add four string/int/float/table attribute -----------------")
	add_matrix2_pointattrib(m,"foo_matrix2", ") 
	add_matrix_pointattrib(m,"foo_matrix",1);            
	add_matrix4_pointattrib(m,"foo_matrix4",12.05);   

	)");
	}
	catch (...) {

	}
}

