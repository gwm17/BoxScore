#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace BoxScore {

    class Logger
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger> GetLogger() { return s_logger; }

    private:
        static std::shared_ptr<spdlog::logger> s_logger;
    };


//Macros for simple logging.

#define BS_CRITICAL(...) ::BoxScore::Logger::GetLogger()->critical(__VA_ARGS__)
#define BS_WARN(...) ::BoxScore::Logger::GetLogger()->warn(__VA_ARGS__)
#define BS_INFO(...) ::BoxScore::Logger::GetLogger()->info(__VA_ARGS__)
#define BS_TRACE(...) ::BoxScore::Logger::GetLogger()->trace(__VA_ARGS__)
#define BS_ERROR(...) ::BoxScore::Logger::GetLogger()->error(__VA_ARGS__)
}