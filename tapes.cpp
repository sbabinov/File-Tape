#include "tapes.hpp"
#include <algorithm>
#include <cstdio>
#include <ios>
#include <thread>
#include <list>
#include <chrono>
#include <tuple>
#include <vector>

tapes::Tape::Tape(size_t size):
  size_(size),
  pos_(0)
{}

size_t tapes::Tape::size() const
{
  return size_;
}

size_t tapes::Tape::pos() const
{
  return pos_;
}

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

tapes::FileTape::~FileTape()
{
  if (file_.is_open())
  {
    file_.close();
  }
}

tapes::Config tapes::FileTape::getConfig() const
{
  return config_;
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

tapes::Sorter::Sorter(std::shared_ptr< FileTape > inTape, std::shared_ptr< FileTape > outTape, size_t ramSize):
  inTape_(inTape),
  outTape_(outTape),
  ramLimit_(ramSize)
{}

void tapes::Sorter::operator()()
{
  std::vector< std::shared_ptr< FileTape > > tempTapes;

  size_t tapeNum = 1;
  while (inTape_->pos() < inTape_->size())
  {
    size_t tapeSize = ramLimit_;
    if (inTape_->size() - inTape_->pos() < ramLimit_)
    {
      tapeSize = inTape_->size() - inTape_->pos();
    }
    std::vector< int > ram(tapeSize);
    for (size_t i = 0; i < tapeSize; ++i)
    {
      ram[i] = inTape_->read();
      inTape_->moveForward();
    }
    std::sort(ram.begin(), ram.end());

    auto tempTape = std::make_shared< FileTape >(TEMP_TAPES_PATH + std::to_string(tapeNum) + ".tp",
      tapeSize, inTape_->getConfig());
    for (auto it = ram.begin(); it != ram.end(); ++it)
    {
      tempTape->write(*it);
      tempTape->moveForward();
    }
    tempTape->rewind();
    tempTapes.push_back(tempTape);
    ++tapeNum;
  }

  using ElementInfo = std::tuple< int, size_t, size_t >;
  std::list< ElementInfo > ram;
  for (size_t i = 0; i < tempTapes.size(); ++i)
  {
    ram.push_back({tempTapes[i]->read(), i, 0});
    tempTapes[i]->moveForward();
  }

  auto pred = [&](const ElementInfo& el1, const ElementInfo& el2) -> bool
  {
    return std::get< 0 >(el1) < std::get< 0 >(el2);
  };

  while (!ram.empty())
  {
    auto it = std::min_element(ram.begin(), ram.end(), pred);
    auto [value, tapeInd, pos] = *it;
    outTape_->write(value);
    outTape_->moveForward();

    if (tempTapes[tapeInd]->pos() < tempTapes[tapeInd]->size())
    {
      ram.push_back({tempTapes[tapeInd]->read(), tapeInd, pos + 1});
      tempTapes[tapeInd]->moveForward();
    }
    ram.erase(it);
  }

  for (size_t i = 0; i < tempTapes.size(); ++i)
  {
    std::string filename = TEMP_TAPES_PATH + std::to_string(i + 1) + ".tp";
    std::remove(filename.c_str());
  }
}
