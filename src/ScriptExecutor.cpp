#include "ScriptExecutor.hpp"

void    ScriptExecutor::sendSetting(
            std::string pass,
            std::string login,
            std::string ip)
{
    std::string     scriptPath = SCRIPT_PATH;
    std::string     script;

    scriptPath += "sendSetting.sh";
    script = scriptPath + " " + pass + " " + login + " " + ip;
    system(script.c_str());
}

void    ScriptExecutor::sendSelfInfo(
            std::string serrial_number,
            std::string ip)
{
    serrial_number = serrial_number;
    ip = ip;
}

void     ScriptExecutor::execute(int count, ...) {
    va_list         args;
    std::string     script;

    va_start(args, count);
    while (count--) {
        script += va_arg(args, char *);
        script += " ";
    }

    system(script.c_str());
}

// with output

std::string ScriptExecutor::getOutput::checkWAN() {
    std::string     scriptPath = SCRIPT_PATH;
    std::string     line;
    char            buff[64];
    FILE            *pipe;

    scriptPath += "checkConnectInternet.sh";
    pipe = popen(scriptPath.c_str(), "r");
    while (fgets(buff, sizeof(buff), pipe))
        line += buff;
    pclose(pipe);
    return line;
}

std::string ScriptExecutor::getOutput::execute(int count, ...) {
    va_list         args;
    std::string     script;

    va_start(args, count);
    while (count--) {
        script += va_arg(args, char *);
        script += " ";
    }


    std::string     line;
    char            buff[64];
    FILE            *pipe;

    pipe = popen(script.c_str(), "r");
    while (fgets(buff, sizeof(buff), pipe))
        line += buff;
    pclose(pipe);
    return line;
}
