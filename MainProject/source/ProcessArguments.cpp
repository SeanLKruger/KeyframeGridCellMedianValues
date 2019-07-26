#include <iostream>

#include <functional>

#include <boost/function.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "ProcessArguments.hpp"

ProcessArguments& ProcessArguments::Instance()
{
  static ProcessArguments instance; // Static variable so created once(thread-safe).

  return instance;
}

void ProcessArguments::SetVideoFileFromWhichToExtractKeyframes(const std::string& videoFileFromWhichToExtractKeyframes)
{
  _videoFileFromWhichToExtractKeyframes = videoFileFromWhichToExtractKeyframes;
}

std::string ProcessArguments::GetVideoFileFromWhichToExtractKeyframes()
{
  return _videoFileFromWhichToExtractKeyframes;
}

void ProcessArguments::SetVideoCodecToExtractKeyframes(const std::string& videoCodecToExtractKeyframes)
{
  _videoCodecToExtractKeyframes = videoCodecToExtractKeyframes;
}

std::string ProcessArguments::GetVideoCodecToExtractKeyframes()
{
  return _videoCodecToExtractKeyframes;
}

void ProcessArguments::SetVideoFileComprisingOnlyKeyframes()
{
  // Create video file comprising only keyframes in same directory as video file from which to extract keyframes.
  boost::filesystem::path path(_videoFileFromWhichToExtractKeyframes);
  boost::filesystem::path directory = path.parent_path();
  // Output file extension informs ffmpeg which video codec to use to extract keyframes from a video file.
  _videoFileComprisingOnlyKeyframes = directory.string() + "/videoFileComprisingKeyframes" "." + _videoCodecToExtractKeyframes;
}

std::string ProcessArguments::GetVideoFileComprisingOnlyKeyframes()
{
  return _videoFileComprisingOnlyKeyframes;
}

void ProcessArguments::SetDimensionOfGridIntoWhichToSplitKeyframe(uint16_t dimensionOfGridIntoWhichToSplitKeyframe)
{
  _dimensionOfGridIntoWhichToSplitKeyframe = dimensionOfGridIntoWhichToSplitKeyframe;
}

uint16_t ProcessArguments::GetDimensionOfGridIntoWhichToSplitKeyframe()
{
  return _dimensionOfGridIntoWhichToSplitKeyframe;
}

void ProcessArguments::SetCSVFilename(const std::string& csvFilename)
{
  _csvFilename = csvFilename;
}

std::string ProcessArguments::GetCSVFilename()
{
  return _csvFilename;
}

void ProcessArgumentsFromCommandLine::on_VideoFileFromWhichToExtractKeyframes(const std::string videoFromWhichToExtractKeyframes)
{
  ProcessArguments::Instance().SetVideoFileFromWhichToExtractKeyframes(videoFromWhichToExtractKeyframes);
}

void ProcessArgumentsFromCommandLine::on_VideoCodecToExtractKeyframes(const std::string videoCodecToExtractKeyframes)
{
  ProcessArguments::Instance().SetVideoCodecToExtractKeyframes(videoCodecToExtractKeyframes);
}

void ProcessArgumentsFromCommandLine::on_DimensionOfGridIntoWhichToSplitKeyframe(uint16_t dimensionOfGridIntoWhichToSplitKeyframe)
{
  ProcessArguments::Instance().SetDimensionOfGridIntoWhichToSplitKeyframe(dimensionOfGridIntoWhichToSplitKeyframe);
}

void ProcessArgumentsFromCommandLine::on_CSVFilename(const std::string csvFilename)
{
  ProcessArguments::Instance().SetCSVFilename(csvFilename);
}

void ProcessArgumentsFromCommandLine::GetCommandLineArguments(int argc, const char* argv[])
{
  try
  {
    // Default values used if command-line argument omitted.

    const std::string defaultVideoFromWhichToExtractKeyframes(
      boost::filesystem::current_path().string() + "/../../../media/videoFileFromWhichToExtractKeyframes.mp4");
    const std::string defaultVideoCodecToExtractKeyframes("y4m");

    const uint16_t defaultDimensionOfGridIntoWhichToSplitKeyframe = 4;

    const std::string defaultCSVFilenameTimestampAndGrayscaleKeyframeGridCellMedianValues(
      boost::filesystem::current_path().string() + "/../../../CSV_results/CSVTimestampAndGrayscaleKeyframeGridCellMedianValues.csv");

    boost::program_options::options_description programOptionsDescription{"program options"};
    programOptionsDescription.add_options()

      // Command-line usage instructions.
      ("help,h", "Help screen.")

      // Video from which to extract keyframes.
      ("video,v", boost::program_options::value<std::string>()
        ->default_value(defaultVideoFromWhichToExtractKeyframes)
        //->required()
        ->notifier(std::bind1st(std::mem_fun(&ProcessArgumentsFromCommandLine::on_VideoFileFromWhichToExtractKeyframes), this)),
       "Video from which to extract keyframes.")

      // Output file extension which informs ffmpeg which video codec to use to extract keyframes from a video file.
      ("prot,p", boost::program_options::value<std::string>()
        ->default_value(defaultVideoCodecToExtractKeyframes)
        //->required()
        ->notifier(std::bind1st(std::mem_fun(&ProcessArgumentsFromCommandLine::on_VideoCodecToExtractKeyframes), this)),
        "Video codec for ffmpeg to use to extract keyframes from a video file.")

      // Dimension of grid into which to split keyframe.
      ("dim,d", boost::program_options::value<uint16_t>()
        ->default_value(defaultDimensionOfGridIntoWhichToSplitKeyframe)
        //->required()
        ->notifier(std::bind1st(std::mem_fun(&ProcessArgumentsFromCommandLine::on_DimensionOfGridIntoWhichToSplitKeyframe), this)),
        "Dimension into which to split keyframe.")

      // CSV file containing results of calculating median values of grayscale pixels of each grid cell for keyframe at reported video timestamp.
      ("csv,c", boost::program_options::value<std::string>()
        ->default_value(defaultCSVFilenameTimestampAndGrayscaleKeyframeGridCellMedianValues)
        //->required()
        ->notifier(std::bind1st(std::mem_fun(&ProcessArgumentsFromCommandLine::on_CSVFilename), this)),
        "CSV file to which to write timestamp and grayscale keyframe grid cell median values.")
    ;

    boost::program_options::variables_map programOptions;
    // Parse command-line.
    boost::program_options::store(parse_command_line(argc, argv, programOptionsDescription), programOptions);
    // Assign command-line arguments to class variables.
    boost::program_options::notify(programOptions);

    // Command-line usage instructions.
    if (programOptions.count("h") || programOptions.count("help")) {
      std::cout << programOptionsDescription << '\n';
    }

    // Assign derived member variables.
    ProcessArguments::Instance().SetVideoFileComprisingOnlyKeyframes();
  }
  catch (const boost::program_options::error &ex)
  {
    std::cerr << ex.what() << '\n';
  }
  catch (boost::bad_function_call &ex)
  {
    std::cerr << ex.what() << '\n';
  }
}
