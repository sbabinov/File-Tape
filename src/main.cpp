#include <iostream>
#include <memory>
#include <filesystem>
#include "tapes.hpp"
#include "converter.hpp"

int main(int argc, char* argv[])
{
  namespace fs = std::filesystem;

  size_t ramLimit = 0;
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

  fs::create_directory(tapes::TEMP_TAPES_PATH);

  converter::FileConverter inConverter(argv[1]);
  try
  {
    converter::FileConverter outConverter(argv[2]);
    outConverter.toBinary();
  }
  catch(const std::runtime_error&)
  {}

  size_t tapeSize = inConverter.toBinary();

  auto inTape = std::make_shared< tapes::FileTape >(argv[1], tapeSize, config);
  auto outTape = std::make_shared< tapes::FileTape >(argv[2], tapeSize, config);
  if (ramLimit == 0)
  {
    ramLimit = inTape->size();
  }

  tapes::Sorter sorter(inTape, outTape, ramLimit);
  sorter();

  inConverter.toText();
  converter::FileConverter outConverter(argv[2]);
  outConverter.toText();

  return 0;
}
