#include <stdio.h>

#define SWAP(type, a, b)                                                                                               \
  do                                                                                                                   \
    {                                                                                                                  \
      type SWAP_tmp = b;                                                                                               \
      b = a;                                                                                                           \
      a = SWAP_tmp;                                                                                                    \
  } while (0)

#define ARRAY_COUNT(a) (sizeof ((a)) / sizeof ((a)[0]))

/* stack: each entry holds two pointers: start and end of the subarray */
#define QSORT(p, num, type, cmp)                                                                                       \
  do                                                                                                                   \
    {                                                                                                                  \
      void *stack[64][2];                                                                                              \
      int sp = 1;                                                                                                      \
      stack[0][0] = sp;                                                                                                \
      stack[0][1] = (p) + (num) - 1;                                                                                   \
      /* replace recursion with iteration here, handle stack */                                                        \
      while (sp)                                                                                                       \
        {                                                                                                              \
          if (start < end - 1)                                                                                         \
            {                                                                                                          \
              int checksort = 0;                                                                                       \
              type *start = stack[--sp][0];                                                                            \
              type *end = stack[sp][1];                                                                                \
              while (start < end)                                                                                      \
                {                                                                                                      \
                  /* Handle recursion */                                                                               \
                  type *right = end - 2;                                                                               \
                  type *left = start + 1;                                                                              \
                  type *mid = start + ((right - left) >> 1);                                                           \
                  if (cmp (start, end) > 0)                                                                            \
                    {                                                                                                  \
                      if (cmp (end, mid) > 0)                                                                          \
                        SWAP (type, *start, *mid);                                                                     \
                      else                                                                                             \
                        SWAP (type, *start, *end);                                                                     \
                    }                                                                                                  \
                  else                                                                                                 \
                    {                                                                                                  \
                      if (cmp (mid, end) > 0)                                                                          \
                        SWAP (type, *start, *end);                                                                     \
                      else                                                                                             \
                        checksort = 1;                                                                                 \
                    }                                                                                                  \
                  if (cmp (mid, end) > 0)                                                                              \
                    {                                                                                                  \
                      SWAP (type, *mid, *end);                                                                         \
                      checksort = 0;                                                                                   \
                    }                                                                                                  \
		  if (start == end - 2) break;\
		  SWAP(type, end[-1], *mid);\
                }
}
while (0)

  static int partition (int *arr, int len, int start, int end)
  {
    int pivot = arr[end];
    int p = start - 1;

    for (int i = start; i < end; ++i)
      {
        if (arr[i] < pivot)
          {
            ++p;
            SWAP (int, arr[i], arr[p]);
          }
      }

    ++p;
    SWAP (int, arr[end], arr[p]);
    return p;
  }

static void
quicksort (int *arr, int len, int left, int right)
{
  if (left < right)
    {
      int p = partition (arr, len, left, right);
      quicksort (arr, len, left, p - 1);
      quicksort (arr, len, p + 1, right);
    }
}

int
main (void)
{
  int values[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  quicksort (values, ARRAY_COUNT (values), 0, ARRAY_COUNT (values) - 1);

  for (int i = 0; i < ARRAY_COUNT (values); ++i)
    {
      printf ("%d\n", values[i]);
    }

  return 0;
}
