#include <cstdlib>
#include <cassert>
#include <iostream>
#include <memory>
#include <mutex>

// Meyers' singleton
class audio_manager final
{
public:
  audio_manager  (audio_manager& other) = delete;
  void operator= (audio_manager& other) = delete;

  audio_manager  (audio_manager&& other) = delete;
  void operator= (audio_manager&& other) = delete;

  static audio_manager&
  get_instance ()
  {
    static audio_manager manager;
    return manager;
  }

  bool
  operator== (audio_manager const& other) const
  {
    return this == &other;
  }

private:
  audio_manager () = default;
};

int
main ()
{
  auto& instance_test1 = audio_manager::get_instance ();
  auto& instance_test2 = audio_manager::get_instance ();

  assert (instance_test1 == instance_test2);

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
