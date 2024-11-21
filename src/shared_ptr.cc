#include <cassert>
#include <cstdlib>
#include <atomic>
#include <cstring>
#include <iostream>

template <typename T> class shared_pointer
{
  T *_ptr;
  std::atomic<size_t> *_counter;

  void release ()
  {
    if (_counter && _counter->fetch_sub (1) == 1)
      {
        delete _ptr;
        delete _counter;
      }
  }

public:
  shared_pointer () : _ptr{nullptr}, _counter{nullptr} {}

  explicit shared_pointer (T *ptr) : _ptr{ptr}
  {
    if (_ptr)
      {
        _counter = new std::atomic<size_t> (1);
      }
  }

  // copy constructor
  shared_pointer (shared_pointer const &other) : _ptr{other._ptr}, _counter{other._counter}
  {
    if (_counter)
      {
        _counter->fetch_add (1);
      }
  }

  // move constructor
  shared_pointer (shared_pointer &&other) : _ptr{other._ptr}, _counter{other._counter}
  {
    other._ptr = nullptr;
    other._counter = nullptr;
  }

  // copy assignment
  shared_pointer &operator= (shared_pointer const &other)
  {
    if (this != other)
      {
        release (); // release our shit bc it doesn't matter now
        _ptr = other.ptr;
        _counter = other._counter;
        if (_counter)
          {
            _counter->fetch_add (1);
          }
      }
    return *this;
  }

  // move assignment
  shared_pointer &operator= (shared_pointer &&other)
  {
    if (this != other)
      {
        release (); // free ur res mf
        _ptr = other._ptr;
        _counter = other._counter;
        other._ptr = nullptr;
        other._counter = nullptr;
      }
    return *this;
  }

  ~shared_pointer () { release (); }

  T *get () { return _ptr; }

  T &operator* () { return *_ptr; }

  T *operator->() { return _ptr; }

  uint32_t use_count () const { return (_counter) ? _counter->load () : 0; }
};

// template specification, if you pass T[] use this template pls
template <typename T> class shared_pointer<T[]>
{
  T *_ptr;
  std::atomic<size_t> *_counter;

  void release ()
  {
    if (_counter && _counter->fetch_sub (1) == 1)
      {
        delete[] _ptr;
        delete _counter;
      }
  }

public:
  shared_pointer () : _ptr{nullptr}, _counter{nullptr} {}

  explicit shared_pointer (T *ptr) : _ptr{ptr}
  {
    if (_ptr)
      {
        _counter = new std::atomic<size_t> (1);
      }
  }

  // copy constructor
  shared_pointer (shared_pointer const &other) : _ptr{other._ptr}, _counter{other._counter}
  {
    if (_counter)
      {
        _counter->fetch_add (1);
      }
  }

  // move constructor
  shared_pointer (shared_pointer &&other) : _ptr{other._ptr}, _counter{other._counter}
  {
    other._ptr = nullptr;
    other._counter = nullptr;
  }

  // copy assignment
  shared_pointer &operator= (shared_pointer const &other)
  {
    if (this != other)
      {
        release (); // release our shit bc it doesn't matter now
        _ptr = other.ptr;
        _counter = other._counter;
        if (_counter)
          {
            _counter->fetch_add (1);
          }
      }
    return *this;
  }

  // move assignment
  shared_pointer &operator= (shared_pointer &&other)
  {
    if (this != other)
      {
        release (); // free ur res mf
        _ptr = other._ptr;
        _counter = other._counter;
        other._ptr = nullptr;
        other._counter = nullptr;
      }
    return *this;
  }

  ~shared_pointer () { release (); }

  T *get () { return _ptr; }

  T &operator* () { return *_ptr; }

  T *operator->() { return _ptr; }

  T &operator[] (uint32_t i) { return _ptr[i]; }

  uint32_t use_count () const { return (_counter) ? _counter->load () : 0; }
};

int
main ()
{
  {
    shared_pointer<int[]> my_dumb_ptr (new int[20]);
    for (uint32_t i = 0; i < 20; ++i)
      {
        my_dumb_ptr[i] = i;
      }
    for (uint32_t i = 0; i < 20; ++i)
      {
        std::cout << my_dumb_ptr[i] << '\n';
      }
  }
  {
    char copy[] = "what are you doing u bastard, this is gonna halt and catch fire";
    char *str = new char[63 + 1];
    std::strncpy (str, copy, 63);
    str[63] = '\0';
    shared_pointer<char[]> my_dumb_ptr (str);
    auto second = my_dumb_ptr;
    auto third = second;
    std::cout << second.get () << '\n';
    std::cout << second.use_count () << '\n';
  }
  return EXIT_SUCCESS;
}
