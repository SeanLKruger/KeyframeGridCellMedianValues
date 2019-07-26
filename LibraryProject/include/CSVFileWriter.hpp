#ifndef CSVFILEWRITER_HPP
#define CSVFILEWRITER_HPP

#include <string>

#include <fstream>

namespace CSV {

  class CSVFileWriter;

  CSVFileWriter& endrow(CSVFileWriter& csvFileWriter);
  CSVFileWriter& flush(CSVFileWriter& csvFileWriter);

  class CSVFileWriter
  {
  public:

    static CSVFileWriter& Instance();                               // Create then subsequently access Singleton.
    static void InstancePrimingAccess(const std::string& filename); // Access Singleton to check exceptions.

    CSVFileWriter(CSVFileWriter const&) = delete;             // Copy construct.
    CSVFileWriter(CSVFileWriter&&) = delete;                  // Move construct.
    CSVFileWriter& operator=(CSVFileWriter const&) = delete;  // Copy assign.
    CSVFileWriter& operator=(CSVFileWriter &&) = delete;      // Move assign.

    void SetFilenameAndSeparator(const std::string filename, const std::string separator = ",");

    void flush();
    void endrow();

    CSVFileWriter& operator << (CSVFileWriter& (* other)(CSVFileWriter&));

    template<typename T>
    CSVFileWriter& operator << (const T& val);

  protected:

      CSVFileWriter();
     ~CSVFileWriter();

  private:

      template<typename T>
      CSVFileWriter& write(const T& value);

      std::ofstream _outputFileStream;
      bool _isFirstColumnInRow;
      std::string _separator;
  };

  template<typename T>
  CSVFileWriter& CSVFileWriter::write(const T& value)
  {
      if (!_isFirstColumnInRow) {
        _outputFileStream << _separator;
      }
      else {
        _isFirstColumnInRow = false;
      }

      _outputFileStream << value;

      return *this;
  }

  template<typename T>
  CSVFileWriter& CSVFileWriter::operator << (const T& value)
  {
    return write(value);
  }
}//namespace CSV

#endif//CSVFILEWRITER_HPP
