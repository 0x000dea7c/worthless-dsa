#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <vector>
#include <memory>

// This design pattern uses an interface that indicates the
// functionality or strategy that has to be implemented.  It assumes
// that there are multiple ways of solving the problem, and it also
// assumes that the client knows the best option at each time.
//
// Finally, it assumes it's okay to use dynamic dispatch. I mean, all
// object oriented code assumes that to be honest... (big assumption?)
//
// Here's an example with sorting.
//
// NOTE: why don't you just use a function pointer? I think this pattern
// also assumes that you're working exclusively under an object oriented
// design?
//

class sorting_strategy
{
public:
  virtual ~sorting_strategy () = default;
  virtual void sort (std::vector<int32_t>&) = 0;
};

class bubble_sort final : public sorting_strategy
{
public:
  ~bubble_sort () = default;

  void
  sort (std::vector<int32_t>& data) override
  {
    std::cout << "sorting using bubble sort...\n";
  }
};

class merge_sort final : public sorting_strategy
{
public:
  ~merge_sort () = default;

  void
  sort (std::vector<int32_t>& data) override
  {
    std::cout << "sorting using merge sort...\n";
  }
};

class radix_sort final : public sorting_strategy
{
public:
  ~radix_sort () = default;

  void
  sort (std::vector<int32_t>& data) override
  {
    std::cout << "sorting using radix sort...\n";
  }
};

static void
do_stuff (std::vector<int32_t>& data, sorting_strategy* strategy)
{
  std::cout << "About to sort this mf\n";
  strategy->sort (data);
  std::cout << "sorted\n";
}

int
main ()
{
  std::vector<int32_t> data {5, 4, 3, 2, 1};

  std::unique_ptr<sorting_strategy> bubblesort = std::make_unique<bubble_sort> ();
  std::unique_ptr<sorting_strategy> mergesort = std::make_unique<merge_sort> ();
  std::unique_ptr<sorting_strategy> radixsort = std::make_unique<radix_sort> ();

  do_stuff (data, bubblesort.get ());
  do_stuff (data, mergesort.get ());
  do_stuff (data, radixsort.get ());

  return EXIT_SUCCESS;
}
