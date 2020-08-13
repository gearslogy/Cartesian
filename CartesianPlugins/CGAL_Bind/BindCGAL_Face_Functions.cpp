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




	}

}

