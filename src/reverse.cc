#include <cassert>
#include <cstdlib>
#include <iostream>
#include <cstring>

void
reverse (char *str)
{
  auto const n = std::strlen (str);
  for (size_t i = 0; i < n / 2; ++i)
    {
      std::swap (str[i], str[n - i - 1]);
    }
}

int
main ()
{
  {
    char str[] = "hello";
    reverse (str);
    assert (std::strcmp (str, "olleh") == 0);
  }
  {
    char str[] = "";
    reverse (str);
    assert (std::strcmp (str, "") == 0);
  }
  {
    char str[] = "innocent";
    reverse (str);
    assert (std::strcmp (str, "tneconni") == 0);
  }
  {
    char str[] = "bruh";
    reverse (str);
    assert (std::strcmp (str, "hurb") == 0);
  }
  {
    char str[] = "kek";
    reverse (str);
    assert (std::strcmp (str, "kek") == 0);
  }
  {
    char str[] = "accelerate husbant";
    reverse (str);
    assert (std::strcmp (str, "tnabsuh etarelecca") == 0);
  }
  {
    char str[] = "a";
    reverse (str);
    assert (std::strcmp (str, "a") == 0);
  }
  {
    char str[] = "ab";
    reverse (str);
    assert (std::strcmp (str, "ba") == 0);
  }
  {
    char str[] = "abc";
    reverse (str);
    assert (std::strcmp (str, "cba") == 0);
  }
  std::cout << "All tests passed!\n";
  return EXIT_SUCCESS;
}
