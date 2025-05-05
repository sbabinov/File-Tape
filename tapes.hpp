#ifndef TAPES_HPP
#define TAPES_HPP
#include <fstream>
#include <string>

namespace tapes
{
  struct Config
  {
    size_t readDelay;
    size_t writeDelay;
    size_t moveDelay;
    size_t rewindDelay;
  };

  class Tape
  {
  public:
    Tape(size_t size);
    virtual ~Tape() = default;

    size_t size();
    virtual int read();
    virtual void write(int value);
    virtual void moveForward();
    virtual void moveBackward();
    virtual void rewind();
  protected:
    size_t size_;
  };

  class FileTape: public Tape
  {
  public:
    FileTape(const std::string& filename, size_t size, const Config& config);
    ~FileTape();

    virtual int read() override;
    virtual void write(int value) override;
    virtual void moveForward() override;
    virtual void moveBackward() override;
    virtual void rewind() override;
  private:
    std::fstream file_;
    size_t pos_;
    Config config_;

    void simDelay(size_t time) const {};
  };
}

#endif
