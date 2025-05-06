#include <iostream>
#include <memory>
#include "tapes.hpp"
#include "converter.hpp"

int main(int argc, char* argv[])
{
  size_t ramLimit;
  if (argc < 3)
  {
    std::cerr << "Invalid parameters" << '\n';
    return 1;
  }
  if (argc > 3)
  {
    ramLimit = std::stoull(argv[3]);
  }

  tapes::Config config;
  std::ifstream confFile("../tape.config");
  if (!confFile)
  {
    std::cerr << "Cannot open config file" << '\n';
    return 1;
  }
  confFile >> config;
  if (confFile.fail())
  {
    std::cerr << "Invalid config file" << '\n';
    return 1;
  }

  converter::FileConverter inConverter(argv[1]);
  converter::FileConverter outConverter(argv[2]);
  size_t inTapeSize = inConverter.toBinary();
  size_t outTapeSize = outConverter.toBinary();

  auto inTape = std::make_shared< tapes::FileTape >(argv[1], inTapeSize, config);
  auto outTape = std::make_shared< tapes::FileTape >(argv[2], outTapeSize, config);

  tapes::Sorter sorter(inTape, outTape, 7);
  sorter();

  inConverter.toText();
  outConverter.toText();

  return 0;
}
