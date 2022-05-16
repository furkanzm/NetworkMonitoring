#include "networkmonitoring.h"
#include <QDebug>
#include <QThread>

typedef unsigned long long int ull;

networkMonitoring::networkMonitoring(const std::string &ep)
{
	createChannel(ep);
	while (true) {
		getBytesPerSecond();
		QThread::msleep(1600);
	}
}

void networkMonitoring::createChannel(const std::string &ep)
{
	using grpc::Channel;
	std::shared_ptr<Channel> channel;
	channel = grpc::CreateChannel(ep, grpc::InsecureChannelCredentials());
	stub = mon::NetworkMonitoring::NewStub(channel);
}

mon::AllIfaceResponses networkMonitoring::getBytesPerSecond()
{
	google::protobuf::Empty empty;
	grpc::ClientContext context;
	grpc::Status statusResponse;
	mon::AllIfaceResponses networkCtrl;
	mon::AllIfaceResponses AllIfaceResponses;
	if (!stub)
		return networkCtrl;
	std::mutex tempNetwork;
	std::lock_guard<std::mutex> lockNetwork(tempNetwork);
	statusResponse =
		stub->GetBytesPerSecond(&context, empty, &AllIfaceResponses);
	int size = AllIfaceResponses.iface_size();
	for (int i = 0; i < size; i++) {
		std::string ifacename = AllIfaceResponses.iface(i).name();
		qDebug() << "\n"
				 << "Iface packet name: " << QString::fromStdString(ifacename);
		getIfaceBytesPerSecond(ifacename);
	}
	qDebug() << "#########################################################";
	return AllIfaceResponses;
}

mon::IfaceResponse
networkMonitoring::getIfaceBytesPerSecond(std::string &packetName)
{
	grpc::Status statusNetwork;
	grpc::ClientContext contextNetwork;
	mon::IfaceResponse ifaceresp;
	mon::IfaceResponse IfaceRespCtrl;
	mon::IfaceResponse IfaceResponse;
	mon::IfaceQ IfaceQ;
	*IfaceQ.mutable_name() = packetName;
	//*IfaceResponse.mutable_value() = value;
	statusNetwork =
		stub->GetIfaceBytesPerSecond(&contextNetwork, IfaceQ, &IfaceResponse);
	/*qDebug() << IfaceResponse.value().ByteSizeLong();*/
	qDebug() << " Iface bytes:" << IfaceResponse.value().r();
	if (!stub)
		return IfaceRespCtrl;
	return IfaceResponse;
}

mon::AllIfaceResponses networkMonitoring::getPacketsPerSecond()
{
	google::protobuf::Empty empty;
	grpc::ClientContext context;
	grpc::Status statusResponse;
	mon::AllIfaceResponses AllIfaceResponses;
	mon::AllIfaceResponses networkCtrl;
	if (!stub)
		return networkCtrl;
	std::mutex tempNetwork;
	std::lock_guard<std::mutex> lockNetwork(tempNetwork);
	statusResponse =
		stub->GetPacketsPerSecond(&context, empty, &AllIfaceResponses);
	return AllIfaceResponses;
}

mon::IfaceResponse
networkMonitoring::getIfacePacketsPerSecond(std::string &packetName)
{
	grpc::Status statusNetwork;
	grpc::ClientContext contextNetwork;
	mon::IfaceResponse IfaceResponse;
	mon::IfaceResponse IfaceRespCtrl;
	mon::IfaceQ IfaceQ;
	*IfaceResponse.mutable_name() = packetName;
	statusNetwork =
		stub->GetIfacePacketsPerSecond(&contextNetwork, IfaceQ, &IfaceResponse);

	return IfaceResponse;
}

networkMonitoring::~networkMonitoring()
{
}
