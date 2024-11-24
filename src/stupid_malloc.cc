#include <cstddef>
#include <cstdlib>
#include <cassert>

static void
my_align_malloc (size_t bytes, size_t alignment)
{}

static void
my_free (void *address)
{}

int
main ()
{
  return EXIT_SUCCESS;
}
