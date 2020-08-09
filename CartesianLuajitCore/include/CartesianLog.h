//
// Created by admin on 2020/8/9.
//

#ifndef BUILDINGCTSLUAJIT_CARTESIANLOG_H
#define BUILDINGCTSLUAJIT_CARTESIANLOG_H
#include "CartesianConfig.h"
#include "spdlog/spdlog.h"
#include <memory>


#define CARTESIAN_CORE_TRACE(...)    ::Cartesian::Log::getLogger()->trace(__VA_ARGS__)
#define CARTESIAN_CORE_INFO(...)     ::Cartesian::Log::getLogger()->info(__VA_ARGS__)
#define CARTESIAN_CORE_WARN(...)     ::Cartesian::Log::getLogger()->warn(__VA_ARGS__)
#define CARTESIAN_CORE_ERROR(...)    ::Cartesian::Log::getLogger()->error(__VA_ARGS__)
#define CARTESIAN_CORE_CRITICAL(...) ::Cartesian::Log::getLogger()->critical(__VA_ARGS__)

namespace Cartesian {

    class CARTESIAN_API Log {
    public:
        static auto &getLogger(){return logger;}
        static void initialize();

    private:
        static std::shared_ptr<spdlog::logger> logger;
    };
}

#endif //BUILDINGCTSLUAJIT_CARTESIANLOG_H
