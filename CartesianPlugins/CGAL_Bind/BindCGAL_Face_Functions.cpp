
#include "BindCGAL_Face_Functions.h"
#include "CGAL_Attrib_Helper.h"
namespace Cartesian {


	void BindCGAL_Face_Functions::bind(sol::state* lua)
	{

		auto npts = [](const PRE_TYPE::Mesh& m) {return m.num_faces(); };
		REGISTER_LUA_FUNCTION("nprims", npts);

		// ---------------------- Get all prim attribute names --------------------------------------
		auto getFaceAttribNames = [](const PRE_TYPE::Mesh& mesh) {
			std::vector<std::string> props = mesh.properties<PRE_TYPE::Face_descriptor>();
			return sol::as_table(props);
		};
		REGISTER_LUA_FUNCTION("primattribnames", getFaceAttribNames);


		// ---------------------- Check a prim attribute is exist --------------------------------------
		auto hasPrimAttrib = [](const PRE_TYPE::Mesh& mesh, const std::string& name)->bool {
			std::vector<std::string> props = mesh.properties<PRE_TYPE::Face_descriptor>();
			for (auto& p : props) {
				if (p == name) return true;
			}
			return false;
		};
		REGISTER_LUA_FUNCTION("hasprimattrib", hasPrimAttrib);



		// --------------------------------------------------------------  int attribute operation --------------------------------------------------------------------
		 // add attribute
		DEFINE_ADD_ATTRIB_FUNCTION(PRE_TYPE::Face_descriptor, add_int_primattrib, int);
		REGISTER_LUA_FUNCTION("add_int_primattrib", add_int_primattrib);

		// get attribute
		DEFINE_GET_ATTRIB_ID_FUNCTION(PRE_TYPE::Face_descriptor, get_int_primattrib_ptnum, int);
		DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(PRE_TYPE::Face_descriptor, get_int_primattrib, int);
		REGISTER_LUA_OVERLOAD_FUNCTION("get_int_primattrib", get_int_primattrib, get_int_primattrib_ptnum);

		// set attribute
		DEFINE_SET_ARRITB_PTNUM_FUNCTION(PRE_TYPE::Face_descriptor, set_int_primattrib_ptnum, int);
		DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(PRE_TYPE::Face_descriptor, set_int_primattrib, int);
		REGISTER_LUA_OVERLOAD_FUNCTION("set_int_primattrib", set_int_primattrib, set_int_primattrib_ptnum);

		// --------------------------------------------------------------  int attribute operation --------------------------------------------------------------------



		// --------------------------------------------------------------  string attribute operation --------------------------------------------------------------------
		 // add attribute
		DEFINE_ADD_ATTRIB_FUNCTION(PRE_TYPE::Face_descriptor, add_string_primattrib, std::string);
		REGISTER_LUA_FUNCTION("add_string_primattrib", add_string_primattrib);

		// get attribute
		DEFINE_GET_ATTRIB_ID_FUNCTION(PRE_TYPE::Face_descriptor, get_string_primattrib_ptnum, std::string);
		DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(PRE_TYPE::Face_descriptor, get_string_primattrib, std::string);
		REGISTER_LUA_OVERLOAD_FUNCTION("get_string_primattrib", get_string_primattrib, get_string_primattrib_ptnum);

		// set attribute
		DEFINE_SET_ARRITB_PTNUM_FUNCTION(PRE_TYPE::Face_descriptor, set_string_primattrib_ptnum, std::string);
		DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(PRE_TYPE::Face_descriptor, set_string_primattrib, std::string);
		REGISTER_LUA_OVERLOAD_FUNCTION("set_string_primattrib", set_string_primattrib, set_string_primattrib_ptnum);

		// --------------------------------------------------------------  string attribute operation --------------------------------------------------------------------




		// --------------------------------------------------------------  float attribute operation --------------------------------------------------------------------
		// add attribute
		DEFINE_ADD_ATTRIB_FUNCTION(PRE_TYPE::Face_descriptor, add_float_primattrib, float);
		REGISTER_LUA_FUNCTION("add_float_primattrib", add_float_primattrib);

		// get attribute
		DEFINE_GET_ATTRIB_ID_FUNCTION(PRE_TYPE::Face_descriptor, get_float_primattrib_ptnum, float);
		DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(PRE_TYPE::Face_descriptor, get_float_primattrib, float);
		REGISTER_LUA_OVERLOAD_FUNCTION("get_float_primattrib", get_float_primattrib, get_float_primattrib_ptnum);

		// set attribute
		DEFINE_SET_ARRITB_PTNUM_FUNCTION(PRE_TYPE::Face_descriptor, set_float_primattrib_ptnum, float);
		DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(PRE_TYPE::Face_descriptor, set_float_primattrib, float);
		REGISTER_LUA_OVERLOAD_FUNCTION("set_float_primattrib", set_float_primattrib, set_float_primattrib_ptnum);
		// --------------------------------------------------------------  float attribute operation --------------------------------------------------------------------


		 // -------------------------------------------------------------- lua table attribute operation --------------------------------------------------------------------
		// add attribute
		DEFINE_ADD_ATTRIB_FUNCTION(PRE_TYPE::Face_descriptor, add_table_primattrib, sol::lua_table);
		REGISTER_LUA_FUNCTION("add_table_primattrib", add_table_primattrib);

		// get attribute
		DEFINE_GET_ATTRIB_ID_FUNCTION(PRE_TYPE::Face_descriptor, get_table_primattrib_ptnum, sol::lua_table);
		DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(PRE_TYPE::Face_descriptor, get_table_primattrib, sol::lua_table);
		REGISTER_LUA_OVERLOAD_FUNCTION("get_table_primattrib", get_table_primattrib, get_table_primattrib_ptnum);

		// set attribute
		DEFINE_SET_ARRITB_PTNUM_FUNCTION(PRE_TYPE::Face_descriptor, set_table_primattrib_ptnum, sol::lua_table);
		DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(PRE_TYPE::Face_descriptor, set_table_primattrib, sol::lua_table);
		REGISTER_LUA_OVERLOAD_FUNCTION("set_table_primattrib", set_table_primattrib, set_table_primattrib_ptnum);
		// -------------------------------------------------------------- lua table attribute operation --------------------------------------------------------------------



		  // Register get tables values
		auto func = GetAttribValues<PRE_TYPE::Face_descriptor, float>::get;
		REGISTER_LUA_FUNCTION("get_float_primattribvalues", func);

		auto func2 = GetAttribValues<PRE_TYPE::Face_descriptor, int>::get;
		REGISTER_LUA_FUNCTION("get_int_primattribvalues", func2);

		auto func3 = GetAttribValues<PRE_TYPE::Face_descriptor, std::string>::get;
		REGISTER_LUA_FUNCTION("get_string_primattribvalues", func3);


		auto func4 = GetAttribValues<PRE_TYPE::Face_descriptor, glm::vec2>::get;
		REGISTER_LUA_FUNCTION("get_vector2_primattribvalues", func4);


		auto func5 = GetAttribValues<PRE_TYPE::Face_descriptor, glm::vec3>::get;
		REGISTER_LUA_FUNCTION("get_vector_primattribvalues", func5);

		auto func6 = GetAttribValues<PRE_TYPE::Face_descriptor, glm::vec4>::get;
		REGISTER_LUA_FUNCTION("get_vector4_primattribvalues", func6);



		auto func7 = GetTableAttribValues<PRE_TYPE::Face_descriptor, float>::get;
		REGISTER_LUA_FUNCTION("get_table_float_primattribvalues", func7);

		auto func8 = GetTableAttribValues<PRE_TYPE::Face_descriptor, int>::get;
		REGISTER_LUA_FUNCTION("get_table_int_primattribvalues", func8);

		auto func9 = GetTableAttribValues<PRE_TYPE::Face_descriptor, std::string>::get;
		REGISTER_LUA_FUNCTION("get_table_string_primattribvalues", func9);


	}

}

