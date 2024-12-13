#ifndef ZMQLISTENER_H
#define ZMQLISTENER_H

#include <QObject>
#include <QString>
#include <zmq.hpp>

class ZmqListener : public QObject {
    Q_OBJECT

public:
    /**
     * Constructor for ZmqListener
     * @param address - The ZeroMQ address to subscribe to (e.g., "tcp://localhost:5555")
     * @param parent - The parent QObject
     */
    explicit ZmqListener(const QString &address, QObject *parent = nullptr);

    /**
     * Destructor
     */
    ~ZmqListener();

    /**
     * Starts listening for messages on the ZeroMQ subscriber socket.
     * Emits newMessageReceived(QString) when a message is received.
     */
public slots:
    void startListening();

signals:
    /**
     * Signal emitted when a new message is received.
     * @param message - The message received from the ZMQ socket
     */
    void gpsMessageReceived(const QString &message);

private:
    zmq::context_t context;  
    zmq::socket_t socket;    
};

#endif // ZMQLISTENER_H