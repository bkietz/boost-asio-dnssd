#include <boost/asio/detail/config.hpp>
#include <boost/asio/async_result.hpp>

#include <boost/asio/ip/dnssd/dnssd_service.hpp>
#include <boost/asio/ip/dnssd/record_set.hpp>


namespace boost {
namespace asio {
namespace ip {
namespace dnssd {

template <typename RegistrationService>
class basic_registration
  : public basic_io_object<RegistrationService>
{
public:

  explicit basic_registration(io_service& io,
      std::string name)
			// eventually, this will have to be wstring or  
			// something - name is UTF-8   [dns-sd.h : 883]
    : basic_io_object<RegistrationService>(io)
  {
  }

  /// Commit a registration
  template <typename CommitHandler>
  inline BOOST_ASIO_INITFN_RESULT_TYPE(CommitHandler,
      void(boost::system::error_code))
  async_commit(BOOST_ASIO_MOVE_ARG(CommitHandler) handler)
  {
    return this->get_service().async_commit(
      this->get_implementation(),
      BOOST_ASIO_MOVE_CAST(CommitHandler)(handler));
  }
};

typedef basic_registration<dnssd_service> registration;

} // namespace dnssd
} // namespace ip
} // namespace asio
} // namespace boost