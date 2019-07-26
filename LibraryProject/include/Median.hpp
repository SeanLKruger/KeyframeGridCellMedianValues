#ifndef MEDIAN_HPP
#define MEDIAN_HPP

#include <vector>

#include <algorithm>

// Important: std::nth_element() reorders elements.

template<typename T>
double Median(std::vector<T>& vec)
{
  if(vec.empty()) {
    return T(0);
  }

  // Determine median.

  // Calculate position, halfway, where median would be found after partial sort of vector.
  typename std::vector<T>::difference_type medianPosition = vec.size() / 2;

  // Perform partial sort of vector, which positions median
  // at halfway index for odd count sets
  // or at right to virtual halfway for even count sets.
  std::nth_element(vec.begin(), vec.begin() + medianPosition, vec.end()); // Important: Reorders elements.

  // Dereference vector element at calculated position to obtain median value.
  T median = vec[static_cast<typename std::vector<T>::size_type>(medianPosition)];

  // If vector length an odd count.
  if((vec.size() & 1)) {
    return median;
  }
  // Vector length an even count.
  else {

    // In this case, the two sample observation sets are straddled by the two values needed to calculate the median:
    // median is the lowest value in sorted set of higher values,
    // so the highest value in the sorted set of lower values has to also be determined.

    // Median has to be calculated in terms of maximum value in partially sorted portion of vector.

    // Determine maximum value in partially sorted set of lower values.
    typename std::vector<T>::iterator maxElement = std::max_element(vec.begin(), vec.begin() + medianPosition);

    // Calculate median in terms of maximum value in partially sorted set of lower values.
    return (*maxElement + median) / 2.0;
  }
}

#endif//MEDIAN_HPP
