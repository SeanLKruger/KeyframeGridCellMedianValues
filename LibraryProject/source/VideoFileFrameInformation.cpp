#include <stdexcept>

#include <iostream>

#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include "StringUtil.hpp"

#include "Process.hpp"

#include "VideoFileFrameInformation.hpp"

//----------------------------------------------------------------
// Video file information.

std::vector<std::string> VideoFileFrameInformation::ListTimestampsOfFramesInVideoFile(const std::string& videoFile)
{
  // Construct ffprobe command to list timestamps of frames in video file, and return value.

  const std::string ffprobe("ffprobe");
  const std::vector<std::string> ffprobeArguments
  {
    "-skip_frame", "nokey",                 // Select keyframes.
    "-select_streams", "v:0",               // Video stream.
    "-show_entries", "frame=pkt_pts_time",  // Timestamp.
    "-of", "csv=print_section=0",           // Output format.
    videoFile
  };

  return Process::ReadProcessStdout(ffprobe, Stringify(ffprobeArguments));
}

std::vector<VideoFileFrameInformation::FrameWithItsTimestamp> VideoFileFrameInformation::LoadFramesFromVideoFileWithTheirTimestamps(const std::string& videoFile)
{
  std::vector<FrameWithItsTimestamp> framesWithTheirTimestamps;

  try {

    // Retrieve timestamps of frames in video file.
    std::vector<std::string> timestampsOfFramesInVideoFile(ListTimestampsOfFramesInVideoFile(videoFile));

    // Open video file.
    cv::VideoCapture videoCapture(videoFile, cv::CAP_FFMPEG);
    if (!videoCapture.isOpened()) {
      CV_Error(cv::Error::Code::StsError, "Cannot open video file");
    }

    // Check video frame count same as count of timestamps of frames determined for same file.
    const size_t frameCount(static_cast<size_t>(videoCapture.get(cv::CAP_PROP_FRAME_COUNT)));
    if (frameCount != timestampsOfFramesInVideoFile.size()) {
      throw std::range_error("Video file frame count != count of timestamps of frames determined for same file.");
    }

    // Cycle through frames in video file.
    for (size_t frameNumber = 0; frameNumber < frameCount; ++frameNumber) {

      // Read next frame from video in file.
      cv::Mat keyframeFromFile; // Mat must be instantiated afresh in loop.
      videoCapture >> keyframeFromFile;
      if (keyframeFromFile.empty()) {
        break;
      }

      // Get timestamp corresponding to frame just read.
      double timestamp = std::stod(timestampsOfFramesInVideoFile[frameNumber]);

      // Add frame and timestamp to collection.
      framesWithTheirTimestamps.push_back(VideoFileFrameInformation::FrameWithItsTimestamp{keyframeFromFile, timestamp});
    }

    videoCapture.release();
  }
  catch (cv::Exception& ex) {
    std::cerr << ex.msg << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return framesWithTheirTimestamps;
}

//----------------------------------------------------------------
// Video file keyframe information.

long VideoFileKeyframeInformation::CountKeyframesInVideoFile(const std::string& videoFile)
{
  // Construct ffprobe command to count only keyframes from video file, and return value.

  const std::string ffprobe("ffprobe");
  const std::vector<std::string> ffprobeArguments
  {
    "-v", "error",
    "-count_frames",
    "-select_streams", "v:0",
    "-show_entries", "stream=nb_read_frames",
    "-of", "default=nokey=1:noprint_wrappers=1", // nokey is keyframes-only specification.
    videoFile.c_str()
  };

  std::vector<std::string> countKeyframesInVideoFile(Process::ReadProcessStdout(ffprobe, Stringify(ffprobeArguments)));

  if(!countKeyframesInVideoFile.empty()) {
    return std::stol(countKeyframesInVideoFile[0]); // The one-and-only string returned.
  }
  else {
    return 0;
  }
}

void VideoFileKeyframeInformation::ExtractKeyframesFromVideoFileAndSaveToFile(
  const std::string& videoFileFromWhichToExtractKeyframes,
  const std::string& videoFileComprisingOnlyKeyframes)
{
  // Construct ffmpeg command to select only keyframes from video file, keeping timestamp.
  // Video codec for ffmpeg to use to extract keyframes is specified by output file extension.

  const std::string ffmpeg("ffmpeg");
  std::vector<std::string> ffmpegArguments
  {
    "-skip_frame", "nokey", // Select keyframes. Or, "-vf", "select=eq(pict_type\\,I)",
    "-vsync", "vfr",        // Select only keyframes, keep timestamp. // 1
    "-i", videoFileFromWhichToExtractKeyframes,
    videoFileComprisingOnlyKeyframes
  };

  Process::LaunchProcess(ffmpeg, ffmpegArguments);

  // 1
  // -vsync vfr
  //   Filter to use a variable bitrate video synchronization.
  //   If not used, ffmpeg will fail to find only the keyframes.
  //   Frames are passed through with their timestamp or dropped
  //   so as to prevent 2 frames from having the same timestamp.
}
