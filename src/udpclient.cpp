#include "udpclient.h"

UdpClient::UdpClient()
{
}

UdpClient::~UdpClient()
{
}

bool UdpClient::setSocketBroadcastable()
{
    int optval = 1;
	int res = setsockopt(sock_, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
	if (res == -1)
    {
		perror("setsockopt");
		return false;
	}
    return true;
}

void UdpClient::setSockAddr(string ip, int port)
{
    memset(&sockAddr_, 0, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
	sockAddr_.sin_port = htons(port);
    sockAddr_.sin_addr.s_addr = inet_addr(ip.c_str());
    memset(&sockAddr_.sin_zero, 0, sizeof(sockAddr_.sin_zero));
}

void UdpClient::setSockAddrBroadcast(int port)
{
    //myIp or !netmask
    GRtmEntry* entry = GNetInfo::instance().rtm().getBestEntry(QString("8.8.8.8"));
    GIp broadcastip = entry->intf()->gateway() | ~(entry->intf()->mask());
    qInfo() << "gateway ip:" << QString(entry->intf()->gateway());
    qInfo() << "mask:" << QString(entry->intf()->mask());
    qInfo() << "broadcast ip:" << QString(broadcastip);
    setSockAddr(QString(broadcastip).toStdString(), port);
    return;
}

void UdpClient::setRecvTimeout(int sec, int millisec)
{
    struct timeval optVal = {sec, millisec}; //sec, millisec
    int optLen = sizeof(optVal);
    setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &optVal, optLen);
}
