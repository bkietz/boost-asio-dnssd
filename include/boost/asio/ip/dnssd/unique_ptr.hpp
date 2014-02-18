#include <boost/config.hpp>
#ifdef BOOST_NO_CXX11_SMART_PTR


namespace boost {
namespace asio {
namespace ip {
namespace dnssd {

/// If std::unique_ptr is unavailable, provide a replacement.
template <typename T>
class unique_ptr
{
//TODO
};

} // namespace dnssd
} // namespace ip
} // namespace asio
} // namespace boost


#else // BOOST_NO_CXX11_SMART_PTR


#include <memory>
namespace boost {
namespace asio {
namespace ip {
namespace dnssd {

/// If std::unique_ptr is available, use that.
using ::std::unique_ptr;

} // namespace dnssd
} // namespace ip
} // namespace asio
} // namespace boost


#endif // BOOST_NO_CXX11_SMART_PTR
