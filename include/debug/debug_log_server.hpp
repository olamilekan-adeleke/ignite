#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <atomic>
#include <memory>

#define SERVER_TCP_ENDPOINT boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345)

// Define the type for the message handler callback.
// It now takes the socket, the received message, and the client's endpoint.
using MessageHandler =
    std::function<void(std::shared_ptr<boost::asio::ip::tcp::socket>, const std::string&, const std::string&)>;

class TCPServer {
 public:
  TCPServer() : io_context_(), acceptor_(io_context_, SERVER_TCP_ENDPOINT), running_(true) {}

  void setMessageHandler(MessageHandler handler) { message_handler_ = handler; }

  void start() {
    startAccept();
    ioThread_ = std::thread([this]() { io_context_.run(); });
    std::cout << "TCP Server running non-blocking on thread..." << std::endl;
  }

  void stop() {
    if (!running_) return;
    running_ = false;
    io_context_.stop();
    if (ioThread_.joinable()) ioThread_.join();
    std::cout << "TCP Server stopped." << std::endl;
  }

  ~TCPServer() { stop(); }

 private:
  void startAccept() {
    auto new_socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
    acceptor_.async_accept(*new_socket, [this, new_socket](std::error_code ec) {
      if (!running_) return;
      if (!ec) {
        std::cout << "Accepted new connection from " << new_socket->remote_endpoint() << std::endl;
        handleClient(new_socket);
      } else {
        std::cerr << "Accept error: " << ec.message() << std::endl;
      }
      startAccept();
    });
  }

  void handleClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    auto buffer = std::make_shared<std::vector<char>>(1024);
    socket->async_read_some(
        boost::asio::buffer(*buffer), [this, socket, buffer](std::error_code ec, size_t bytes_transferred) {
          if (!running_) return;
          if (!ec) {
            std::string message(buffer->data(), bytes_transferred);
            std::string client_endpoint_str = socket->remote_endpoint().address().to_string() + ":" +
                                              std::to_string(socket->remote_endpoint().port());

            // Call the user-defined handler with the socket
            if (message_handler_) {
              message_handler_(socket, message, client_endpoint_str);
            }

            // Continue reading from the same client
            handleClient(socket);
          } else if (ec.value() == boost::asio::error::eof || ec.value() == boost::asio::error::connection_reset) {
            std::cout << "Client " << socket->remote_endpoint() << " disconnected." << std::endl;
          } else {
            std::cerr << "Read error: " << ec.message() << std::endl;
          }
        });
  }

 private:
  boost::asio::io_context io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::thread ioThread_;
  std::atomic<bool> running_;
  MessageHandler message_handler_;
};
