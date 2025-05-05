#include "tapes.hpp"
#include <ios>
#include <iostream>
#include <thread>
#include <chrono>

tapes::Tape::Tape(size_t size):
  size_(size)
{}

size_t tapes::Tape::size() const
{
  return size_;
}

tapes::FileTape::FileTape(const std::string& filename, size_t size, const Config& config):
  Tape(size),
  pos_(0),
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

tapes::FileTape::~FileTape()
{
  if (file_.is_open())
  {
    file_.close();
  }
}

int tapes::FileTape::read()
{
  simDelay(config_.readDelay);

  int value = 0;
  file_.seekg(pos_ * sizeof(int));
  file_.read(reinterpret_cast< char* >(&value), sizeof(value));
  if (file_.fail())
  {
    throw std::runtime_error("Failed to read");
  }
  return value;
}

void tapes::FileTape::write(int value)
{
  simDelay(config_.writeDelay);

  file_.seekp(pos_ * sizeof(int));
  file_.write(reinterpret_cast< const char* >(&value), sizeof(value));
  file_.flush();
  if (file_.fail())
  {
    throw std::runtime_error("Failed to write");
  }
}

void tapes::FileTape::moveForward()
{
  simDelay(config_.moveDelay);
  ++pos_;
}

void tapes::FileTape::moveBackward()
{
  simDelay(config_.moveDelay);
  --pos_;
}

void tapes::FileTape::rewind()
{
  simDelay(config_.rewindDelay);
  pos_ = 0;
}

void tapes::FileTape::simDelay(size_t ms) const
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
