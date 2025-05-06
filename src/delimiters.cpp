#include "delimiters.hpp"
#include <ios>

std::istream& delimiters::operator>>(std::istream& in, CharDelimiter&& del)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = 0;
  in >> c;
  if (c != del.expected)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& delimiters::operator>>(std::istream& in, StringDelimiter&& del)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  const char* expected = del.expected;
  while ((*expected != '\0') && in)
  {
    in >> CharDelimiter{*expected};
    ++expected;
  }
  return in;
}
