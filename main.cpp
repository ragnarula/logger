#define LOGGING_LEVEL_3
#include <iostream>
#include "log.hpp"

using namespace std;

LOG_INIT

int main()
{
    LOG_INST.register_policy(new logging::file_log_policy("exe.log"));
    LOG("NORMAL");
    LOG_ERR("ERROR");
    LOG_WARN("WARNING");
    cout << "Hello World!" << endl;
    return 0;
}

