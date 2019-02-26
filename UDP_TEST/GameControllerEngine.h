//
// Created by obyoxar on 09/10/18.
//

#ifndef DUCKBURG_EXAMPLEENGINE_H
#define DUCKBURG_EXAMPLEENGINE_H

#include "../core/EngineBase.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

class GameControllerEngine : public EngineBase {
public:
    const char* getName() override;

    void doThing(void* data);

    void onLoad() override;

    void handle_receive(void* pVoid);

    unsigned int getEngineBaseVersion() override;

private:
    int count = 0;

    boost::asio::io_service* _io_service = nullptr;

    boost::asio::ip::udp::socket* _socket = nullptr;

    boost::asio::ip::udp::endpoint* _endpoint = nullptr;

};



#endif //DUCKBURG_EXAMPLEENGINE_H
