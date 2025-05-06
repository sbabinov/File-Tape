#ifndef CONVERTER_HPP
#define CONVERTER_HPP
#include <string>

namespace converter
{
  class FileConverter
  {
  public:
    FileConverter(const std::string& filename);
    size_t toBinary() const;
    size_t toText() const;
  private:
    std::string filename_;
  };
}

#endif
