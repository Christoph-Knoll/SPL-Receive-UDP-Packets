#include <iostream>
#include <boost/program_options/option.hpp>
#include <thread>
#include <boost/signals2/signal.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

#include "RoboCupGameControlData.h"

using boost::asio::ip::udp;

// Definitions
void handle_receive(void* pVoid){
    RoboCupGameControlData data;
    memcpy(&data, pVoid, sizeof(data));
    printf("Received Data\n");

    printf("Packet Number %d\n", data.packetNumber);
    printf("Seconds Remaining %d\n", data.secsRemaining);
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    printf("onLoad running");
    //Brain::getInstance();

    //Brain::getInstance()->addDispatcher(INTENTID_ON_BRAIN_TICK, BINDTHIS(GameControllerEngine::doThing));

    printf("Initialized Brain\n");
    boost::asio::io_service* _io_service = new boost::asio::io_service();


    printf("Initialized IO Service\n");

    void* pVoid = malloc(sizeof(RoboCupGameControlData));

    boost::asio::ip::udp::endpoint* _endpoint = new boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), GAMECONTROLLER_DATA_PORT);


    printf("Created UDP-Endpoint\n");
    boost::asio::ip::udp::socket* _socket = new boost::asio::ip::udp::socket(*_io_service, *_endpoint);


    boost::asio::socket_base::broadcast option(true);
    _socket->set_option(option);

    printf("Initialized Socket\n");


    boost::asio::ip::udp::endpoint* localEndpoint = new boost::asio::ip::udp::endpoint();

    _socket->async_receive_from(
            boost::asio::buffer(pVoid, sizeof(RoboCupGameControlData)), *localEndpoint,
            boost::bind(&handle_receive, /*nullptr,*/ pVoid /*,boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred*/));

    // Start IO Servicel
    std::thread serverThread(boost::bind(&boost::asio::io_service::run, _io_service));
    serverThread.detach();

    scanf("%s");
}