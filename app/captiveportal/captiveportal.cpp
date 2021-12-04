#include "captiveportal.h"
CaptivePortal::CaptivePortal(QWidget *parent)
{
    tcpblock_.backwardRst_ = false;
    tcpblock_.backwardFin_ = true;
	tcpblock_.writer_ = &writer_;
}

void CaptivePortal::setComponent()
{
    capturer_.intfName_ = intfname_;
    tcpblock_.backwardFinMsg_ = "HTTP/1.1 302 Found\r\nLocation: "+redirectpage_+"\r\n\r\n";
}

bool CaptivePortal::doOpen()
{
    std::string url = redirectpage_;
    struct addrinfo *servinfo;
    struct addrinfo hints;
    char host[16];

    std::string domain;

    if(url.substr(0,7) == "http://") {
        domain = url.substr(7);
    }else if(url.substr(0,8) == "https://") {
        domain = url.substr(8);
    }
    int i = 0;
    for(i = 0; i < domain.size(); i++) {
        if(domain[i] == '/')
            break;
    }
    domain = domain.substr(0,i);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(domain.c_str(), NULL, &hints, &servinfo))
    {
        spdlog::info("failed to get host ip");
        exit(1);
    }

    getnameinfo(servinfo->ai_addr,
                servinfo->ai_addrlen,
                host, sizeof(host),
                NULL,
                0,
                NI_NUMERICHOST);

    std::string ip = std::string(host);
    host_ = Ip(ip);
    spdlog::info("domain="+redirectpage_+",ip="+string(host_));

    setComponent();

    if(!(writer_.open()))
    {
        spdlog::info("failed to open writer");
        return false;
    }
    if(!(tcpblock_.open()))
    {
        spdlog::info("failed to open tcpblock");
        return false;
    }
    if(!(capturer_.open()))
    {
        spdlog::info("failed to open arpspoof");
        return false;
    }

    //receive packet
    recv_ = std::thread(&Capture::run, &capInstance, packet);
    return true;
}

bool CaptivePortal::doClose()
{
    if(!(writer_.close()))
    {
        spdlog::info("failed to close writer");
        return false;
    }
    if(!(tcpblock_.close()))
    {
        spdlog::info("failed to close tcpblock");
        return false;
    }
    if(!(capturer_.close()))
    {
        spdlog::info("failed to close arpspoof");
        return false;
    }
    return true;
}
/*
void CaptivePortal::propLoad(QJsonObject jo)
{
    GProp::propLoad(jo);
    jo["capturer"] >> capturer_;
    jo["tcpblock"] >> tcpblock_;
    jo["writer"] >> writer_;
}

void CaptivePortal::propSave(QJsonObject &jo)
{
    GProp::propSave(jo);
    jo["capturer"] << capturer_;
    jo["tcpblock"] << tcpblock_;
    jo["writer"] << writer_;
}*/

void CaptivePortal::processPacket(Packet *packet)
{
    EthHdr* ethHdr = packet->ethHdr_;
	if (ethHdr == nullptr) {
        spdlog::critical("ethHdr is null");
		return;
	}

    if (ethHdr->type() != EthHdr::Ip4)
        return;

    IpHdr* ipHdr = packet->ipHdr_;
	if (ipHdr == nullptr) {
        spdlog::critical("ipHdr is null");
		return;
	}

    if(ipHdr->p() != IpHdr::TCP)
        return;

    TcpHdr* tcpHdr = packet->tcpHdr_;

	if (tcpHdr == nullptr) {
        spdlog::critical("tcpHdr is null");
		return;
	}

	if (tcpHdr->dport() == 443)
    {
		if (ipHdr->dip() != host_)
			packet->ctrl.block_ = true;
        return;
    }

	if (tcpHdr->dport() == 80)
    {
        Buf tcpData = packet->tcpData_;
		if(!tcpData.valid())
			return;

		const char* castedtcpdata = reinterpret_cast<const char*>(tcpData.data_);
		if(strncmp(castedtcpdata, "GET ", 4) == 0 && ipHdr->dip() != host_)
        {
            spdlog::info("Send redirect page data to client");
            tcpblock_.block(packet);
        }
		if(strncmp(castedtcpdata, "POST ", 5) == 0)
        {
            std::string api = "infected=false";
            std::string tcpdata = castedtcpdata;
			auto it = std::search(tcpdata.begin(), tcpdata.end(), std::boyer_moore_searcher(api.begin(), api.end()));
            if (it != tcpdata.end())
            {
                spdlog::info("infection off"+std::string(ipHdr->sip()));
                capturer_.removeFlows(Flow(ethHdr->smac(), ipHdr->sip()));
            }
            else if(it == tcpdata.end())
            {
                spdlog::info("infection keep");
                return;
            }
        }
    }
}
