#include <boost/asio/detail/config.hpp>
#include <boost/asio/async_result.hpp>

#include <boost/asio/ip/dnssd/browser_service.hpp>
#include <boost/asio/ip/dnssd/record_set.hpp>
#include <boost/asio/ip/dnssd/update.hpp>
#include <boost/asio/ip/dnssd/stub.hpp>
#include <boost/asio/ip/dnssd/unique_ptr.hpp>


namespace boost {
namespace asio {
namespace ip {
namespace dnssd {

template <typename BrowserService>
class basic_browser
  : public basic_io_object<BrowserService>
{
public:

  explicit basic_browser(io_service& io,
      std::string service_type,
      std::string domain = "local")
    : basic_io_object<BrowserService>(io)
  {
    // call start_op here
  }

  /// Wait for an browse update
  template <typename BrowseHandler>
  inline BOOST_ASIO_INITFN_RESULT_TYPE(BrowseHandler,
      void(boost::system::error_code, unique_ptr<update>))
  async_browse(BOOST_ASIO_MOVE_ARG(BrowseHandler) handler)
  {
    return this->get_service().async_browse(
      this->get_implementation(),
      BOOST_ASIO_MOVE_CAST(BrowseHandler)(handler));
  }

  /// Start an asynchronous lookup
  template <typename LookupHandler>
  inline BOOST_ASIO_INITFN_RESULT_TYPE(LookupHandler,
      void(boost::system::error_code))
  async_lookup(const stub& s, record_set& r,
      BOOST_ASIO_MOVE_ARG(LookupHandler) handler)
  {
    return this->get_service().async_lookup(
      this->get_implementation(), s, r,
      BOOST_ASIO_MOVE_CAST(LookupHandler)(handler));
  }
};

typedef basic_browser<browser_service> browser;

} // namespace dnssd
} // namespace ip
} // namespace asio
} // namespace boost
