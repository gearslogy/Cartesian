#pragma once

namespace Cartesian {


    class GetFloatTuple {
    public:
        static void bind(Foundry::Katana::GeolibCookInterface& iface, sol::state * lua) {


            // Direct Get Attribute of current node from katana, only return table
            auto getPointFloatTuple = [&iface](const std::string& attname) {
                auto time = Foundry::Katana::GetCurrentTime(iface);
                std::string PAttName = "geometry.point.P";
                std::string arbitraryAttName = "geometry.arbitrary";
                std::string valueAttribName = arbitraryAttName;
                valueAttribName += ".";
                valueAttribName += attname;
                valueAttribName += ".";
                valueAttribName += "value";

                std::vector<float> values;
                if (attname == "P") {
                    FnAttribute::FloatAttribute posAttr = iface.getAttr(PAttName);
                    if (posAttr.isValid())
                    {
                        auto samples = posAttr.getNearestSample(time);
                        for (auto& val : samples)
                            values.emplace_back(val)
                            ;
                        return sol::as_table(values);
                    }
                }
                FnAttribute::FloatAttribute valueAttr = iface.getAttr(valueAttribName);


                if (valueAttr.isValid()) {
                    auto samples = valueAttr.getNearestSample(time);
                    for (auto& val : samples) values.emplace_back(val);

                }
                else {
                    CARTESIAN_CORE_ERROR("query attribute no (value) attribute: {0}", valueAttribName);;
                }
                return sol::as_table(values);
            };

            auto getPointFloatTuple2 = [&iface](const std::string& attname, const std::string& inputLocation, const int inputIndex) {
                auto time = Foundry::Katana::GetCurrentTime(iface);
                std::string arbitraryAttName = "geometry.arbitrary";
                std::string valueAttribName = arbitraryAttName;
                std::string PAttName = "geometry.point.P";
                valueAttribName += ".";
                valueAttribName += attname;
                valueAttribName += ".";
                valueAttribName += "value";


                try {
                    if (attname == "P")
                        iface.getAttr(PAttName, inputLocation, inputIndex);
                    iface.getAttr(valueAttribName, inputLocation, inputIndex);
                }
                catch (...) {
                    CARTESIAN_CORE_WARN("just check getfloattuple() , ignore this error: runtime_error");
                    throw GetAttribException();
                }

                std::vector<float> values;
                if (attname == "P") {
                    FnAttribute::FloatAttribute posAttr = iface.getAttr(PAttName, inputLocation, inputIndex);
                    if (posAttr.isValid()) {
                        auto samples = posAttr.getNearestSample(time);
                        for (auto& val : samples) values.emplace_back(val);
                        return sol::as_table(values);
                    }
                }


                FnAttribute::FloatAttribute valueAttr = iface.getAttr(valueAttribName, inputLocation, inputIndex);
                if (valueAttr.isValid()) {
                    auto samples = valueAttr.getNearestSample(time);
                    for (auto& val : samples) values.emplace_back(val);

                }
                else {
                    CARTESIAN_CORE_ERROR("query attribute no (value) attribute: {0}", valueAttribName);;
                }
                return sol::as_table(values);
            };

            lua->set_function("getfloattuple", sol::overload(getPointFloatTuple, getPointFloatTuple2));

        }

    };




    class GetIntTuple {
    public:
        static void bind(Foundry::Katana::GeolibCookInterface& iface, sol::state* lua) {


            // Direct Get Attribute of current node from katana, only return table
            auto getIntTuple = [&iface](const std::string& attname) {
                auto time = Foundry::Katana::GetCurrentTime(iface);
                std::string PAttName = "geometry.point.P";
                std::string arbitraryAttName = "geometry.arbitrary";
                std::string valueAttribName = arbitraryAttName;
                valueAttribName += ".";
                valueAttribName += attname;
                valueAttribName += ".";
                valueAttribName += "value";

                std::vector<int> values;
                if (attname == "P") {
                    return sol::as_table(values);
                }
                FnAttribute::IntAttribute valueAttr = iface.getAttr(valueAttribName);

                if (valueAttr.isValid()) {
                    auto samples = valueAttr.getNearestSample(time);
                    for (auto& val : samples) values.emplace_back(val);

                }
                else {
                    CARTESIAN_CORE_ERROR("query attribute no (value) attribute: {0}", valueAttribName);;
                }
                return sol::as_table(values);
            };

            auto getIntTuple2 = [&iface](const std::string& attname, const std::string& inputLocation, const int inputIndex) {
                std::vector<int> values;
                auto time = Foundry::Katana::GetCurrentTime(iface);
                std::string arbitraryAttName = "geometry.arbitrary";
                std::string valueAttribName = arbitraryAttName;
                valueAttribName += ".";
                valueAttribName += attname;
                valueAttribName += ".";
                valueAttribName += "value";


                try {
                    if (attname == "P") return sol::as_table(values);
                    iface.getAttr(valueAttribName, inputLocation, inputIndex);
                }
                catch (...) {
                    CARTESIAN_CORE_WARN("just check getfloattuple() , ignore this error: runtime_error");
                    throw GetAttribException();
                }

                FnAttribute::IntAttribute valueAttr = iface.getAttr(valueAttribName, inputLocation, inputIndex);
                if (valueAttr.isValid()) {
                    auto samples = valueAttr.getNearestSample(time);
                    for (auto& val : samples) values.emplace_back(val);

                }
                else {
                    CARTESIAN_CORE_ERROR("query attribute no (value) attribute: {0}", valueAttribName);;
                }
                return sol::as_table(values);
            };

            lua->set_function("getinttuple", sol::overload(getIntTuple, getIntTuple2));

        }

    };


    class GetStringTuple {
    public:
        static void bind(Foundry::Katana::GeolibCookInterface& iface, sol::state* lua) {


            // Direct Get Attribute of current node from katana, only return table
            auto getstringtuple = [&iface](const std::string& attname) {
                auto time = Foundry::Katana::GetCurrentTime(iface);
                std::string PAttName = "geometry.point.P";
                std::string arbitraryAttName = "geometry.arbitrary";
                std::string valueAttribName = arbitraryAttName;
                valueAttribName += ".";
                valueAttribName += attname;
                valueAttribName += ".";
                valueAttribName += "value";

                std::vector<std::string> values;
                if (attname == "P") {
                    return sol::as_table(values);
                }
                FnAttribute::StringAttribute valueAttr = iface.getAttr(valueAttribName);

                if (valueAttr.isValid()) {
                    auto samples = valueAttr.getNearestSample(time);
                    for (auto& val : samples) values.emplace_back(val);

                }
                else {
                    CARTESIAN_CORE_ERROR("query attribute no (value) attribute: {0}", valueAttribName);;
                }
                return sol::as_table(values);
            };

            auto getstringtuple2 = [&iface](const std::string& attname, const std::string& inputLocation, const int inputIndex) {
                std::vector<std::string> values;
                auto time = Foundry::Katana::GetCurrentTime(iface);
                std::string arbitraryAttName = "geometry.arbitrary";
                std::string valueAttribName = arbitraryAttName;
                valueAttribName += ".";
                valueAttribName += attname;
                valueAttribName += ".";
                valueAttribName += "value";


                try {
                    if (attname == "P") return sol::as_table(values);
                    iface.getAttr(valueAttribName, inputLocation, inputIndex);
                }
                catch (...) {
                    CARTESIAN_CORE_WARN("just check getfloattuple() , ignore this error: runtime_error");
                    throw GetAttribException();
                }

                FnAttribute::StringAttribute valueAttr = iface.getAttr(valueAttribName, inputLocation, inputIndex);
                if (valueAttr.isValid()) {
                    auto samples = valueAttr.getNearestSample(time);
                    for (auto& val : samples) values.emplace_back(val);

                }
                else {
                    CARTESIAN_CORE_ERROR("query attribute no (value) attribute: {0}", valueAttribName);;
                }
                return sol::as_table(values);
            };

            lua->set_function("getinttuple", sol::overload(getstringtuple, getstringtuple2));

        }

    };


}; // end of namespace 