#ifndef CONVERTER_HPP
#define CONVERTER_HPP
#include <string>

namespace converter
{
  class FileConverter
  {
  public:
    FileConverter(const std::string& filename);
    void toBinary() const;
    void toText() const;
  private:
    std::string filename_;
  };
}

#endif
