#ifndef TAPES_HPP
#define TAPES_HPP
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
    virtual ~Tape() = default;

    size_t size();
    virtual int read();
    virtual void write();
    virtual void rewind();
    virtual void moveForward();
    virtual void moveBackward();
  protected:
    size_t size_;
  };
}

#endif
