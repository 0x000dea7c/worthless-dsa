#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>

//
// In this behavioural design pattern you have a subject (the class
// that contains information that can change) and observers (classes
// that are interested in these changes and need to react to them).
//
// NOTE: instead of having an observer interface, you can just have a
// vector of function pointers inside the subclass that implements the
// subject interface and invoke them every time values change. I think
// the observer interface is more informational, I think. Measure
// speed.
//
class observer
{
public:
  virtual ~observer () = default;
  virtual void update () = 0;
};

class subject
{
public:
  virtual ~subject () = default;
  // NOTE: notice you're passing them by value because you want the
  // reference count to be incremented!!! (or so I think!)
  virtual void add_observer (std::shared_ptr<observer>) = 0;
  virtual void remove_observer (std::shared_ptr<observer>) = 0;
  virtual void notify_observers () const = 0;
};

class weather_lcd_display final : public observer
{
public:
  ~weather_lcd_display () = default;

  void
  update () override
  {
    std::cout << "updating values in weather lcd display.\n";
  }
};

class weather_desktop_app final : public observer
{
public:
  ~weather_desktop_app () = default;

  void
  update () override
  {
    std::cout << "updating values in weather desktop app.\n";
  }
};

//
// Random example. The weather station reads information about the
// current weather and if something changes it will invoke the
// notify_observers function.
//
class weather_station final : public subject
{
public:
  ~weather_station () = default;

  void
  add_observer (std::shared_ptr<observer> o) override
  {
    _observers.push_back (o);
  }

  void
  remove_observer (std::shared_ptr<observer> o) override
  {
    std::erase_if (_observers, [o] (std::shared_ptr<observer> arg) { return o == arg; });
  }

  void
  notify_observers () const override
  {
    std::lock_guard<std::mutex> guard (_mutex);
    for (auto observer : _observers)
      observer->update ();
  }

private:
  mutable std::mutex _mutex;	// mutable is used bc you're
				// locking/unlocking with lock_guard
				// in a const member function, and you
				// cannot do it without the `mutable`
				// keyword!
  std::vector<std::shared_ptr<observer>> _observers;
};

int
main ()
{
  std::unique_ptr<subject> station = std::make_unique<weather_station> ();

  auto lcd = std::make_shared<weather_lcd_display> ();
  auto desktop = std::make_shared<weather_desktop_app> ();

  station->add_observer (lcd);
  station->add_observer (desktop);

  station->notify_observers ();

  station->remove_observer (desktop);

  station->notify_observers ();

  return EXIT_SUCCESS;
}
