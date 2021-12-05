#include "core.h"
void Core::captured(Packet *packet)
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
                arpspoof_.removeFlows(Flow(ethHdr->smac(), ipHdr->sip()));
            }
            else if(it == tcpdata.end())
            {
                spdlog::info("infection keep");
                return;
            }
        }
    }
}

void Core::prepare() {
    arpspoof_.open();
    tcpblock_.open();

    tcpblock_.backwardRst_ = false;
    tcpblock_.backwardFin_ = true;
    tcpblock_.writer_ = &writer_;
    tcpblock_.backwardFinMsg_ = "HTTP/1.1 302 Found\r\nLocation: "+redirectpage_+"\r\n\r\n";

    struct addrinfo *servinfo;
    struct addrinfo hints;
    char host[16];

    std::string domain;

    if(redirectpage_.substr(0,7) == "http://") {
        domain = redirectpage_.substr(7);
    } else if(redirectpage_.substr(0,8) == "https://") {
        domain = redirectpage_.substr(8);
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
    if(getaddrinfo(domain.c_str(), NULL, &hints, &servinfo)) {
        spdlog::info("failed to get host ip");
        exit(1);
    }

    getnameinfo(servinfo->ai_addr,
                servinfo->ai_addrlen,
                host, sizeof(host),
                NULL,
                0,
                NI_NUMERICHOST);

    host_ = Ip(std::string(host));
    spdlog::info("domain="+redirectpage_+",ip="+string(host_));
}

void Core::checkForReInfection(){
    std::set<Flow>::iterator iter;
    struct timeval now;
    for(iter = arpspoof_.timeSet_.begin(); iter != arpspoof_.timeSet_.end(); iter++){
        gettimeofday(&now, NULL);

        //because of not to modify
        if((now.tv_sec-iter->lastAccess_.tv_sec) % reinfectionTime == 0) {
            arpspoof_.infectionList_.push_back(*iter);
        }
    }

}

void Core::checkForInfection(){
    std::list<Flow>::iterator iter;
    struct timeval now;
    for(iter = arpspoof_.infectionList_.begin(); iter != arpspoof_.infectionList_.end(); iter++) {
        gettimeofday(&now, NULL);

        if((now.tv_sec-iter->lastAccess_.tv_sec)%infectionTime == 0) {
            arpspoof_.removeFlows(*iter);
        }
    }

}
void Core::read() {
    Packet* packet;

    while (active) {
        Packet::Result res = capturer_.read(packet);
        if (res == Packet::None) continue;
        if (res == Packet::Eof || res == Packet::Fail) break;
        arpspoof_.detect(packet);
        arpspoof_.sendArpInfectAll();
    }
}

Core::Core(){
    prepare();
    readPacket_ = new std::thread(&Core::read,this);

}
void Core::load(Json::Value& json) {
    json["infectionTime"] >> infectionTime;
    json["reinfectionTime"] >> reinfectionTime;
}

void Core::save(Json::Value& json) {
    json["infectionTime"] << infectionTime;
    json["reinfectionTime"] << reinfectionTime;
}
