#include "test_point_get_attrib_values.h"



void TEST_GET_ATTRIB_VALUES::TEST(sol::state* lua)
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
	add_string_pointattrib(m,"foo_string", "default_string_value") -- add string
	add_int_pointattrib(m,"foo_int",1);             -- add int
	add_float_pointattrib(m,"foo_float",12.05);     -- add float
	add_table_pointattrib(m,"foo_table",{1,2,3} );  -- add table
	printAttribs()
	set_string_pointattrib(m,"foo_string", 0, "AlgebraMaster")
	set_int_pointattrib(m,"foo_int", 0, 10000)
	set_float_pointattrib(m,"foo_float", 0, 634.0123)
	set_table_pointattrib(m,"foo_table", 0, {1,2,3,4,5,"EvenString!"})
	local fvalues = get_float_pointattribvalues(m,"foo_float")
	local ivalues = get_int_pointattribvalues(m,"foo_int")
	local svalues = get_string_pointattribvalues(m,"foo_string")
	local tvalues = get_table_pointattribvalues(m,"foo_table")
	print("--------- For simply we just print the length for string/int/float/table attribute")
	print("fvalues length:", #fvalues)
	print("ivalues length:", #ivalues)
	print("svalues length:", #svalues)
	print("tvalues length:", #tvalues)
	print("tvalues first table{1,2,3,4,5,EvenString!} length should be five:", #tvalues[1])


	print("------------- add three vector attribute -----------------")
	add_vector2_pointattrib(m,"foo_vector2",set(0,0) )
	add_vector_pointattrib(m,"foo_vector",set(0,1,0) )
	add_vector4_pointattrib(m,"foo_vector4",set(0,0,0,1) )
	printAttribs()
	
	set_vector2_pointattrib(m, "foo_vector2", vertex.new(0), set(1,1) )
	set_vector_pointattrib(m, "foo_vector", 0, set(1,1,1) )
	set_vector4_pointattrib(m, "foo_vector4", 0, set(1,1,1,1) )

	print(get_vector2_pointattrib(m, "foo_vector2", vertex.new(0) ))
	print(get_vector_pointattrib(m, "foo_vector", 0 ))
	print(get_vector4_pointattrib(m, "foo_vector4", 0 ))


	local vec2_values = get_vector2_pointattribvalues(m,"foo_vector2")
	for k,v in ipairs(vec2_values) do 
		for i,t in ipairs(v) do
			io.write(t," " )
		end
		io.write("\n")
	end

	

	)");
	}
	catch (...) {

	}
}

