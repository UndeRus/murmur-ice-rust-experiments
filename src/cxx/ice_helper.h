// File: src/cxx/ice_helper.h
#pragma once

#include <memory>
#include <string>
#include <Ice/Ice.h>
#include "MumbleServer.h" // Include the generated MumbleServer header file
#include "rust/cxx.h"

struct MumbleServerInstance;

class MetaProxy {
    public:
        MetaProxy(MumbleServer::MetaPrx& metaProxy);

        virtual ~MetaProxy() {}

        std::unique_ptr<std::vector<MumbleServerInstance>> get_servers() const;

        void setCallback() const;
    private:
        MumbleServer::MetaPrx metaProxy;

};

// Wrapper class for the Ice Communicator
class Communicator
{
public:
    Communicator()
    {
        int argc = 0;
        char *argv[] = {};

        auto c = Ice::initialize(argc, argv);
        communicator = c; // std::make_shared<Ice::Communicator>();
    }

    // Destructor to properly shut down the communicator
    virtual ~Communicator()
    {
        if (communicator)
        {
            std::cout << "Destroying communicator" << std::endl;
            communicator->destroy();
        }
    }

    std::unique_ptr<MetaProxy> connect() const
    {
        try
        {
            auto base = communicator->stringToProxy("Meta:tcp -h localhost -p 6502 -t 60000"); // TODO: extract this to params
            MumbleServer::MetaPrx metaProxy = MumbleServer::MetaPrx::checkedCast(base);
            auto aaa = std::unique_ptr<MetaProxy>(new MetaProxy(metaProxy));
            return aaa;
        }
        catch (Ice::ConnectionRefusedException ex)
        {
            std::cout << "Failed to connect" << std::endl;
            return nullptr;
        }
    }




    // Accessor for the internal communicator
    /*
    std::shared_ptr<Ice::Communicator> get() const {
        return communicator;
    }
    */

private:
    Ice::CommunicatorPtr communicator;
};

std::unique_ptr<Communicator> new_communicator();
