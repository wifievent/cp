#pragma once
#include "tcpblock.h"
#include "arpspoof.h"
#include "appjson.h"
struct Core : PcapDevice
{
    Core(){};
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
    EthPacket packet_;

    int infectionTime = 15000; //15 seconds
    int reinfectionTime = 3600; // 60 minutes
    int sendInfectionTime = 1000; // 1 seconds
    int sendInterval_ = 100; // 1 seconds

    Ip host_;
    bool active;
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
    void readPacket();
    void stop();
    void infect();
    void checkForInfection();//check infection time
    //void load(Json::Value& json) override;
    //void save(Json::Value& json) override;
};


