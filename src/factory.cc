#include <cstdlib>
#include <memory>
#include <vector>
#include <cstdio>

// i fucking hate object oriented programming, i swear
struct order
{
  virtual void execute () = 0;
  virtual ~order () = default;
};

class market_order final : public order
{
public:
  market_order () = default;

  void execute () override { std::printf ("executing market order...\n"); }
};

class limit_order final : public order
{
public:
  ~limit_order () = default;

  void execute () override { std::printf ("executing limit order...\n"); }
};

struct order_factory
{
  virtual std::unique_ptr<order> create_order () = 0;
  virtual ~order_factory () = default;
};

struct market_order_factory : order_factory
{
  std::unique_ptr<order> create_order () override { return std::make_unique<market_order> (); }
};

struct limit_order_factory : order_factory
{
  std::unique_ptr<order> create_order () override { return std::make_unique<limit_order> (); }
};

class example final
{
public:
  void add_order (order_factory *factory) { _orders.emplace_back (factory->create_order ()); }

  void execute_orders ()
  {
    for (auto &order : _orders)
      {
        order->execute ();
      }
  }

private:
  std::vector<std::unique_ptr<order>> _orders;
};

int
main ()
{
  auto market = std::make_unique<market_order_factory> ();
  auto limit = std::make_unique<limit_order_factory> ();
  example ex;
  ex.add_order (market.get ());
  ex.add_order (limit.get ());
  ex.add_order (market.get ());
  ex.add_order (limit.get ());
  ex.execute_orders ();
  return EXIT_SUCCESS;
}
