#include <gtest/gtest.h>

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-client/publish.h>
#include <avahi-common/error.h>
#include <avahi-common/malloc.h>
#include <avahi-common/alternative.h>

#include <boost/lockfree/spsc_queue.hpp>
#include <string>
#include <sstream>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include "boost/asio/ip/dnssd/impl/avahi_poll.hpp"
using boost::system::error_code;
using std::string;
using std::stringstream;
using std::cout; using std::endl; using std::cerr;

boost::lockfree::spsc_queue<string> found_queue(1024);

/// This is a test with callbacks lifted (almost) directly from the
/// [Avahi examples](http://avahi.org/download/doxygen/examples.html)
/// as a basic test of the implementation of AvahiPoll.

static void create_services(AvahiClient *c);
static AvahiEntryGroup *group = NULL;
static char *name = "Hello World";

static void browse_callback(
    AvahiServiceBrowser *b,
    AvahiIfIndex interface,
    AvahiProtocol protocol,
    AvahiBrowserEvent event,
    const char *name,
    const char *type,
    const char *domain,
    AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
    void* userdata) {

    auto c = static_cast<AvahiClient *>(userdata);
    assert(b);

    /* Called whenever a new services becomes available on the LAN or is removed from the LAN */
    stringstream found;

    switch (event) {
        case AVAHI_BROWSER_FAILURE:
            fprintf(stderr, "(Browser) %s\n", avahi_strerror(avahi_client_errno(avahi_service_browser_get_client(b))));
            return;

        case AVAHI_BROWSER_NEW:
	    found << name << '.' << type << '.' << domain << '.';
	    found_queue.push(found.str());
	    cerr << found.str() << endl;
            fprintf(stderr, "(Browser) NEW: service '%s' of type '%s' in domain '%s'\n", name, type, domain);
            break;

        case AVAHI_BROWSER_REMOVE:
            fprintf(stderr, "(Browser) REMOVE: service '%s' of type '%s' in domain '%s'\n", name, type, domain);
            break;

        case AVAHI_BROWSER_ALL_FOR_NOW:
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            fprintf(stderr, "(Browser) %s\n", event == AVAHI_BROWSER_CACHE_EXHAUSTED ? "CACHE_EXHAUSTED" : "ALL_FOR_NOW");
            break;
    }
}

static void entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, AVAHI_GCC_UNUSED void *userdata) {
    assert(g == group || group == NULL);
    group = g;

    /* Called whenever the entry group state changes */

    switch (state) {
        case AVAHI_ENTRY_GROUP_ESTABLISHED :
            /* The entry group has been established successfully */
            fprintf(stderr, "Service '%s' successfully established.\n", name);
            break;

        case AVAHI_ENTRY_GROUP_COLLISION : {
            char *n;

            /* A service name collision with a remote service
             * happened. Let's pick a new name */
            n = avahi_alternative_service_name(name);
            avahi_free(name);
            name = n;

            fprintf(stderr, "Service name collision, renaming service to '%s'\n", name);

            /* And recreate the services */
            create_services(avahi_entry_group_get_client(g));
            break;
        }

        case AVAHI_ENTRY_GROUP_FAILURE :
            fprintf(stderr, "Entry group failure: %s\n", avahi_strerror(avahi_client_errno(avahi_entry_group_get_client(g))));
            break;

        case AVAHI_ENTRY_GROUP_UNCOMMITED:
        case AVAHI_ENTRY_GROUP_REGISTERING:
            ;
    }
}

static void create_services(AvahiClient *c) {
    char *n, r[128];
    int ret;
    assert(c);

    /* If this is the first time we're called, let's create a new
     * entry group if necessary */

    if (!group)
        if (!(group = avahi_entry_group_new(c, entry_group_callback, NULL))) {
            fprintf(stderr, "avahi_entry_group_new() failed: %s\n", avahi_strerror(avahi_client_errno(c)));
        }

    /* If the group is empty (either because it was just created, or
     * because it was reset previously, add our entries.  */

    if (avahi_entry_group_is_empty(group)) {
        fprintf(stderr, "Adding service '%s'\n", name);

        /* Create some random TXT data */
        snprintf(r, sizeof(r), "random=%i", rand());

        /* Add the service for IPP */
        if ((ret = avahi_entry_group_add_service(group, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, (AvahiPublishFlags)0, name, "_http._tcp", NULL, NULL, 651, "test=blah", r, NULL)) < 0) {

            if (ret == AVAHI_ERR_COLLISION)
                goto collision;

            fprintf(stderr, "Failed to add _http._tcp service: %s\n", avahi_strerror(ret));
        }

        /* Tell the server to register the service */
        if ((ret = avahi_entry_group_commit(group)) < 0) {
            fprintf(stderr, "Failed to commit entry group: %s\n", avahi_strerror(ret));
        }
    }

    return;

collision:

    /* A service name collision with a local service happened. Let's
     * pick a new name */
    n = avahi_alternative_service_name(name);
    avahi_free(name);
    name = n;

    fprintf(stderr, "Service name collision, renaming service to '%s'\n", name);

    avahi_entry_group_reset(group);

    create_services(c);
    return;
}

static void client_callback(AvahiClient *c, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata) {
    assert(c);

    /* Called whenever the client or server state changes */

    switch (state) {
        case AVAHI_CLIENT_S_RUNNING:

            /* The server has startup successfully and registered its host
             * name on the network, so it's time to create our services */
            create_services(c);
            break;

        case AVAHI_CLIENT_FAILURE:

            fprintf(stderr, "Client failure: %s\n", avahi_strerror(avahi_client_errno(c)));

            break;

        case AVAHI_CLIENT_S_COLLISION:

            /* Let's drop our registered services. When the server is back
             * in AVAHI_SERVER_RUNNING state we will register them
             * again with the new host name. */

        case AVAHI_CLIENT_S_REGISTERING:

            /* The server records are now being established. This
             * might be caused by a host name change. We need to wait
             * for our own records to register until the host name is
             * properly esatblished. */

            if (group)
                avahi_entry_group_reset(group);

            break;

        case AVAHI_CLIENT_CONNECTING:
            ;
    }
}

TEST (AvahiRegisterBrowse, AvahiTests) {

  boost::asio::io_service io;

  int error;
  AvahiClient *client = avahi_client_new(avahi_io_service_poll_wrap(io), 
    (AvahiClientFlags)0, client_callback, 0, &error);

  AvahiServiceBrowser *browser = avahi_service_browser_new(client, 
    AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, "_http._tcp", 0, 
    (AvahiLookupFlags)0, browse_callback, client);

  boost::asio::deadline_timer timer(io);
  timer.expires_from_now(boost::posix_time::seconds(2));
  timer.async_wait([&](const error_code& ec){ io.stop(); });

  io.run();

  stringstream ours;
  ours << name << '.' << "_http._tcp" << '.' << "local" << '.';
  string popped;
  while(found_queue.pop(popped)) {
	  if(popped == ours.str()) {
		  SUCCEED() << "We found the service we published ourselves";
		  return;
	  }
  }
  FAIL() << "The service we published was not discovered...";

}

int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
