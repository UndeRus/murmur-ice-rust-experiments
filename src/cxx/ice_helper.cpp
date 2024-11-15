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

	auto servers = metaProxy->getAllServers();
	auto result = std::vector<MumbleServerInstance>(1);


	auto iter{servers.begin()};

	while (iter != servers.end()) // пока не дойдем до конца
	{
		std::cout << *iter << std::endl; // получаем элементы через итератор
		// result.push_back(MumbleServerInstance { })

		MumbleServerInstance instance{
			id: 100
		};
		//instance.id = 100;//iter->get()->getConf();
		result.push_back(instance);
		++iter; // перемещаемся вперед на один элемент
	}

	std::cout << "Result size" << result.size() << std::endl;

	return std::unique_ptr<std::vector<MumbleServerInstance>>(&result);
}