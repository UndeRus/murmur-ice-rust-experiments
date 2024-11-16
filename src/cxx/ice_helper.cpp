#include <Ice/Ice.h>
#include "MumbleServer.h" // Include the generated MumbleServer header file
#include "murmur-ice-rust-experiments/src/cxx/ice_helper.h"
#include "murmur-ice-rust-experiments/src/main.rs.h"


class Callback : public IceProxy::MumbleServer::ServerCallback
{
public:
	Callback() : IceProxy::MumbleServer::ServerCallback() {};

	virtual ~Callback() {}

	virtual void userConnected(const MumbleServer::User &state, const ::Ice::Current &current = ::Ice::emptyCurrent)
	{
	}

	virtual void userDisconnected(const MumbleServer::User &state, const ::Ice::Current &current = ::Ice::emptyCurrent)
	{
	}

	virtual void userStateChanged(const MumbleServer::User &state, const ::Ice::Current &current = ::Ice::emptyCurrent)
	{
	}

	virtual void userTextMessage(const MumbleServer::User &state, const MumbleServer::TextMessage &message, const ::Ice::Current &current = ::Ice::emptyCurrent)
	{
		std::cout << "New message" << std::endl;
	}

	virtual void channelCreated(const MumbleServer::Channel &state, const ::Ice::Current &current = ::Ice::emptyCurrent)
	{
	}

	virtual void channelRemoved(const MumbleServer::Channel &state, const ::Ice::Current &current = ::Ice::emptyCurrent)
	{
	}

	virtual void channelStateChanged(const MumbleServer::Channel &state, const ::Ice::Current &current = ::Ice::emptyCurrent)
	{
	}
};

MetaProxy::MetaProxy(MumbleServer::MetaPrx &metaProxy)
{
	this->metaProxy = metaProxy;
}

std::unique_ptr<Communicator> new_communicator()
{
	return std::unique_ptr<Communicator>(new Communicator());
}

std::unique_ptr<std::vector<MumbleServerInstance>> MetaProxy::get_servers() const
{
	auto callback = new Callback();
	//TODO: this proxy failed with IceUtil::NullHandleException
	auto callbackProxy = new MumbleServer::ServerCallbackPrx(callback);

	auto defaultConf = metaProxy->getDefaultConf();
	auto servers = metaProxy->getAllServers();
	auto result = new std::vector<MumbleServerInstance>();
	for (auto iter = servers.begin(); iter != servers.end(); ++iter)
	{
		auto server = iter->get();
		auto localConf = server->getAllConf();
		auto conf = defaultConf;
		conf.insert(localConf.begin(), localConf.end());

		MumbleServerInstance instance{
			.id = static_cast<uint8_t>(server->id()),
			.port = static_cast<uint16_t>(std::stoi(conf["port"])),
		};
		result->push_back(instance);
		server->addCallback(*callbackProxy);
	}

	return std::unique_ptr<std::vector<MumbleServerInstance>>(result);
}

void MetaProxy::setCallback() const
{
}
