#include "tapes.hpp"
#include <ios>

tapes::Tape::Tape(size_t size):
  size_(size)
{}

tapes::FileTape::FileTape(const std::string& filename, size_t size, const Config& config):
  Tape(size),
  config_(config)
{
  file_.open(filename, std::ios::in | std::ios::out | std::ios::binary);
  if (!file_.is_open())
  {
    file_.open(filename, std::ios::out | std::ios::binary);
    file_.close();
    file_.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (size_ > 0)
    {
      file_.seekp((size - 1) * sizeof(int));
      int zero = 0;
      file_.write(reinterpret_cast< const char* >(&zero), sizeof(zero));
      file_.flush();
    }
  }
  if (!file_.is_open())
  {
    throw std::runtime_error("Failed to open file: " + filename);
  }
}

