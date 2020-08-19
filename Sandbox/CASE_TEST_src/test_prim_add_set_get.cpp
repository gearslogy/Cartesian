#include "test_prim_add_set_get.h"
#include "CartesianLog.h"

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
	print("current mesh have prim num:", nprims(m) )
	printAttribs = function()
		local pnt_attrib_names = primattribnames(m)
		for k,v in ipairs(pnt_attrib_names) do
			print(v)
		end
	end
	add_float_primattrib(m,"foo_float",1.012)
	add_string_primattrib(m,"foo_string", "default_str")
	add_int_primattrib(m,"foo_int",1)
	add_table_primattrib(m,"foo_table",{1,2,3,4})

	add_vector2_primattrib(m,"foo_vector2",set(0,0) )
	add_vector_primattrib(m,"foo_vector", set(0,0,0) )
	add_vector4_primattrib(m,"foo_vector4",set(1,0,0,0) )

	add_matrix2_primattrib(m,"foo_mat2",ident2() )
	add_matrix3_primattrib(m,"foo_mat3", ident3() )
	add_matrix_primattrib(m,"foo_mat4",ident() )
	printAttribs()
	
	print("------------ Test set attribute on prim id : 1----------")
	set_float_primattrib(m,"foo_float", 1 , 1.012)
	set_string_primattrib(m,"foo_string", 1 , "default_str")
	set_int_primattrib(m,"foo_int",1,2)
	set_table_primattrib(m, "foo_table", 1, {3,1,2} ) 

	set_vector2_primattrib(m,"foo_vector2",1, set(1,0) )
	set_vector_primattrib(m,"foo_vector",1, set(1,1,1) )
	set_vector4_primattrib(m,"foo_vector4",1, set(1,132,232,0) )

	set_matrix2_primattrib(m,"foo_mat2",1, ident2()*5 )
	set_matrix3_primattrib(m,"foo_mat3",1, ident3()*100 )
	set_matrix_primattrib(m,"foo_mat4",1,  ident()*500 )

	print("------------ Test get attribute on prim----------")
	-- Get prim id = 1 data
	print ( get_float_primattrib(m,"foo_float", 1 ) )
	print ( get_string_primattrib(m,"foo_string", 1 ) )
	print ( get_int_primattrib(m,"foo_int",1) )
	print ( get_table_primattrib(m, "foo_table", 1) )

	print(get_vector2_primattrib(m,"foo_vector2",1 ) )
	print(get_vector_primattrib(m,"foo_vector",1))
	print(get_vector4_primattrib(m,"foo_vector4",1) )

	print(get_matrix2_primattrib(m,"foo_mat2",1) )
	print(get_matrix3_primattrib(m,"foo_mat3",1) )
	print(get_matrix_primattrib(m,"foo_mat4",1 ))

	print("------------------ Get Values just check return type is table ----------------- ")
	print(get_float_primattribvalues(m,"foo_float"))
	print(get_int_primattribvalues(m,"foo_int"))
	print(get_string_primattribvalues(m,"foo_string"))
	print(get_table_primattribvalues(m,"foo_table"))

	print(get_vector2_primattribvalues(m,"foo_vector2"))
	print(get_vector_primattribvalues(m,"foo_vector"))
	print(get_vector4_primattribvalues(m,"foo_vector4"))

	print(get_matrix2_primattribvalues(m,"foo_mat2"))
	print(get_matrix3_primattribvalues(m,"foo_mat3"))
	print(get_matrix_primattribvalues(m,"foo_mat4"))
	
	print("------------------ set Values  ----------------- ")
	set_float_primattribvalues(m,"foo_float", {1.03,1.04})
	set_int_primattribvalues(m,"foo_int", {1,2})
	set_string_primattribvalues(m,"foo_string",{"hhh","ggg"})
	set_table_primattribvalues(m,"foo_table",{{"ggg","fff"},{1,2}})
	set_float_primattribvalues(m,"foo_float", {1.03,1.04})

	set_vector2_primattribvalues(m,"foo_vector2", { {1,2},{2,3} } )
	set_vector_primattribvalues(m,"foo_vector",{ {1,2,3},{1,2,3} })
	set_vector4_primattribvalues(m,"foo_vector4",{ {1,2,3,4},{1,2,3,4} })

	set_matrix2_primattribvalues(m,"foo_mat2", { astable(ident2()),astable(ident2()) } )
	set_matrix3_primattribvalues(m,"foo_mat3",{ astable(ident3()) , astable(ident3()) })
	set_matrix_primattribvalues(m,"foo_mat4",{ astable(ident()), astable(ident())   })
	
	

	)");
    }
    catch (std::runtime_error &e) {
		CARTESIAN_CORE_ERROR("Runtime Core Error");
    }
}

