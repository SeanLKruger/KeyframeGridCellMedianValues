#ifndef PROCESSARGUMENTS_HPP
#define PROCESSARGUMENTS_HPP

#include <string>

class ProcessArgumentsFromCommandLine
{
public:

    void GetCommandLineArguments(int argc, const char* argv[]);

private:

    // Setter callsback used by boost::program_options in extracting command-line arguments.

    void on_VideoFileFromWhichToExtractKeyframes(const std::string videoFileFromWhichToExtractKeyframes);
    void on_VideoCodecToExtractKeyframes(const std::string videoCodecToExtractKeyframes);

    void on_DimensionOfGridIntoWhichToSplitKeyframe(uint16_t dimensionOfGridIntoWhichToSplitKeyframe);

    void on_CSVFilename(const std::string csvFilename);
};

class ProcessArguments
{
  friend ProcessArgumentsFromCommandLine;

public:

  static ProcessArguments& Instance();                            // Create then subsequently access Singleton.

  ProcessArguments(ProcessArguments const&) = delete;             // Copy construct.
  ProcessArguments(ProcessArguments&&) = delete;                  // Move construct.
  ProcessArguments& operator=(ProcessArguments const&) = delete;  // Copy assign.
  ProcessArguments& operator=(ProcessArguments &&) = delete;      // Move assign.

  // Accessors.

  std::string GetVideoFileFromWhichToExtractKeyframes();
  std::string GetVideoCodecToExtractKeyframes();
  std::string GetVideoFileComprisingOnlyKeyframes();

  uint16_t GetDimensionOfGridIntoWhichToSplitKeyframe();

  std::string GetCSVFilename();

protected:

  ProcessArguments() {}
 ~ProcessArguments() {}

  // Setters

  void SetVideoFileFromWhichToExtractKeyframes(const std::string& videoFileFromWhichToExtractKeyframes);
  void SetVideoCodecToExtractKeyframes(const std::string& videoCodecToExtractKeyframes);
  void SetVideoFileComprisingOnlyKeyframes();

  void SetDimensionOfGridIntoWhichToSplitKeyframe(uint16_t dimensionOfGridIntoWhichToSplitKeyframe);

  void SetCSVFilename(const std::string& csvFilename);

private:

  std::string _videoFileFromWhichToExtractKeyframes;
  std::string _videoCodecToExtractKeyframes;
  std::string _videoFileComprisingOnlyKeyframes;

  uint16_t _dimensionOfGridIntoWhichToSplitKeyframe = 1;

  std::string _csvFilename;
};

#endif//PROCESSARGUMENTS_HPP
