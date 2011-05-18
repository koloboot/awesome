//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef AWESOME_CONNECTION_HPP
#define AWESOME_CONNECTION_HPP

#include <boost/array.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace awesome {

using boost::asio::ip::tcp;

// Represents a single connection from a client.
class connection
  : public boost::enable_shared_from_this<connection>,
    private boost::noncopyable
{
public:
  // Construct a connection with the given io_service.
  explicit connection(boost::asio::io_service& io_service);

  // Get the downstream socket associated with the connection.
  tcp::socket& down_socket();

  // Kick off the asynchronous operations associated with the connection.
  void start(const tcp::endpoint& up_endpoint);

private:
  // Cancel all asynchronous operations associated with the connection.
  void stop();

  // Determine whether the connection has been stopped.
  bool is_stopped() const;

  // Handle completion of connection to the server.
  void handle_connect(const boost::system::error_code& ec);

  // Handle completion of a transfer operation. Called only on error.
  void handle_transfer();

  // The socket used for communicating with the client.
  tcp::socket down_socket_;

  // Buffer for data to be forwarded to the client.
  boost::array<unsigned char, 1024> down_buffer_;

  // The socket used for communicating with the server.
  tcp::socket up_socket_;

  // Buffer for data received from the client.
  boost::array<unsigned char, 1024> up_buffer_;
};

typedef boost::shared_ptr<connection> connection_ptr;

} // namespace awesome

#endif // AWESOME_CONNECTION_HPP