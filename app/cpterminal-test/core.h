#pragma once
#include "tcpblock.h"
#include "appjson.h"
struct Core : Obj
{
    Core();
    ~Core(){};
    struct InfectionList : std::list<Flow> { //for infection
        std::mutex m_;
    } infectionList_;

    struct TimeSet : std::set<Flow> { //for check time
        std::mutex m_;
    } timeSet_;

    std::string redirectpage_ = "http://wifievent.io";
    ArpSpoof arpspoof_;
    TcpBlock tcpblock_;
    PcapDevice writer_;
    PcapCapture capturer_;
    Packet* packet_;

    int infectionTime = 15; //15 seconds
    int reinfectionTime = 3600; // 60 minutes
    int sendInfectionTime = 30000; // 3 seconds
    int sendInterval_ = 10000; // 1 seconds

    Ip host_;
    bool active = true;
    std::mutex Mutex_;
    std::thread* readPacket_;
    std::thread* infectHost_;
    std::thread* checkTime_;
    std::condition_variable myCv_;

    void start();
    bool sendArpInfectAll();
    void removeFlows(Flow sender);
    void prepare();
    void captured(Packet *packet);
    void read();
    void stop();
    void infect();
    void checkForInfection();//check infection time
    //void load(Json::Value& json) override;
    //void save(Json::Value& json) override;
};


