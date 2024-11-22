#include <cstdlib>
#include <future>
#include <iostream>
#include <cmath>

static int
add (int a, int b)
{
  return a + b;
}

static void
compute_square (std::promise<int> promise, int x)
{
  promise.set_value (x * x); // important: do not set_value twice bc it will blow up
}

static void
print_value (std::shared_future<int> future)
{
  std::cout << __PRETTY_FUNCTION__ << ": " << future.get () << '\n'; // future.get () blocks til the result is ready
}

static float
do_sqrt (int a)
{
  return static_cast<float> (std::sqrt (a));
}

int
main ()
{
  {
    // async: used to launch simple tasks
    auto future = std::async (std::launch::async, add, 6, 9);
    std::cout << "Result of async call -> " << future.get () << '\n';
  }
  {
    // promise is used to set a value that future will pick up
    std::promise<int> promise;
    std::future<int> future = promise.get_future ();
    std::thread t (compute_square, std::move (promise), 69);
    std::cout << "Square of future -> " << future.get () << '\n'; // future.get () blocks!
    t.join ();
  }
  {
    // shared_future is used to allow more than one thread to get the same future result
    std::promise<int> promise;
    std::shared_future<int> future = promise.get_future ();
    std::thread t1 (print_value, future), t2 (print_value, future);
    promise.set_value (69); // this shit sends a notif. to waiting threads
    t1.join ();
    t2.join ();
  }
  {
    // packaged task wraps a func and lets me run it async while also provinding a future to get
    // the result, ait
    std::packaged_task<float (int)> task (do_sqrt);
    std::future<float> future = task.get_future ();
    std::thread t (std::move (task), 31);
    std::cout << "Square root is -> " << future.get () << '\n';
    t.join ();
  }
  return EXIT_SUCCESS;
}
