#include <iostream>
#include <sstream>

#include "CSVFileWriter.hpp"

namespace CSV {

  CSVFileWriter::CSVFileWriter()
  : _outputFileStream()
  , _isFirstColumnInRow(true)
  {
  }

  CSVFileWriter::~CSVFileWriter()
  {
    flush();
    _outputFileStream.close();
  }

  CSVFileWriter& CSVFileWriter::Instance()
  {
    // Static variable so created once(thread-safe).
    static CSVFileWriter instance;

    return instance;
  }

  void CSVFileWriter::InstancePrimingAccess(const std::string& filename)
  {
    CSVFileWriter& csvFileWriter{CSVFileWriter::Instance()};
    const std::string separator(",");
    csvFileWriter.SetFilenameAndSeparator(filename, separator);
  }

  CSVFileWriter& CSVFileWriter::operator << (CSVFileWriter& (* other)(CSVFileWriter&))
  {
    return other(*this);
  }

  void CSVFileWriter::SetFilenameAndSeparator(const std::string filename, const std::string separator /* = "," */)
  {
    _outputFileStream.exceptions(std::ios::failbit | std::ios::badbit);
    _outputFileStream.open(filename);

    _separator = separator;
  }

  void CSVFileWriter::flush()
  {
    _outputFileStream.flush();
  }

  void CSVFileWriter::endrow()
  {
    _outputFileStream << std::endl;

    _isFirstColumnInRow = true;
  }

  // Output stream manipulators.

  CSVFileWriter& endrow(CSVFileWriter& csvFileWriter)
  {
    csvFileWriter.endrow();

    return csvFileWriter;
  }

  CSVFileWriter& flush(CSVFileWriter& csvFileWriter)
  {
    csvFileWriter.flush();

    return csvFileWriter;
  }
}//namespace CSV
