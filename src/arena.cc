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
using U8 = unsigned char;
using I32 = std::int32_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

#define internal static

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

struct arena
{
  U8 *buffer;
  U64 len;
  U64 offset;
};

[[nodiscard]] internal U8 *
arena_alloc (arena &a, std::size_t size)
{
  if (a.offset + size <= a.len)
    {
      // TODO: NOT ALIGNED: problem!
      auto *ptr = &a.buffer[a.offset];
      a.offset += size;
      std::memset (ptr, 0, size);
      return ptr;
    }
  return nullptr;
}

int
main ()
{
  arena my_arena;
  my_arena.len = megabytes (2);
  my_arena.offset = 0;
  my_arena.buffer = (U8 *) mmap (nullptr, my_arena.len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (!my_arena.buffer)
    {
      perror ("couldn't allocate any memory");
      return 1;
    }

  // TODO: this is also very hacky!
  I32 *buff = (I32 *) arena_alloc (my_arena, megabytes (1));

  for (U32 i = 0; i < 32; ++i)
    buff[i] = i + 1;

  for (U32 i = 0; i < 32; ++i)
    __builtin_printf ("%d ", buff[i]);

  __builtin_printf ("\n");

  if (munmap (my_arena.buffer, my_arena.len) == -1)
    perror ("couldn't deallocate memory");

  return 0;
}
