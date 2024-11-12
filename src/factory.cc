#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>

class order
{
public:
  virtual ~order () = default;
  virtual void execute () = 0;
};

class market_order final : public order
{
public:
  ~market_order () = default;

  void
  execute () override
  {
    std::cout << "Executing market order...\n";
  }
};

class limit_order final : public order
{
public:
  ~limit_order () = default;

  void
  execute () override
  {
    std::cout << "Executing limit order...\n";
  }
};

class order_factory
{
public:
  virtual ~order_factory () = default;
  virtual std::unique_ptr<order> create_instance () = 0;
};

class market_order_factory final : public order_factory
{
public:
  ~market_order_factory () = default;

  std::unique_ptr<order>
  create_instance () override
  {
    // Logic to create the instance goes in here...
    return std::make_unique<market_order> ();
  }
};

class limit_order_factory final : public order_factory
{
public:
  ~limit_order_factory () = default;

  std::unique_ptr<order>
  create_instance () override
  {
    // Logic to create the instance goes in here...
    return std::make_unique<limit_order> ();
  }
};

class example final
{
public:
  void
  add_order (order_factory* factory)
  {
    _orders.emplace_back (factory->create_instance ());
  }

  void
  execute ()
  {
    for (auto const& order : _orders)
      order->execute ();
  }

private:
  std::vector<std::unique_ptr<order>> _orders;
};

int
main ()
{
  // I understand that the way this is coded, if you needed to add a
  // new product, you only need to create the new class and the new
  // factory and a new line here, that's all. You don't break existing
  // code either, which is cool.
  //
  // Also, the caller needs to know about factories implementations
  // beforehand, which in some cases I pressume it could be
  // problematic.
  //
  std::unique_ptr<order_factory> market = std::make_unique<market_order_factory> ();
  std::unique_ptr<order_factory> limit  = std::make_unique<limit_order_factory> ();

  example ex;

  ex.add_order (market.get ());
  ex.add_order (limit.get ());
  ex.add_order (market.get ());
  ex.add_order (limit.get ());

  ex.execute ();

  return EXIT_SUCCESS;
}