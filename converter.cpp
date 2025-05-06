#include "converter.hpp"
#include <fstream>
#include <ios>
#include <vector>

converter::FileConverter::FileConverter(const std::string& filename):
  filename_(filename)
{}

void converter::FileConverter::toBinary() const
{
  std::ifstream in(filename_);
  if (!in)
  {
    throw std::runtime_error("Cannot open file: " + filename_);
  }

  std::vector< int > values;
  int value;
  while (in >> value)
  {
    values.push_back(value);
  }
  in.close();

  std::ofstream out(filename_, std::ios::binary | std::ios::trunc);
  if (!out)
  {
    throw std::runtime_error("Cannot open file: " + filename_);
  }
  for (auto it = values.begin(); it != values.end(); ++it)
  {
    out.write(reinterpret_cast< const char* >(&(*it)), sizeof((*it)));
  }
  out.close();
}

void converter::FileConverter::toText() const
{
  std::ifstream in(filename_, std::ios::binary);
  if (!in)
  {
    throw std::runtime_error("Cannot open file: " + filename_);
  }

  std::vector< int > values;
  int value;
  while (in.read(reinterpret_cast< char* >(&value), sizeof(value)))
  {
    values.push_back(value);
  }
  in.close();

  std::ofstream out(filename_, std::ios::trunc);
  if (!out)
  {
    throw std::runtime_error("Cannot open file: " + filename_);
  }
  for (auto it = values.begin(); it != values.end(); ++it)
  {
    out << *it << '\n';
  }
  out.close();
}
