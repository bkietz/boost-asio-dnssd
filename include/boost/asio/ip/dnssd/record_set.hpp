#include <string>
#include <map>


namespace boost {
namespace asio {
namespace ip {
namespace dnssd {

class record_set;

class record
{
public:
enum type
{
  a         = 1,      // Host address.
  ns        = 2,      // Authoritative server.
  md        = 3,      // Mail destination.
  mf        = 4,      // Mail forwarder.
  cname     = 5,      // Canonical name.
  soa       = 6,      // Start of authority zone.
  mb        = 7,      // Mailbox domain name.
  mg        = 8,      // Mail group member.
  mr        = 9,      // Mail rename name.
  null      = 10,     // Null resource record.
  wks       = 11,     // Well known service.
  ptr       = 12,     // Domain name pointer.
  hinfo     = 13,     // Host information.
  minfo     = 14,     // Mailbox information.
  mx        = 15,     // Mail routing information.
  txt       = 16,     // One or more text strings.
  rp        = 17,     // Responsible person.
  afsdb     = 18,     // AFS cell database.
  x25       = 19,     // X_25 calling address.
  isdn      = 20,     // ISDN calling address.
  rt        = 21,     // Router.
  nsap      = 22,     // NSAP address.
  nsap_ptr  = 23,     // Reverse NSAP lookup (deprecated).
  sig       = 24,     // Security signature.
  key       = 25,     // Security key.
  px        = 26,     // X.400 mail mapping.
  gpos      = 27,     // Geographical position (withdrawn).
  aaaa      = 28,     // IPv6 Address.
  loc       = 29,     // Location Information.
  nxt       = 30,     // Next domain (security).
  eid       = 31,     // Endpoint identifier.
  nimloc    = 32,     // Nimrod Locator.
  srv       = 33,     // Server Selection.
  atma      = 34,     // ATM Address
  naptr     = 35,     // Naming Authority PoinTeR
  kx        = 36,     // Key Exchange
  cert      = 37,     // Certification record
  a6        = 38,     // IPv6 Address (deprecated)
  dname     = 39,     // Non-terminal DNAME (for IPv6)
  sink      = 40,     // Kitchen sink (experimentatl)
  opt       = 41,     // EDNS0 option (meta-RR)
  tkey      = 249,    // Transaction key
  tsig      = 250,    // Transaction signature.
  ixfr      = 251,    // Incremental zone transfer.
  axfr      = 252,    // Transfer zone of authority.
  mailb     = 253,    // Transfer mailbox records.
  maila     = 254,    // Transfer mail agent records.
  any       = 255     // Wildcard match.
};

  /// default to empty data
  record() {}

  explicit record(const std::string& d)
    : data_(d)
  {
  }

  std::string data() const { return data_; }

private:
  std::string data_;
};


class record_set
{
  std::map<record::type, record> records;

public:

  record_set()
  {
    records[record::srv];
    records[record::txt];
  }

  record_set& without(record::type t);
  record_set& with(record::type t,
      const std::string& data = "");
  record_set& with(record::type t);

  std::string get(record::type t);
};

/// remove a record from the record_set
record_set& record_set::without(record::type t)
{
  records.erase(t);
  return *this;
}

/// add a record to the record_set
record_set& record_set::with(record::type t,
    const std::string& data)
{
  records[t] = record(data);
  return *this;
}

/// default to adding an empty record
record_set& record_set::with(record::type t)
{
  records[t];
  return *this;
}

} // namespace dnssd
} // namespace ip
} // namespace asio
} // namespace boost
