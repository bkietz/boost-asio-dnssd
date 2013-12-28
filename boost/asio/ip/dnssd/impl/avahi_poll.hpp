/*
 * This adaptor works with the examples that ship with Avahi.
 * TODO: 
 *  - better error handling
 *  - research AVAHI_WATCH_OUT and the other watch events I never see
 *  - build guarantees that nothing here will throw
 *  - squirrel all of this into boost::asio::ip::dnssd::impl
 *  - remove C++11 dependency
 */
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <utility>


#include <avahi-common/watch.h>
#include <avahi-common/timeval.h>



struct AvahiWatch {
  typedef boost::asio::generic::stream_protocol::socket socket;
  typedef boost::asio::io_service io_service;

  socket socket_;
  AvahiWatchCallback callback_;

  // stored arguments for callback_
  // int fd;  // get with socket_.native_handle
  AvahiWatchEvent event_;
  void *userdata_;

  AvahiWatch(
    const AvahiPoll *api, 
    int fd, 
    AvahiWatchEvent event,
    AvahiWatchCallback callback,
    void *userdata
  ):  socket_(*static_cast<io_service *>(api->userdata)),
      callback_(callback),
      userdata_(userdata) {

    // Assign socket_ to monitor the provided descriptor.
    // The family and protocol shouldn't matter, since we
    // defer actual read/write to the library.
    socket_.assign(socket::protocol_type(0,0), fd);
    update(event);
  }

  void handler(boost::system::error_code ec) {
    if(ec) return;
    callback_(this, socket_.native_handle(), event_, userdata_);
    monitor(event_);
    event_ = (AvahiWatchEvent)0;
  }

  void monitor(AvahiWatchEvent events) {

    using boost::asio::null_buffers;
    using boost::system::error_code; 

    if(events & AVAHI_WATCH_IN)
    socket_.async_read_some(null_buffers(),
      [this] (error_code ec, size_t) { 
        event_ = AVAHI_WATCH_IN;
        handler(ec); 
      });

    if(events & AVAHI_WATCH_OUT)
    socket_.async_write_some(null_buffers(),
      [this] (error_code ec, size_t) { 
        event_ = AVAHI_WATCH_OUT;
        handler(ec); 
      });
  }

  AvahiWatchEvent get_events() { return event_; }

  void update(AvahiWatchEvent events) { 
    socket_.cancel();
    monitor(events);
  }
};




struct AvahiTimeout {
  typedef boost::asio::steady_timer timer;
  typedef boost::asio::io_service io_service;

  timer timer_;
  AvahiTimeoutCallback callback_;

  // stored argument for callback_
  void *userdata_;

  AvahiTimeout(
    const AvahiPoll *api,
    const struct timeval *tv,
    AvahiTimeoutCallback callback,
    void *userdata
  ):  timer_(*static_cast<io_service *>(api->userdata)),
      callback_(callback),
      userdata_(userdata) {

    update(tv);
  }

  void update(const struct timeval *tv) {
    if(!tv) {
      timer_.cancel();
      return;
    }

    timer_.expires_at( timer::time_point(
      std::chrono::seconds(tv->tv_sec) + 
      std::chrono::microseconds(tv->tv_usec)
    ));

    using boost::system::error_code; 
    timer_.async_wait(
      [this](error_code ec){
      if(ec) return;
      callback_(this, userdata_);
    });
  }
};






// This is a tiny ~vtable for the AvahiPoll class,
// which provides a dinky asynchronous interface 
// of just two operations:
//  -  watch a file descriptor
//  -  set a timer

// destructor
static void watch_free(AvahiWatch *w) { delete w; }
// examine an AvahiWatch to find out which event it is experiencing
static AvahiWatchEvent watch_get_events(AvahiWatch *w) {
  return w->get_events();
}
// set watched events
static void watch_update(AvahiWatch *w, AvahiWatchEvent events) {
  w->update(events);
}
// constructor
static AvahiWatch *watch_new(
  const AvahiPoll *api, 
  int fd, 
  AvahiWatchEvent event,
  AvahiWatchCallback callback,
  void *userdata) {

return new AvahiWatch(api, fd, event, callback, userdata);
}

// destructor
static void timeout_free(AvahiTimeout *t) { delete t; }
// set deadline
static void timeout_update(AvahiTimeout *t, const struct timeval *tv) {
  t->update(tv);
}
// constructor
static AvahiTimeout *timeout_new(
  const AvahiPoll *api,
  const struct timeval *tv,
  AvahiTimeoutCallback callback,
  void *userdata) {

return new AvahiTimeout(api, tv, callback, userdata);
}

// take an io_service object and provide an AvahiPoll 
// interface to it, as required by avahi_client_new()
// Whatever owns the AvahiClient must free this.
const AvahiPoll* avahi_io_service_poll_wrap(boost::asio::io_service& io) {
  return new AvahiPoll {
    &io, //userdata
    watch_new,
    watch_update,
    watch_get_events,
    watch_free,
    timeout_new,
    timeout_update,
    timeout_free
  };
};

