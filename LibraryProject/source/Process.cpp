#include <sstream>

#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream_buffer.hpp>

#include <boost/filesystem.hpp>

#include "StringUtil.hpp"

#include "Process.hpp"

void Process::LaunchProcess(const std::string& processName, const std::vector<std::string>& processArguments)
{
  // Find filesystem path to executable.
  boost::filesystem::path executablePath = boost::process::search_path(processName);

  // Run executable.
  std::error_code processErrorCode;
  boost::process::system(executablePath.string(), processArguments, processErrorCode);

  // Check process completion.
  CheckProcessReturnCode(processName, processArguments, processErrorCode);
}

std::vector<std::string> Process::ReadProcessStdout(const std::string& processName, const std::string& processArguments)
{
  // Construct child process specification as process name concatenated with its arguments.
  boost::filesystem::path executablePath = boost::process::search_path(processName);
  std::string processSpecification{executablePath.string() + " " + processArguments};

  // Run child process, piping its stdout to a threading future in this processes.
  std::future<std::vector<char>> childsStdOutput, childsStdError;
  boost::asio::io_service ioService;
  boost::process::child childProcess(processSpecification,
                                     boost::process::std_out > childsStdOutput,
                                     boost::process::std_err > childsStdError,
                                     ioService);
  ioService.run();

  // Read child processes' stdout.
  std::vector<std::string> stdOutLines;
  {
    auto rawChildsStdOutputData = childsStdOutput.get();
    std::string lineFromChildsStdout;
    boost::iostreams::stream_buffer<boost::iostreams::array_source> streamBuffer(rawChildsStdOutputData.data(), rawChildsStdOutputData.size());
    std::istream inputStreamFromChildsStdout(&streamBuffer);

    while (std::getline(inputStreamFromChildsStdout, lineFromChildsStdout) && !lineFromChildsStdout.empty()) {
      stdOutLines.push_back(lineFromChildsStdout);
    }
  }

  // Wait for child process to complete and check return code.
  std::error_code processErrorCode;
  childProcess.wait(processErrorCode);
  CheckProcessReturnCode(processName, processArguments, processErrorCode);

  // Return stdout lines produced by command issued to child process.
  return stdOutLines;
}

void Process::CheckProcessReturnCode(const std::string& processName, const std::string& processArguments, std::error_code processErrorCode)
{
  if (processErrorCode) {

    std::ostringstream errorMessage;
    errorMessage << "Failure launching or executing process: " << processName << std::endl << "with arguments: " << processArguments << std::endl;

    throw std::system_error(EDOM, std::generic_category(), errorMessage.str());
  }
}

void Process::CheckProcessReturnCode(const std::string& processName, const std::vector<std::string>& processArguments, std::error_code processErrorCode)
{
  if (processErrorCode) {
    CheckProcessReturnCode(processName, Stringify(processArguments), processErrorCode);
  }
}
