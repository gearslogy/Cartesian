#include "BindKatanaCH.h"
#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Cartesian {


    void BindKatanaCH::bind(Foundry::Katana::GeolibCookInterface& Geolibface, sol::state *lua)
    {
        // ---------------------------- chf()---------------------------------------------------------
        auto chf = [&Geolibface](const std::string& chanName) -> float {
            FnAttribute::GroupAttribute userGroupAttr = Geolibface.getOpArg("user");
            if (userGroupAttr.isValid()) {
                FnAttribute::FloatAttribute chAttr = userGroupAttr.getChildByName(chanName);
                if (chAttr.isValid()) {
                    return chAttr.getValue();
                }
                std::cout << "[KATANA::CARTESIAN]: can not get user." << chanName << std::endl;
            }
            else
                std::cout << "[KATANA::CARTESIAN]: get param must in user group attrib\n";
            return 0;
        };
        lua->set_function("chf", chf);


        // ---------------------------- chs()---------------------------------------------------------
        auto chs = [&Geolibface](const std::string& chanName) -> std::string {
            FnAttribute::GroupAttribute userGroupAttr = Geolibface.getOpArg("user");
            if (userGroupAttr.isValid()) {
                FnAttribute::StringAttribute chAttr = userGroupAttr.getChildByName(chanName);
                if (chAttr.isValid()) {
                    return chAttr.getValue();
                }
                std::cout << "[KATANA::CARTESIAN]: can not get user." << chanName << std::endl;
            }
            else
                std::cout << "[KATANA::CARTESIAN]: get param must in user group attrib\n";
            return "";
        };
        lua->set_function("chs", chs);

        
        // ---------------------------- chcolor()---------------------------------------------------------
        auto chcolor = [&Geolibface](const std::string& chanName)
        {
            glm::vec3 ret(0.0); 
            FnAttribute::GroupAttribute userGroupAttr = Geolibface.getOpArg("user");
            if (userGroupAttr.isValid()) {
                FnAttribute::FloatAttribute  chfAttr = userGroupAttr.getChildByName(chanName);
                if (!chfAttr.isValid()) {
                    CARTESIAN_CORE_ERROR("can not find user arg :{0}", chanName);
                    return ret;
                }
                
                auto samples = chfAttr.getNearestSample(Foundry::Katana::GetCurrentTime(Geolibface));
                if (samples.size() != 3)
                {
                    CARTESIAN_CORE_ERROR("error get color param value size == 3 ? :{0}", chanName);
                    return ret;
                }
                ret.x = samples[0];
                ret.y = samples[1];
                ret.z = samples[2];
            }
            return ret;
        };

        lua->set_function("chcolor", chcolor);

        auto chcolor4 = [&Geolibface](const std::string& chanName)
        {
            glm::vec4 ret(0.0);
            FnAttribute::GroupAttribute userGroupAttr = Geolibface.getOpArg("user");
            if (userGroupAttr.isValid()) {
                FnAttribute::FloatAttribute  chfAttr = userGroupAttr.getChildByName(chanName);
                if (!chfAttr.isValid()) {
                    CARTESIAN_CORE_ERROR("can not find user arg :{0}", chanName);
                    return ret;
                }
                auto samples = chfAttr.getNearestSample(Foundry::Katana::GetCurrentTime(Geolibface));
                if (samples.size() != 4) {
                    CARTESIAN_CORE_ERROR("error get color4 param value size == 4 ? :{0}", chanName);
                    return ret;
                }
                ret.x = samples[0];
                ret.y = samples[1];
                ret.z = samples[2];
                ret.w = samples[3];
            }
            return ret;
        };

        lua->set_function("chcolor4", chcolor4);

        // chf array
        auto chfarray = [&Geolibface](const std::string& chanName)
        {
            std::vector<float> values;
            FnAttribute::GroupAttribute userGroupAttr = Geolibface.getOpArg("user");
            if (userGroupAttr.isValid()) {
                FnAttribute::FloatAttribute  chfAttr = userGroupAttr.getChildByName(chanName);
                if (!chfAttr.isValid()) {
                    CARTESIAN_CORE_ERROR("can not find user arg :{0}", chanName);
                    return sol::as_table(values);
                }
                auto samples = chfAttr.getNearestSample(Foundry::Katana::GetCurrentTime(Geolibface));
                for (auto& val : samples) {
                    values.emplace_back(val);
                }
            }
            return sol::as_table(values);;
        };
        lua->set_function("chfarray", chfarray);


        // chsarray
        auto chsarray = [&Geolibface](const std::string& chanName)
        {
            std::vector<std::string> values;
            FnAttribute::GroupAttribute userGroupAttr = Geolibface.getOpArg("user");
            if (userGroupAttr.isValid()) {
                FnAttribute::StringAttribute  chfAttr = userGroupAttr.getChildByName(chanName);
                if (!chfAttr.isValid()) {
                    CARTESIAN_CORE_ERROR("can not find user arg :{0}", chanName);
                    return sol::as_table(values);
                }
                auto samples = chfAttr.getNearestSample(Foundry::Katana::GetCurrentTime(Geolibface));
                for (auto& val : samples) {
                    values.emplace_back(val);
                }
            }
            return sol::as_table(values);;
        };

        lua->set_function("chsarray", chsarray);


        auto chv = [&Geolibface](const std::string& chanName)
        {
            glm::vec4 ret(0.0);
            FnAttribute::GroupAttribute userGroupAttr = Geolibface.getOpArg("user");
            if (userGroupAttr.isValid()) {
                FnAttribute::FloatAttribute  chfAttr = userGroupAttr.getChildByName(chanName);
                if (!chfAttr.isValid()) {
                    CARTESIAN_CORE_ERROR("can not find user arg :{0}", chanName);
                    return ret;
                }
                auto samples = chfAttr.getNearestSample(Foundry::Katana::GetCurrentTime(Geolibface));
                if (samples.size() != 3) {
                    CARTESIAN_CORE_ERROR("error get vector param value size == 3 ? :{0}", chanName);
                    return ret;
                }
                ret.x = samples[0];
                ret.y = samples[1];
                ret.z = samples[2];
            }
            return ret;
        };

        lua->set_function("chv", chv);

    } // end of bind function

}// end of namespace