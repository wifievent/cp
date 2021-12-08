#include "core.h"

//after infection
void Core::removeFlows(Flow sender) { //sender == not gateway
    {
        std::lock_guard<std::mutex> lock(infectionList_.m_);
        arpspoof_.sendRecover(sender);
        for(std::list<Flow>::iterator iter = infectionList_.begin(); iter!= infectionList_.end(); iter++) {
            if(iter->ip_ == sender.ip_){
                infectionList_.erase(iter);
                break;
            }
        }
    }
}

bool Core::sendArpInfectAll() {
    {
        std::lock_guard<std::mutex> lock(infectionList_.m_);
        for (Flow& flow: infectionList_) {
            if (!arpspoof_.sendInfect(flow))
                return false;
            spdlog::info("Core::sendInfect");
            std::this_thread::sleep_for(std::chrono::milliseconds(sendInterval_));
        }
    }
    return true;
}

bool Core::captured(Packet *packet)
{
    EthHdr* ethHdr = packet->ethHdr_;
    if (ethHdr == nullptr) {
        spdlog::critical("ethHdr is null");
        return false;
    }
    if (ethHdr->type() != EthHdr::Ip4)
        return false;

    IpHdr* ipHdr = packet->ipHdr_;
    if (ipHdr == nullptr) {
        spdlog::critical("ipHdr is null");
        return false;
    }

    //mypacket
    if(ipHdr->sip() == intf()->ip())
        return false;

    //not tcp
    if(ipHdr->p() != IpHdr::TCP)
        return false;

    TcpHdr* tcpHdr = packet->tcpHdr_;
    if (tcpHdr == nullptr) {
        spdlog::critical("tcpHdr is null");
        return false;
    }

    spdlog::info("<<<Tcp>>>");
    if (tcpHdr->dport() == 443) {
        spdlog::info("<<<443>>>");
        //block
        if (ipHdr->dip() != host_)
            return true;
        return false;
    }

    if (tcpHdr->dport() == 80) {
        spdlog::info("<<<80>>>");
        Buf tcpData = packet->tcpData_;

        if(!tcpData.valid())//not http
            return false;

        const char* castedtcpdata = reinterpret_cast<const char*>(tcpData.data_);

        if(strncmp(castedtcpdata, "GET ", 4) == 0 && ipHdr->dip() != host_) {
            spdlog::info("Send redirect page data to client");
            tcpblock_.block(packet);
            return true;
        } else if(strncmp(castedtcpdata, "POST ", 5) == 0) {
            std::string api = "infected=false";
            std::string tcpdata = castedtcpdata;
            auto it = std::search(tcpdata.begin(), tcpdata.end(), std::boyer_moore_searcher(api.begin(), api.end()));

            if (it != tcpdata.end()) {
                spdlog::info("infection off"+std::string(ipHdr->sip()));
                removeFlows(Flow(ethHdr->smac(), ipHdr->sip()));
            } else if(it == tcpdata.end()) {
                spdlog::info("infection keep");
                return true;
            }
        }
    }
    return false;
}

void Core::prepare()
{
    active = true;
    bool res = arpspoof_.open();
    if (!res) return;
    spdlog::info("Core::arpspoof open");
    tcpblock_.writer_ = this;
    res = tcpblock_.open();
    if (!res) return;
    spdlog::info("Core::tcpblock open");

    intfName_ = arpspoof_.intfName_;
    res = open();
    if (!res) return;
    spdlog::info("Core::pcapdevice open");

    arpspoof_.prepare();
    spdlog::info("arpspoof prepare");

    tcpblock_.forwardRst_ = true;
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
    for(i = 0; i < (int)domain.size(); i++) {
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


void Core::checkForInfection()
{
    struct timeval now;
    active = true;
    while(active) {
        std::unique_lock<std::mutex> lock(Mutex_);
        if(myCv_.wait_for(lock,std::chrono::milliseconds(sendInfectionTime)) == std::cv_status::no_timeout) break;

        //check infection time
        for(std::list<Flow>::iterator iter_l = infectionList_.begin(); iter_l != infectionList_.end(); iter_l++) {
            gettimeofday(&now, NULL);
            char ch[100];
            sprintf(ch,"%d",(int)(now.tv_sec - iter_l->lastAccess_.tv_sec));
            //spdlog::info(ch);
            if((now.tv_sec - iter_l->lastAccess_.tv_sec) == infectionTime) {
                removeFlows(*iter_l);//include recover
            }
        }

        //check reinfection time
        for(std::set<Flow>::iterator iter_s = timeSet_.begin(); iter_s != timeSet_.end(); iter_s++){
            gettimeofday(&now, NULL);

            //because of not to modify
            if((now.tv_sec-iter_s->lastAccess_.tv_sec) == reinfectionTime) {
                infectionList_.push_back(*iter_s);
            }
        }
    }
}

void Core::readPacket() {
    active = true;
    while (active) {
        Packet::Result res =read(&packet_);
        if (res == Packet::None) continue;
        if (res == Packet::Eof || res == Packet::Fail) break;

        //find host
        Flow host = arpspoof_.detect(&packet_);
        if(host.mac_ != Mac::nullMac() &&host.ip_ != Ip::nullIp() && host.mac_ != arpspoof_.gwMac_&&timeSet_.find(host) == timeSet_.end()) {
            timeSet_.insert(host);
            infectionList_.push_back(host);
            spdlog::info("<<<Core::infectionList push>>> " + std::string(host.mac_)+" "+ std::string(host.ip_));
        }

        //tcpblock
        if(captured(&packet_)) {//true -> block
            drop(&packet_);
        }else{ //false -> relay
            arpspoof_.relay(&packet_);
        }
    }
}

void Core::infect() {
    active = true;
    while (active) {
        if(infectionList_.size() != 0){
        sendArpInfectAll();
        spdlog::info("Core::sendArpInfectAll");
        std::unique_lock<std::mutex> lock(Mutex_);
        if(myCv_.wait_for(lock,std::chrono::milliseconds(sendInfectionTime)) == std::cv_status::no_timeout) break;
        }
    }
}

Core::Core(){
    Json::Value jv;
    if(AppJson::loadFromFile("captiveportal.json",jv)){
        load(jv);
    }
    save(jv);
    AppJson::saveToFile("captiveportal.json",jv);
}

void Core::start() {
    prepare();
    spdlog::info("prepare finish");
    readPacket_ = new std::thread(&Core::readPacket,this);
    spdlog::info("readPacket finish");
    infectHost_ = new std::thread(&Core::infect,this);
    spdlog::info("infectHost finish");
    checkTime_ = new std::thread(&Core::checkForInfection,this);
    spdlog::info("checkTime finish");
}

void Core::stop() {
    active = false;
    myCv_.notify_all();
    readPacket_->join();
    infectHost_->join();
    checkTime_->join();
}

void Core::load(Json::Value& json) {
    json["infectionTime"] >> infectionTime;
    json["reinfectionTime"] >> reinfectionTime;
    json["sendInfectionTime"] >> sendInfectionTime;
    json["sendInterval_"] >> sendInterval_;
}

void Core::save(Json::Value& json) {
    json["infectionTime"] << infectionTime;
    json["reinfectionTime"] << reinfectionTime;
    json["sendInfectionTime"] << sendInfectionTime;
    json["sendInterval_"] << sendInterval_;
}
