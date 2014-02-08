#include <boost/asio/detail/config.hpp>
#include <boost/asio/async_result.hpp>

// in .../impl/browser.hpp:
// #if defined( AVAHI )
// typedef avahi::browser browser_impl;
// #elif defined( BONJOUR )
// typedef bonjour::browser browser_impl;
// #else
// #error "No DNS-SD implementation found."
// #endif
#include <boost/asio/ip/dnssd/impl/browser.hpp>

#include <boost/asio/ip/dnssd/record_set.hpp>
#include <boost/asio/ip/dnssd/update.hpp>
#include <boost/asio/ip/dnssd/stub.hpp>
#include <boost/asio/ip/dnssd/unique_ptr.hpp>


namespace boost {
namespace asio {
namespace ip {
namespace dnssd {

class browser_service
  : public io_service::service
{
public:
  static io_service::id id;

  typedef browser_impl implementation_type;

  explicit browser_service(io_service& io)
  {
  }

  ~browser_service()
  {
  }

  void construct(implementation_type& impl)
  {
  }

  void destroy(implementation_type& impl)
  {
  }

  /// Wait for an browse update
  template <typename BrowseHandler>
  inline BOOST_ASIO_INITFN_RESULT_TYPE(BrowseHandler,
      void(boost::system::error_code, unique_ptr<update>))
  async_browse(implementation_type& impl,
      BOOST_ASIO_MOVE_ARG(BrowseHandler) handler)
  {
  }

  /// Start an asynchronous lookup
  template <typename LookupHandler>
  inline BOOST_ASIO_INITFN_RESULT_TYPE(LookupHandler,
      void(boost::system::error_code))
  async_lookup(implementation_type& impl,
      const stub& s, record_set& r,
      BOOST_ASIO_MOVE_ARG(LookupHandler) handler)
  {
  }
};


} // namespace dnssd
} // namespace ip
} // namespace asio
} // namespace boost

