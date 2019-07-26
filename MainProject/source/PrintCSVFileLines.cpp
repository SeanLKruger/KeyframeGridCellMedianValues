#include <tuple>
#include <vector>
#include <string>

#include <sstream>
#include <iomanip>

#include "PrintCSVFileLines.hpp"

namespace CSV {

  // Print results of calculating median values of grayscale pixels of each grid cell for keyframe at reported video timestamp.
  void PrintCSVFileLines(std::vector<std::tuple<double, std::vector<double>>> csvFileLinesComprisingTimestampAndMedianValueOfPixelsOfEachGridCell)
  {
    // Print results for each keyframe. (See embellished function comment.)
    for (std::tuple<double, std::vector<double>> csvFileLineComprisingTimestampAndMedianValueOfPixelsOfEachGridCell :
      csvFileLinesComprisingTimestampAndMedianValueOfPixelsOfEachGridCell) {

      // Extract values from tuple.
      double keyframeTimestamp;
      std::vector<double> medianValuesOfPixelsOfEachGridCell;
      std::tie(keyframeTimestamp, medianValuesOfPixelsOfEachGridCell) = csvFileLineComprisingTimestampAndMedianValueOfPixelsOfEachGridCell;

      // Write timestamp of keyframe.
      std::stringstream keyframeTimestampOfWidthWithTrailingZeroesFormatter;
      keyframeTimestampOfWidthWithTrailingZeroesFormatter << std::fixed << std::setprecision(6) << keyframeTimestamp;
      std::string keyframeTimestampOfWidthWithTrailingZeroes(keyframeTimestampOfWidthWithTrailingZeroesFormatter.str());
      CSVFileWriter::Instance() << keyframeTimestampOfWidthWithTrailingZeroes;

      // Write median values of grayscale pixels of each grid cell for keyframe.
      for(double medianValueOfPixelsOfEachGridCell : medianValuesOfPixelsOfEachGridCell) {
        CSVFileWriter::Instance() << medianValueOfPixelsOfEachGridCell;
      }

      CSVFileWriter::Instance() << endrow;
    }
  }
}//namespace CSV
