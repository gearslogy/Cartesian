#include "test_prim_add_set_get.h"


void TEST_RPIM_ADD_SET_GET::TEST(sol::state* lua)
{
    try {
        lua->safe_script(
            R"(
	local m = mesh.new();
	local pt0 = addpoint(m, {0,0,0} );
	local pt1 = addpoint(m, {1,0,0} );
	local pt2 = addpoint(m, {1,1,0} );
	local pt3 = addpoint(m, {0,1,0} );
	-- Two triangles
	addface(m,pt0,pt1,pt3)   -- 0 1 3
	addface(m,pt1,pt2,pt3)   -- 1 2 3

	print("current mesh have points num:", npoints(m) )
	printAttribs = function()
		local pnt_attrib_names = pointattribnames(m)
		for k,v in ipairs(pnt_attrib_names) do
			print(v)
		end
	end
	


	)");
    }
    catch (...) {

    }
}

