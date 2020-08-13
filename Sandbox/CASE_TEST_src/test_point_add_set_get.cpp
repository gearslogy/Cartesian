#pragma once

#include "test_point_add_set_get.h"

void TEST_POINT_ADD_SET_GET::TEST(sol::state* lua) 
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

	print("------------- set first point value -----------------")
	set_string_pointattrib(m,"foo_string", 0, "AlgebraMaster")
	set_int_pointattrib(m,"foo_int", 0, 10000)
	set_float_pointattrib(m,"foo_float", 0, 634.0123)
	set_table_pointattrib(m,"foo_table", 0, {1,2,3,4,5,"EvenString!"})
	
	print("------------- debug all that use get*function -----------------")
	for k = 1, npoints(m) do
		local pointid = k - 1
		local value = get_string_pointattrib(m,"foo_string", pointid )
		io.write("id:",pointid," value: ")
		io.write( value,"\t") 
	end
	io.write( "\n") 
	for k = 1, npoints(m) do
		local pointid = k - 1
		local value = get_int_pointattrib(m,"foo_int", pointid )
		io.write("id:",pointid," value: ")
		io.write( value,"\t") 
	end
	io.write( "\n") 
	for k = 1, npoints(m) do
		local pointid = k - 1
		local value = get_float_pointattrib(m,"foo_float", pointid )
		io.write("id:",pointid," value: ")
		io.write( value,"\t") 
	end
	io.write( "\n") 

	for k = 1, npoints(m) do
		local pointid = k - 1
		local value = get_table_pointattrib(m,"foo_table", pointid )
		io.write("id:",pointid, " value: ")

		for i,v in ipairs(value) do 
			io.write(v," ")
		end
		io.write("\t") 
	end
	io.write( "\n") 

	)");
	}
	catch (...) {

	}

}
