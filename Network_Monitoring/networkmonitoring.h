#ifndef NETMONITORING_H
#define NETMONITORING_H
#include "monitoring.grpc.pb.h"
#include <QObject>
#include <QString>
#include <google/protobuf/empty.pb.h>
#include <grpc++/grpc++.h>

using grpc::Channel;
using mon::NetworkMonitoring;
class networkMonitoring : public QObject
{
	Q_OBJECT

public:
	networkMonitoring(const std::string &ep);
	~networkMonitoring();
	mon::AllIfaceResponses getBytesPerSecond();
	mon::IfaceResponse getIfaceBytesPerSecond(std::string &packetName);
	mon::AllIfaceResponses getPacketsPerSecond();
	mon::IfaceResponse getIfacePacketsPerSecond(std::string &packetName);

	networkMonitoring(QObject *parent_ = nullptr);

private:
	void createChannel(const std::string &ep);
	std::unique_ptr<NetworkMonitoring::Stub> stub = nullptr;
	std::string endpoint;
	// std::string const name;
};
#endif // NETMONITORING_H
