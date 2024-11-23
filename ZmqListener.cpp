#include "ZmqListener.h"


ZmqListener::ZmqListener(const QString &address, QObject* parent)
    : QObject(parent), context(1), socket(context, zmq::socket_type::sub) {
    socket.connect(address.toStdString());
    socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);  // Subscribe to all messages
  }

  ZmqListener::~ZmqListener() {
    socket.close();
  }

  void ZmqListener::startListening() {
    printf("Listening for messages\n");
    while (true) {
      zmq::message_t message;
      if (socket.recv(message, zmq::recv_flags::none)) {
        printf("Received message\n");
        QString data = QString::fromStdString(message.to_string());
        emit newMessageReceived(data);
      }
    }
  }

    
