#include <string>

namespace boost {
namespace asio {
namespace ip {
namespace dnssd {

/// Stub class - minimal representation of a discovered service
// I may subsume update into stub...
class stub
{
  std::string name_;
  std::string type_;
  std::string domain_;
  int interface_index_;

public:
  stub(std::string name,
      std::string type,
      std::string domain,
      int interface_index = -1)
    : name_(name),
      type_(type),
      domain_(domain),
      interface_index_(interface_index)
  {
  }

  std::string get_name()   const { return name_; }
  std::string get_type()   const { return type_; }
  std::string get_domain() const { return domain_; }

};

} // namespace dnssd
} // namespace ip
} // namespace asio
} // namespace boost
