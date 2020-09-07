#include "PyStringBind.h"


namespace Cartesian {
    void BindPystring(sol::state* lua)
    {
        auto pystring_split = [](const std::string& str, const std::string& spr) {
            std::vector<std::string> ret;
            pystring::split(str, ret, spr);
            return sol::as_table(ret);
        };
        (*lua)["pystring"] = (*lua).create_table_with("split", pystring_split);
    }
}
