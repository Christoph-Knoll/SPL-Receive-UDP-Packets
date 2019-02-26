//
// Created by obyoxar on 09/10/18.
//

#include <boost/signals2/signal.hpp>
#include <iostream>
#include "GameControllerEngine.h"
#include "../hightools/Dyllo.h"
#include "../core/Brain.h"
#include "../core/Logger.h"


// Definitions
#include "../core/definitions/RoboCupGameControlData.h"
#include "../core/definitions/SPLStandardMessage.h"

// Boost
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>


#define LOG_TAG LOGGER(GameControllerEngine)

using boost::asio::ip::udp;

void GameControllerEngine::onLoad() {
    LINFO("onLoad running");
    Brain::getInstance();

    Brain::getInstance()->addDispatcher(INTENTID_ON_BRAIN_TICK, BINDTHIS(GameControllerEngine::doThing));

    LFINFO("Initialized Brain");
    this->_io_service = new boost::asio::io_service();
    LFINFO("Initialized IO Service");
    void* pVoid = malloc(sizeof(RoboCupGameControlData));

    this->_endpoint = new udp::endpoint(boost::asio::ip::address_v4::broadcast()/*udp::v4()*/, GAMECONTROLLER_DATA_PORT);
    LFINFO("Created UDP-Endpoint");
    this->_socket = new udp::socket(*this->_io_service, *this->_endpoint);
    LINFO("Initialized Socket");



    this->_socket->async_receive_from(
            boost::asio::buffer(pVoid, sizeof(RoboCupGameControlData)), *this->_endpoint,
            boost::bind(&GameControllerEngine::handle_receive, this, pVoid/*boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred*/));

    // Start IO Service
    std::thread serverThread(boost::bind(&boost::asio::io_service::run, this->_io_service));
    serverThread.detach();




    LINFO("onLoad finished");
}

void GameControllerEngine::handle_receive(void* pVoid){
    RoboCupGameControlData* data = static_cast<RoboCupGameControlData*>(pVoid);
    LFINFO("Received Data");
    LFINFO(data->secsRemaining);
}

void GameControllerEngine::doThing(UNUSED void* data) {
    LDEBUG("Thing is done");
}

const char *GameControllerEngine::getName() {
    return "GameControllerEngine";
}

unsigned int GameControllerEngine::getEngineBaseVersion() {
    return ENGINE_BASE_VERSION;
}

DL_CLASS_PUBLISH(GameControllerEngine)
