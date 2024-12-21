// the goal of this program is to create a arena allocator that I will use
// in my next projects

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstdio>

using U0 = void;
using U8 = std::uint8_t;
using I32 = std::int32_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;
using F32 = float;

constexpr F32 PI = 3.14159265;

#define internal static

inline internal U0
prove (bool expr, char const* msg)
{
  if (!expr)
    {
      std::fprintf(stderr, "%s\n", msg);
      *(int *) 0 = 0; // blow the fuck up!
    }
}

inline internal constexpr U32
kilobytes (U32 n)
{
  return n * 1024ull;
}

inline internal constexpr U32
megabytes (U32 n)
{
  return kilobytes (n) * 1024ull;
}

inline internal constexpr U32
gigabytes (U32 n)
{
  return megabytes (n) * 1024ull;
}

inline internal constexpr bool
is_power_of_2 (U32 n)
{
  return (n & (n - 1)) == 0;
}

struct arena
{
  arena (U8 *buff, U64 l, U64 o) : buffer{buff}, len{l}, offset{o} {}
  U8 *buffer;
  U64 len;
  U64 offset;
};

[[nodiscard]] internal std::uintptr_t
align_forward (std::uintptr_t ptr, std::size_t alignment)
{
  prove (is_power_of_2 (alignment), "My alignment isn't a power of 2");
  std::uintptr_t p, a, mod;
  p = ptr;
  a = alignment;
  mod = p & (a - 1); // this is the same as p % a, but faster bc % is very expensive
  if (mod != 0)      // if a isn't a factor of p then it's not aligned, so align it
    p += a - mod;    // it's nothing special, just modulo arithmetic, try it with some numbers if I don't remember
  return p;
}

[[nodiscard]] U8 *
arena_alloc (arena &a, std::size_t size, std::size_t alignment = 2 * sizeof (U0 *)) // in this machine: 16 bytes
{
  // I want to align bc computers want to access memory locations in powers of 2 since the size of a word is a
  // power of 2. If I don't align memory then what could have been a single read could degrade to two reads.
  std::uintptr_t p = (std::uintptr_t) a.buffer + (std::uintptr_t) a.offset;
  std::uintptr_t rel_offset = align_forward (p, alignment) - (std::uintptr_t) a.buffer;
  if (rel_offset + size <= a.len)
    {
      a.offset = rel_offset;
      auto *ptr = &a.buffer[a.offset];
      a.offset += size;
      std::memset (ptr, 0, size);
      return ptr;
    }
  return nullptr;
}

U0
arena_free (arena &a)
{
  a.offset = 0;
}

U0
store_str (char* dst, char const *src, std::size_t chunk_size)
{
  auto len = std::strlen(src) + 1;
  prove (chunk_size >= len, "I don't have enough space to store your str");
  std::strncpy(dst, src, len);
}

int
main ()
{
  auto size = megabytes (2);
  auto *buff = (U8 *) mmap (nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (!buff)
    {
      perror ("couldn't allocate any memory");
      return 1;
    }

  arena my_arena{buff, size, 0};
  char *str = (char *) arena_alloc (my_arena, kilobytes (32));
  std::strncpy (str, "Hello World", 12);
  __builtin_printf ("Your string is: %s\n", str);
  arena_free (my_arena);

  auto chunk_size = 23;
  str = (char *) arena_alloc (my_arena, chunk_size);
  store_str (str, "Is this going to work?", chunk_size);
  __builtin_printf ("Your new string is: %s\n", str);

  auto const n = 32;
  auto const array_size = 4 * n;     // store 32 elements
  F32 *array = (F32 *) arena_alloc(my_arena, array_size);
  prove(array, "oops, couldn't allocate space for my array");
  for (U32 i = 0; i < n; ++i)
    array[i] = PI + i * 2;
  for (U32 i = 0; i < n; ++i)
    __builtin_printf ("array[%u] = %f\n", i, array[i]);

  if (munmap (my_arena.buffer, my_arena.len) == -1)
    perror ("couldn't deallocate memory");

  return 0;
}
