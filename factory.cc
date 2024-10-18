#include <iostream>
#include <cstdlib>
#include <memory>

// this is referred to as the "product"
class logger
{
public:
  virtual ~logger () = default;
  virtual void log (std::string const&) = 0;
};

class file_logger final : public logger
{
public:
  ~file_logger () = default;

  void
  log (std::string const& message) override
  {
    // log to a file here...
    std::cout << "logging to a file...\n";
  }
};

class console_logger final : public logger
{
public:
  ~console_logger () = default;

  void
  log (std::string const& message) override
  {
    // log to the console here...
    std::cout << "logging to the console...\n";
  }
};

// factory
class logger_factory
{
public:
  virtual ~logger_factory () = default;

  virtual std::unique_ptr<logger> create_logger () = 0;
};

class file_logger_factory final : public logger_factory
{
public:
  std::unique_ptr<logger>
  create_logger () override
  {
    return std::make_unique<file_logger> ();
  }
};

class console_logger_factory final : public logger_factory
{
public:
  std::unique_ptr<logger>
  create_logger () override
  {
    return std::make_unique<console_logger> ();
  }
};

// "client", "driver" or caller code, whatever
void
use_case (logger_factory* log_factory)
{
  auto logger = log_factory->create_logger ();
  logger->log ("Doing nasty stuff here");
}

int
main ()
{
  std::unique_ptr<logger_factory> file = std::make_unique<file_logger_factory> ();
  use_case (file.get ());

  std::unique_ptr<logger_factory> console = std::make_unique<console_logger_factory> ();
  use_case (console.get ());

  return EXIT_SUCCESS;
}
