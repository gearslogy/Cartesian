#include "test_point_set_attrib_values.h"


void TEST_POINT_SET_ATTRIB_VALUES::TEST(sol::state* lua)
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
	print("------------- add four attribute -----------------")
	add_string_pointattrib(m,"foo_string", "default_string_value") -- add string
	add_int_pointattrib(m,"foo_int",1);             -- add int
	add_float_pointattrib(m,"foo_float",12.05);     -- add float
	add_table_pointattrib(m,"foo_table",{1,2,3} );  -- add table
	printAttribs()

	set_float_pointattribvalues(m,"foo_float",{1.1,2.2,3.3} )
	set_int_pointattribvalues(m,"foo_int",{1,2,3} )
	set_string_pointattribvalues(m,"foo_string",{"bar","foo","nice"} )
	set_table_pointattribvalues(m,"foo_table",{  {4,5,6},   {"holly","day"},  {3.1415926, 2.0,"good"} } )
	print("------------ debug the attribute values" ) 
	print("debug float:")
	local temp = get_float_pointattribvalues(m,"foo_float")
	for i,v in ipairs(temp) do
		io.write(v, " ")
	end	
	io.write("\n\n")


	print("debug int:")
	local temp = get_int_pointattribvalues(m,"foo_int")
	for i,v in ipairs(temp) do
		io.write(v, " ")
	end	
	io.write("\n\n")

	print("debug string:")
	local temp = get_string_pointattribvalues(m,"foo_string")
	for i,v in ipairs(temp) do
		io.write(v, " ")
	end	
	io.write("\n\n")

	print("debug table:")
	local temp = get_table_pointattribvalues(m,"foo_table")
	for i,v in ipairs(temp) do
		io.write("{ ")
		for m,t in ipairs(v) do
			io.write(t," ")
		end
		io.write("}\n")
	end	
	io.write("\n\n")


	print("------------- add some vec2 vec3 vec4 attribute -----------------")
	add_vector2_pointattrib(m,"foo_vec2", set(0,0) ) -- add vec2
	add_vector_pointattrib(m,"foo_vec3",set(0,1,0) )             -- add vec3
	add_vector4_pointattrib(m,"foo_vec4",set(0,0,0,0) )     -- add vec4
	printAttribs()
	set_vector2_pointattribvalues(m,"foo_vec2", { {1,2},{1,2},{1,2} })	
	set_vector_pointattribvalues(m,"foo_vec3", { {1,2,3},{3,4,5},{6,7,8} })	
	set_vector4_pointattribvalues(m,"foo_vec4", { {1,2,3,4},{1,2,3,4},{1,2,3,4} })	
	
	print("debug vec2:")
	local temp = get_vector2_pointattribvalues(m,"foo_vec2")
	for i,v in ipairs(temp) do
		io.write("{ ")
		for m,t in ipairs(v) do
			io.write(t," ")
		end
		io.write("}\n")
	end	
	io.write("\n\n")
	
	print("debug vec3:")
	local temp = get_vector_pointattribvalues(m,"foo_vec3")
	for i,v in ipairs(temp) do
		io.write("{ ")
		for m,t in ipairs(v) do
			io.write(t," ")
		end
		io.write("}\n")
	end	
	io.write("\n\n")

	print("debug vec4:")
	local temp = get_vector4_pointattribvalues(m,"foo_vec4")
	for i,v in ipairs(temp) do
		io.write("{ ")
		for m,t in ipairs(v) do
			io.write(t," ")
		end
		io.write("}\n")
	end	
	io.write("\n\n")
	print("------------- add some mat2 mat3 mat4 attribute -----------------")
	add_matrix2_pointattrib(m,"foo_matrix2", ident2() ) 
	add_matrix3_pointattrib(m,"foo_matrix3",ident3() )      
	add_matrix_pointattrib(m,"foo_matrix", ident())  
	printAttribs()
	------------------------------------ set matrix attribute values
	local mat2_pt0 = astable( ident2() * 100 )
	local mat2_pt1 = astable( ident2() * 50 )
	local mat2_pt2 = astable( ident2() * 10 )
	set_matrix2_pointattribvalues(m, "foo_matrix2", {  mat2_pt0, mat2_pt1, mat2_pt2    }   )
	
	local mat3_pt0 = astable( ident3() * 100 )
	local mat3_pt1 = astable( ident3() * 50 )
	local mat3_pt2 = astable( ident3() * 10 )
	set_matrix3_pointattribvalues(m, "foo_matrix3", {  mat3_pt0, mat3_pt1, mat3_pt2    }   )
	
	local mat4_pt0 = astable( ident() * 100 )
	local mat4_pt1 = astable( ident() * 50 )
	local mat4_pt2 = astable( ident() * 10 )
	set_matrix4_pointattribvalues(m, "foo_matrix", {  mat4_pt0, mat4_pt1, mat4_pt2    }   )
	--------------------------- get matrix attribute values
	
	print("--------iterator of get_matrix*_pointattribvalues----------")
	local allmat2 = get_matrix2_pointattribvalues(m,"foo_matrix2") 
	local allmat3 = get_matrix3_pointattribvalues(m,"foo_matrix3") 
	local allmat4 = get_matrix_pointattribvalues(m,"foo_matrix") 
	print("mat2:")
	for k,v in ipairs(allmat2) do
		io.write("id=",k," :")
		for n,a in ipairs(v) do
			io.write(a, " ")
		end
		io.write("\n")
	end
	io.write("\n")
	
	print("mat3:")
	for k,v in ipairs(allmat3) do
		io.write("id=",k," :")
		for n,a in ipairs(v) do
			io.write(a, " ")
		end
		io.write("\n")
	end
	io.write("\n")

	print("mat4:")
	for k,v in ipairs(allmat4) do
		io.write("id=",k," :")
		for n,a in ipairs(v) do
			io.write(a, " ")
		end
		io.write("\n")
	end
	io.write("\n")


	)");
    }
    catch (...) {

    }
}

