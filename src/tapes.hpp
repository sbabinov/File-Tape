#ifndef TAPES_HPP
#define TAPES_HPP
#include <istream>
#include <fstream>
#include <memory>
#include <string>

namespace tapes
{
  const std::string TEMP_TAPES_PATH = "../tmp/";

  struct Config
  {
    size_t readDelay;
    size_t writeDelay;
    size_t moveDelay;
    size_t rewindDelay;
  };
  std::istream& operator>>(std::istream& in, Config& config);

  class Tape
  {
  public:
    Tape(size_t size);
    virtual ~Tape() = default;

    size_t size() const;
    size_t pos() const;
    virtual int read() = 0;
    virtual void write(int value) = 0;
    virtual void moveForward() = 0;
    virtual void moveBackward() = 0;
    virtual void rewind() = 0;
  protected:
    size_t size_;
    size_t pos_;
  };

  class FileTape: public Tape
  {
  public:
    FileTape(const std::string& filename, size_t size, const Config& config);
    ~FileTape();

    Config getConfig() const;
    virtual int read() override;
    virtual void write(int value) override;
    virtual void moveForward() override;
    virtual void moveBackward() override;
    virtual void rewind() override;
  private:
    std::fstream file_;
    Config config_;

    void simDelay(size_t ms) const;
  };

  class Sorter
  {
  public:
    Sorter(std::shared_ptr< FileTape > inTape, std::shared_ptr< FileTape > outTape, size_t ramSize);
    void operator()();
  private:
    std::shared_ptr< FileTape > inTape_;
    std::shared_ptr< FileTape > outTape_;
    size_t ramLimit_;
  };
}

#endif
