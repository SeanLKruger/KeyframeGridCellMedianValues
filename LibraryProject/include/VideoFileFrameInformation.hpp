#ifndef VIDEOFILEFRAMEINFORMATION_HPP
#define VIDEOFILEFRAMEINFORMATION_HPP

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

class VideoFileFrameInformation
{
public:

  struct FrameWithItsTimestamp
  {
    cv::Mat _frame;
    double  _timestamp;
  };

  static std::vector<std::string> ListTimestampsOfFramesInVideoFile(const std::string& videoFile);

  static std::vector<VideoFileFrameInformation::FrameWithItsTimestamp> LoadFramesFromVideoFileWithTheirTimestamps(const std::string& videoFile);
};

class VideoFileKeyframeInformation
{
public:

  static long CountKeyframesInVideoFile(const std::string& videoFile);

  static void ExtractKeyframesFromVideoFileAndSaveToFile(const std::string& videoFileFromWhichToExtractKeyframes,
                                                         const std::string& videoFileComprisingOnlyKeyframes);
};

#endif//VIDEOFILEFRAMEINFORMATION_HPP
