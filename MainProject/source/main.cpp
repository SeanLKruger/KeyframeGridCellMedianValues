#include <tuple>
#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

#include "ProcessArguments.hpp"
#include "VideoFileFrameInformation.hpp"
#include "CalculateMedianValuesOfPixelsOfFrameGridCells.hpp"

#include "PrintCSVFileLines.hpp"

// Application call flow, data pipeline:
//
// Main:
// ExtractKeyframesFromVideoFileAndSaveToFile
// LoadFramesFromVideoFileWithTheirTimestamps
//   ListTimestampsOfFramesInVideoFile
// CalculateMedianValuesOfPixelsOfFrameGridCells
// PrintCSVFileLines
//
// Test:
// CountKeyframesInVideoFile
//   compare to size of framesFromVideoFileWithTheirTimestamps
//   provides some verification of veracity of ffmpeg and ffprobe calls

int main(int argc, const char* argv[])
{
  // Get command-line arguments.
  ProcessArgumentsFromCommandLine processArgumentsFromCommandLine;
  processArgumentsFromCommandLine.GetCommandLineArguments(argc, argv);

  // Access CSVFileWriter Instance for first time to check exceptions early
  // and to open CSV file.

  CSV::CSVFileWriter::InstancePrimingAccess(ProcessArguments::Instance().GetCSVFilename());

  // Extract keyframes from video file.

  VideoFileKeyframeInformation::ExtractKeyframesFromVideoFileAndSaveToFile(
    ProcessArguments::Instance().GetVideoFileFromWhichToExtractKeyframes(),
    ProcessArguments::Instance().GetVideoFileComprisingOnlyKeyframes());

  // Get timestamps corresponding to keyframes.

  std::vector<VideoFileFrameInformation::FrameWithItsTimestamp> framesFromVideoFileWithTheirTimestamps(
    VideoFileFrameInformation::LoadFramesFromVideoFileWithTheirTimestamps(
      ProcessArguments::Instance().GetVideoFileComprisingOnlyKeyframes()));

  // Calculate median values of pixels of frame grid cells.

  CalculateMedianValuesOfPixelsOfFrameGridCells calculateMedianValuesOfPixelsOfFrameGridCells;
  std::vector<std::tuple<double, std::vector<double>>> csvFileLinesComprisingTimestampAndMedianValueOfPixelsOfEachGridCell(
    calculateMedianValuesOfPixelsOfFrameGridCells(framesFromVideoFileWithTheirTimestamps,
                                                  ProcessArguments::Instance().GetDimensionOfGridIntoWhichToSplitKeyframe()));

  // Print results to CSV file of comma separated format:
  // timestamp followed by list of values for each cell.
  // Note: Could be advice sice woven in by aspect-weaver.
  CSV::PrintCSVFileLines(csvFileLinesComprisingTimestampAndMedianValueOfPixelsOfEachGridCell);

  return EXIT_SUCCESS;
}
