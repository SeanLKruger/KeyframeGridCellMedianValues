#include "StringUtil.hpp"

std::string Stringify(const std::vector<std::string>& vectorOfStrings, const std::string& separator /*= " "*/)
{
  std::string concatenatedStrings;

  for(const std::string& aString : vectorOfStrings) {
    concatenatedStrings += aString + separator;
  }

  return concatenatedStrings;
}
