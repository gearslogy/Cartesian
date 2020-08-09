//
// Created by admin on 2020/8/9.
//

#include "CartesianLog.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Cartesian{

    std::shared_ptr<spdlog::logger> Log::logger;
    void Log::initialize() {
        logger = spdlog::stdout_color_mt("CARTESIAN");
        spdlog::set_pattern("%^[%T] %n: %v%$" );
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
    }


}