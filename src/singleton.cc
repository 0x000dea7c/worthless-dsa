#include <cstdlib>
#include <cassert>
#include <iostream>

// Meyers' singleton (since c++11 initialisation is thread safe)
class audio_manager
{
  audio_manager () = default;

public:
  audio_manager (audio_manager &) = delete;
  audio_manager (audio_manager &&) = delete;

  bool operator= (audio_manager const &) = delete;
  bool operator= (audio_manager &&) = delete;

  bool operator== (audio_manager const &other) const { return this == &other; }

  static audio_manager &get_instance ()
  {
    static audio_manager instance;
    return instance;
  }
};

int
main ()
{
  auto &instance_test1 = audio_manager::get_instance ();
  auto &instance_test2 = audio_manager::get_instance ();

  assert (instance_test1 == instance_test2);

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
