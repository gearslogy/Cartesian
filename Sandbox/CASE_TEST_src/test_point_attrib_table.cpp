#include "test_base.h"
#include "test_point_attrib_table.h"


void TEST_POINT_ATTRIB_TABLE::TEST(sol::state* lua)
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

print("current mesh have attib names:")
printAttribs = function()
	local pnt_attrib_names = pointattribnames(m)
	for k,v in ipairs(pnt_attrib_names) do
		print(v)
	end
end
printAttribs()

print("------------ add table attribute:foo ---------------------")
add_table_pointattrib(m,"foo",{1,2,3,4})
printAttribs()
set_table_pointattrib(m, "foo", 0 , {1,2,3,"even string!"} ) --- change vertex id table value

for k = 1,npoints(m) do 
	print("current look up the vertex id data: ", k-1)
	local gettable_values = get_table_pointattrib(m,"foo",k-1);
	for i = 1, #gettable_values do 
		print(gettable_values[i])
	end
end


print("------------------all not flat tables --------------------")
local get_vertex_tables = get_table_pointattribvalues(m,"foo")
print(get_vertex_tables)
for k,v in ipairs(get_vertex_tables) do
	io.write(k,":")
	for i,val in ipairs(v) do
		io.write(val," ")
	end
	io.write("\n")
end
)");
	}
	catch (...) {

	}

}
