#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>

// this is called the "Product"
class order
{
public:
  virtual ~order () = default;

  virtual void execute () = 0;
};

class market_order : public order
{
public:
  ~market_order () = default;

  void
  execute () override
  {
    std::cout << "executing market order...\n";
  }
};

class limit_order : public order
{
public:
  ~limit_order () = default;

  void
  execute () override
  {
    std::cout << "executing limit order...\n";
  }
};

// the "Factory"

class order_factory
{
public:
  virtual ~order_factory () = default;

  virtual std::unique_ptr<order> create_order () const = 0;
};

class market_order_factory : public order_factory
{
public:
  virtual ~market_order_factory () = default;

  std::unique_ptr<order>
  create_order () const
  {
    return std::make_unique<market_order> ();
  }
};

class limit_order_factory : public order_factory
{
public:
  virtual ~limit_order_factory () = default;

  std::unique_ptr<order>
  create_order () const
  {
    return std::make_unique<limit_order> ();
  }
};

class example final
{
public:
  void
  add_order (order_factory* factory)
  {
    // I think the point here is that the "cl13nt c0d3" doesn't have
    // to know about which order type you're dealing with...  The
    // assumption then is that in this particular piece of code you
    // don't care about your data because they all have to behave the
    // same way?
    _trash.emplace_back (factory->create_order ());
  }

  void
  add_order_without_factory (std::string const& arg)
  {
    // In here you'd be in a situation where you need to do multiple
    // if statements and most likely some sort of prelude in order to
    // create instances of your products. This can be annoying if you
    // expect to have multiple products or to add/delete them (how can
    // this even happen, though? bad specs?)  Also, if creating
    // instance sof market_order and limit_order is expensive, having
    // make_unique's or naked new all over the codebase is bad. If you
    // were to centralise the creation of such instances, you could
    // implement object pooling? maybe? I don't know...
    if (arg == "condition_for_market")
      {
	_trash.emplace_back (std::make_unique<market_order> ());
      }
    else if (arg == "condition_for_limit")
      {
	_trash.emplace_back (std::make_unique<limit_order> ());
      }
  }

  void
  execute ()
  {
    // I understand the value of polymorphism here.
    for (auto const& o : _trash)
      o->execute ();
  }

private:
  std::vector<std::unique_ptr<order>> _trash;
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
