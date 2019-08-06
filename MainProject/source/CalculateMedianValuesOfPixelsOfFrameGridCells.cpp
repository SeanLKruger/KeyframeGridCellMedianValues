#include <exception>

#include <memory>

#include <vector>

#include <algorithm>

#include "CalculateMedianValuesOfPixelsOfFrameGridCells.hpp"

#include "Median.hpp"

std::vector<std::tuple<double, std::vector<double>>> CalculateMedianValuesOfPixelsOfFrameGridCells::operator()(
  std::vector<VideoFileFrameInformation::FrameWithItsTimestamp>& framesWithTheirTimestamps,
  uint16_t dimensionOfGridIntoWhichToSplitFrame)
{
  std::vector<std::tuple<double, std::vector<double>>> csvFileLineComprisingTimestampAndMedianValueOfPixelsOfEachGridCell;

  std::for_each (framesWithTheirTimestamps.begin(), framesWithTheirTimestamps.end(),
    [&](VideoFileFrameInformation::FrameWithItsTimestamp& frameWithItsTimestamp) {

    // Convert the keyframe into grayscale.
    cv::Mat grayscaleVersionOfFrame;
    cv::cvtColor(frameWithItsTimestamp._frame, grayscaleVersionOfFrame, cv::COLOR_RGB2GRAY);

    // Split each frame into a grid of dimensions given on command-line.
    std::vector<cv::Mat> grayscaleVersionOfFrameAsGrid(SplitFrameIntoGridOfDimensions(grayscaleVersionOfFrame, dimensionOfGridIntoWhichToSplitFrame));

    // Calculate median value of all the pixels of each cell of the grid.
    std::vector<double> medianValueOfPixelsOfEachGridCell(CalculateMedianValueOfPixelsOfEachGridCell(grayscaleVersionOfFrameAsGrid));

    // Compose results to tuple to be used to print CSV file line of comma-separated format:
    // timestamp followed by list of median values for each cell of grid's grayscale pixels.
    csvFileLineComprisingTimestampAndMedianValueOfPixelsOfEachGridCell.push_back(
      std::make_tuple(frameWithItsTimestamp._timestamp, medianValueOfPixelsOfEachGridCell));
  });

  return csvFileLineComprisingTimestampAndMedianValueOfPixelsOfEachGridCell;
}

std::vector<cv::Mat> CalculateMedianValuesOfPixelsOfFrameGridCells::SplitFrameIntoGridOfDimensions(
  const cv::Mat& grayscaleVersionOfFrame, uint16_t dimensionOfGridIntoWhichToSplitFrame)
{
  std::vector<cv::Mat> frameInGridOfDimensions;

  // Calculate grid cell row and column extents.

  if (grayscaleVersionOfFrame.rows % dimensionOfGridIntoWhichToSplitFrame
  || grayscaleVersionOfFrame.cols % dimensionOfGridIntoWhichToSplitFrame) {

    throw std::runtime_error("Dimension of grid into which to split frame must be even divisor of row and column counts of video frames.");
  }

  int32_t gridCellRowCount = grayscaleVersionOfFrame.rows / dimensionOfGridIntoWhichToSplitFrame;
  int32_t gridCellColCount = grayscaleVersionOfFrame.cols / dimensionOfGridIntoWhichToSplitFrame;

  // Split frame into grid cells.
  for (int frameRow = 0; frameRow < grayscaleVersionOfFrame.rows; frameRow += gridCellRowCount) {
    for (int frameCol = 0; frameCol < grayscaleVersionOfFrame.cols; frameCol += gridCellColCount) {

      // Create tile comprising copy of frame block at grid cell location.
      // One of the tile sides can be smaller than dimension of grid into which to split frame
      // if frame size is not a factor of dimensionOfGridIntoWhichToSplitKeyframe.

      //TODO Rather than necessarily requiring an even divisor,
      // check if divisor of frame dimensions (grid dimensions) gives *remainder*
      // and then additionally handle those extra, smaller cells at right and bottom edges.

      cv::Mat tileCopyOfFrameGridCell = grayscaleVersionOfFrame(
          cv::Range(frameRow, frameRow + gridCellRowCount),
          cv::Range(frameCol, frameCol + gridCellColCount))
        .clone(); // Copy data.

      frameInGridOfDimensions.push_back(tileCopyOfFrameGridCell);
    }
  }

  return frameInGridOfDimensions;
}

template<bool isContinuous = true, typename PixelT = uint8_t, class IterT = cv::MatConstIterator_<PixelT>>
struct ExtractPixelsOfFrameGridCell_impl;

template<typename PixelT, class IterT>
struct ExtractPixelsOfFrameGridCell_impl<true, PixelT, IterT>
{
  std::vector<PixelT> operator()(cv::MatConstIterator_<PixelT>& beginFrameGridCell,
                                 cv::MatConstIterator_<PixelT>& endFrameGridCell) const
  {
    return std::vector<PixelT>(beginFrameGridCell, endFrameGridCell);
  }
};

template<typename PixelT, class IterT>
struct ExtractPixelsOfFrameGridCell_impl<false, PixelT, IterT>
{
  std::vector<PixelT> operator()(cv::MatConstIterator_<PixelT>& beginFrameGridCell,
                                 cv::MatConstIterator_<PixelT>& endFrameGridCell) const
  {
    std::vector<PixelT> pixelsOfFrameGridCell;
    std::copy(beginFrameGridCell, endFrameGridCell, std::back_inserter(pixelsOfFrameGridCell));
    return pixelsOfFrameGridCell;
  }
};

template<bool isContinuous = true, typename PixelT = uint8_t, class IterT = cv::MatConstIterator_<PixelT>>
class PixelsOfFrameGridCellExtractor
{
public:

  std::vector<PixelT> ExtractPixelsOfFrameGridCell(cv::MatConstIterator_<PixelT>& beginFrameGridCell,
                                                   cv::MatConstIterator_<PixelT>& endFrameGridCell)
  {
    return ExtractPixelsOfFrameGridCell_impl<isContinuous, PixelT, IterT>()(beginFrameGridCell, endFrameGridCell);
  }
};

std::vector<double> CalculateMedianValuesOfPixelsOfFrameGridCells::CalculateMedianValueOfPixelsOfEachGridCell(
  const std::vector<cv::Mat>& grayscaleVersionOfFrameAsGrid)
{
  std::vector<double> medianValuesOfPixelsOfEachGridCell;

  // For each cell of frame grid.
  for (const cv::Mat& grayscaleVersionOfFrameGridCell : grayscaleVersionOfFrameAsGrid) {

    // Retrieve each pixel from frame grid cell.
    // Mat pixel type == CV_8UC1 == uchar == uint8_t.

    cv::MatConstIterator_<uint8_t>
      beginFrameGridCell = grayscaleVersionOfFrameGridCell.begin<uint8_t>(),
      endFrameGridCell   = grayscaleVersionOfFrameGridCell.end<uint8_t>();

    std::vector<uint8_t> pixelsOfFrameGridCell;

    // Determine method of pixel extraction - array-wise or by iteration.
    if (grayscaleVersionOfFrameGridCell.isContinuous()) {
      // Array-wise copy.
      const bool whetherPixelsContinuous = true;
      PixelsOfFrameGridCellExtractor<whetherPixelsContinuous> pixelsOfFrameGridCellExtractor;
      pixelsOfFrameGridCell = pixelsOfFrameGridCellExtractor.ExtractPixelsOfFrameGridCell(beginFrameGridCell, endFrameGridCell);
    }
    else {
      // Copy by iteration.
      pixelsOfFrameGridCell.clear();
      const bool whetherPixelsContinuous = false;
      PixelsOfFrameGridCellExtractor<whetherPixelsContinuous> pixelsOfFrameGridCellExtractor;
      pixelsOfFrameGridCell = pixelsOfFrameGridCellExtractor.ExtractPixelsOfFrameGridCell(beginFrameGridCell, endFrameGridCell);
    }

    medianValuesOfPixelsOfEachGridCell.push_back(Median(pixelsOfFrameGridCell));
  }

  return medianValuesOfPixelsOfEachGridCell;
}
