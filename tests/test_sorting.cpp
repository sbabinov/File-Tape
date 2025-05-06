#include <algorithm>
#include <fstream>
#include <string>
#include <iterator>
#include <memory>
#include <cstdio>
#include <filesystem>
#include "gtest/gtest.h"
#include "../src/tapes.hpp"
#include "../src/converter.hpp"

namespace fs = std::filesystem;

class TapeSortTest: public testing::TestWithParam< size_t >
{
protected:
    void SetUp() override
    {
      fs::create_directory(tapes::TEMP_TAPES_PATH);
      testNum = GetParam();
      inFilename = "../tests/tapes/inp" + std::to_string(testNum) + ".tp";
      outFilename = "../tests/tapes/out" + std::to_string(testNum) + ".tp";
      expectedFilename = "../tests/expected_output/out" + std::to_string(testNum) + ".tp";
    }
    size_t testNum;
    std::string inFilename;
    std::string outFilename;
    std::string expectedFilename;
};

TEST_P(TapeSortTest, SortsCorrectly)
{
  using iter = std::istream_iterator< int >;

  converter::FileConverter inConverter(inFilename);
  size_t tapeSize = inConverter.toBinary();

  auto inTape = std::make_shared< tapes::FileTape >(inFilename, tapeSize, tapes::Config{});
  auto outTape = std::make_shared< tapes::FileTape >(outFilename, tapeSize, tapes::Config{});
  tapes::Sorter sorter(inTape, outTape, (tapeSize / 3 > 0) ? (tapeSize / 3) : 1);
  sorter();

  converter::FileConverter(outFilename).toText();
  inConverter.toText();

  std::ifstream out(outFilename);
  std::ifstream expectedOut(expectedFilename);

  EXPECT_TRUE(std::equal(iter(out), iter(), iter(expectedOut)));

  std::remove(outFilename.c_str());
}

INSTANTIATE_TEST_SUITE_P(TapeTests, TapeSortTest, testing::Values(1, 2, 3, 4, 5, 6, 7, 8, 9));
