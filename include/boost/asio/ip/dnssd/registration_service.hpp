#include <boost/asio/detail/config.hpp>
#include <boost/asio/async_result.hpp>

// in .../impl/registration.hpp:
// #if defined( AVAHI )
// typedef avahi::registration registration_impl;
// #elif defined( BONJOUR )
// typedef bonjour::registration registration_impl;
// #else
// #error "No DNS-SD implementation found."
// #endif
#include <boost/asio/ip/dnssd/impl/registration.hpp>

#include <boost/asio/ip/dnssd/record_set.hpp>
#include <boost/asio/ip/dnssd/update.hpp>
#include <boost/asio/ip/dnssd/stub.hpp>
#include <boost/asio/ip/dnssd/unique_ptr.hpp>


namespace boost {
namespace asio {
namespace ip {
namespace dnssd {

class registration_service
  : public io_service::service
{
public:
  static io_service::id id;

  typedef registration_impl implementation_type;

  explicit registration_service(io_service& io)
  {
  }

  ~registration_service()
  {
  }

  void construct(implementation_type& impl)
  {
  }

  void destroy(implementation_type& impl)
  {
  }

  /// Commit a registration
  template <typename CommitHandler>
  inline BOOST_ASIO_INITFN_RESULT_TYPE(CommitHandler,
      void(boost::system::error_code))
  async_commit(implementation_type& impl,
      BOOST_ASIO_MOVE_ARG(CommitHandler) handler)
  {
  }
};


} // namespace dnssd
} // namespace ip
} // namespace asio
} // namespace boost

