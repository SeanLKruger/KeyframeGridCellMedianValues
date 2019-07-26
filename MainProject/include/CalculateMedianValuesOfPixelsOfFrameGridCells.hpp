#ifndef CALCULATEMEDIANVALUEOFPIXELSOFFRAMEGRIDCELLS_HPP
#define CALCULATEMEDIANVALUEOFPIXELSOFFRAMEGRIDCELLS_HPP

#include <tuple>
#include <vector>

#include <opencv2/opencv.hpp>

#include "VideoFileFrameInformation.hpp"

class CalculateMedianValuesOfPixelsOfFrameGridCells
{
public:

  std::vector<std::tuple<double, std::vector<double>>>
  operator()(std::vector<VideoFileFrameInformation::FrameWithItsTimestamp>& framesWithTheirTimestamps,
             uint16_t dimensionOfGridIntoWhichToSplitFrame);

private:

  std::vector<cv::Mat> SplitFrameIntoGridOfDimensions(const cv::Mat& grayscaleVersionOfFrame, uint16_t dimensionOfGridIntoWhichToSplitFrame);

  std::vector<double> CalculateMedianValueOfPixelsOfEachGridCell(const std::vector<cv::Mat>& grayscaleVersionOfFrameAsGrid);
};

#endif//CALCULATEMEDIANVALUEOFPIXELSOFFRAMEGRIDCELLS_HPP
