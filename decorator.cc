#include <cstdlib>
#include <iostream>
#include <memory>

//
// This design pattern is especially useful for situations where
// you're working exclusively with an object oriented design (you're
// probably already fucked) and you need to do combinations of classes
// between them.
//
// Since you need to combine the functionality between all of these
// classes, you'll quickly be in a bad situation where you have a lot
// of subclasses, inheritance, and god knows what else.
//
// Example: suppose you're originally working with an email notifier.
// Now all of sudden the requirements change and you need to have a
// sms+push notifiers. Further, sometimes you need an email + sms,
// other times sms + push, push + email, etc.
//
// If you were to use inheritance, you'd have these classes (at least):
//
// - email_notifier
// - sms_notifier
// - push_notifier
// - email_sms_notifier
// - email_push_notifier
// - sms_push_notifier
// - email_sms_push_notifier
//
// .................................... pretty fucking bad, isn't it?
//
// The idea of the decorator pattern is to add responsibilities or
// functionality at dynamic or static time to an INDIVIDUAL object.
// You can do this by wrapping the original object in a decorator
// object that adds the new funcionality.
//

class notifier
{
public:
  virtual ~notifier () = default;
  virtual void send (std::string const& message) = 0;
};

class simple_notifier final : public notifier
{
public:
  ~simple_notifier () = default;

  void
  send (std::string const& message) override
  {
    // base functionality here, this is the single object that you
    // will use to keep adding functionality, it's like the "base"
    // object so to speak.
    std::cout << "doing basic functionality and printing " << message << '\n';
  }
};

class notifier_decorator : public notifier
{
protected:
  std::unique_ptr<notifier> _notifier;

public:
  ~notifier_decorator () = default;

  notifier_decorator (std::unique_ptr<notifier> notifier)
    : _notifier {std::move (notifier)}
  {}

  virtual void
  send (std::string const& message) override
  {
    _notifier->send (message);
  }
};

class email_notifier_decorator final : public notifier_decorator
{
public:
  email_notifier_decorator (std::unique_ptr<notifier> notifier)
    : notifier_decorator (std::move (notifier))
  {}

  ~email_notifier_decorator () = default;

  void
  send (std::string const& message) override
  {
    _notifier->send (message);
    send_email_notification (message);
  }

private:
  void
  send_email_notification (std::string const& message)
  {
    std::cout << "doing email notification specific stuff...\n";
  }
};

class sms_notifier_decorator final : public notifier_decorator
{
public:
  sms_notifier_decorator (std::unique_ptr<notifier> notifier)
    : notifier_decorator (std::move (notifier))
  {}

  ~sms_notifier_decorator () = default;

  void
  send (std::string const& message) override
  {
    _notifier->send (message);
    send_sms_notification (message);
  }

private:
  void
  send_sms_notification (std::string const& message)
  {
    std::cout << "doing sms notification specific stuff...\n";
  }
};

class push_notifier_decorator final : public notifier_decorator
{
public:
  push_notifier_decorator (std::unique_ptr<notifier> notifier)
    : notifier_decorator (std::move (notifier))
  {}

  ~push_notifier_decorator () = default;

  void
  send (std::string const& message) override
  {
    _notifier->send (message);
    send_push_notification (message);
  }

private:
  void
  send_push_notification (std::string const& message)
  {
    std::cout << "doing push notification specific stuff...\n";
  }
};

int
main ()
{
  //
  // This is how it fucking works (it's a mess for the untrained brain).
  //

  {
    std::unique_ptr<notifier> notifier = std::make_unique<simple_notifier> ();

    notifier = std::make_unique<email_notifier_decorator> (std::move (notifier));
    notifier = std::make_unique<push_notifier_decorator> (std::move (notifier));
    notifier = std::make_unique<sms_notifier_decorator> (std::move (notifier));

    notifier->send ("Let's try this shit out!");
  }

  std::cout << '\n';

  {
    std::unique_ptr<notifier> notifier = std::make_unique<simple_notifier> ();

    notifier = std::make_unique<push_notifier_decorator> (std::move (notifier));
    notifier = std::make_unique<sms_notifier_decorator> (std::move (notifier));

    notifier->send ("Let's try this shit out 2.0!");
  }

  // NOTE: how do you undo this? can you? Apparently you can, but it's
  // a freaking mess. This pattern is not so good imho.

  return EXIT_SUCCESS;
}
