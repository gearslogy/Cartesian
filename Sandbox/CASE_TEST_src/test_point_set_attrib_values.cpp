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
	)");
    }
    catch (...) {

    }
}

