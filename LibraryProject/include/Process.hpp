#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <vector>
#include <string>

#include <system_error>

class Process
{
public:

  static void LaunchProcess(const std::string& processName, const std::vector<std::string>& processArguments);

  static std::vector<std::string> ReadProcessStdout(const std::string& processName, const std::string& processArguments);

private:

  static void CheckProcessReturnCode(const std::string& processName, const std::string& processArguments, std::error_code processErrorCode);
  static void CheckProcessReturnCode(const std::string& processName, const std::vector<std::string>& processArguments, std::error_code processErrorCode);
};

#endif//PROCESS_HPP
