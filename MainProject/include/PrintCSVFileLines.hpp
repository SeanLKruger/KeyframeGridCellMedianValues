#ifndef PRINTCSVFILELINES_HPP
#define PRINTCSVFILELINES_HPP

#include <tuple>
#include <vector>

#include "CSVFileWriter.hpp"

namespace CSV {

  // Print results of calculating median values of grayscale pixels of each grid cell for keyframe at reported video timestamp.
  void PrintCSVFileLines(std::vector<std::tuple<double, std::vector<double>>> csvFileLinesComprisingTimestampAndMedianValueOfPixelsOfEachGridCell);

}//namespace CSV

#endif//PRINTCSVFILELINES_HPP
