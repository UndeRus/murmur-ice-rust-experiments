#include <Ice/Ice.h>
#include "MumbleServer.h" // Include the generated MumbleServer header file
#include "murmur-ice-rust-experiments/src/cxx/ice_helper.h"
#include "murmur-ice-rust-experiments/src/main.rs.h"

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
	}

	return std::unique_ptr<std::vector<MumbleServerInstance>>(result);
}