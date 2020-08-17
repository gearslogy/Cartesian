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
	add_matrix2_pointattrib(m,"foo_matrix2", ident2() ) 
	add_matrix3_pointattrib(m,"foo_matrix3",ident3() )      
	add_matrix_pointattrib(m,"foo_matrix", ident())  
	printAttribs()
	set_matrix2_pointattrib(m,"foo_matrix2",0, ident2()*2 ) 
	set_matrix3_pointattrib(m,"foo_matrix3",0,ident3()*3 )      
	set_matrix_pointattrib(m,"foo_matrix",0, ident()*4)  
	
	local mat2_id0 = get_matrix2_pointattrib(m,"foo_matrix2",0 ) 
	local mat3_id0 = get_matrix3_pointattrib(m,"foo_matrix3",0 )      
	local mat4_id0 = get_matrix_pointattrib(m,"foo_matrix",0)  
	print(mat2_id0	)
	print(mat3_id0	)
	print(mat4_id0	)
	
	local allmat2 = get_matrix2_pointattribvalues(m,"foo_matrix2") 
	local allmat3 = get_matrix3_pointattribvalues(m,"foo_matrix3") 
	local allmat4 = get_matrix_pointattribvalues(m,"foo_matrix") 

	print("--------iterator of get_matrix*_pointattribvalues----------")
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

