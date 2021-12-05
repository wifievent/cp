#include "spdlog/spdlog.h"
#include "opennet.h"
#include "arpspoof.h"
#include "tcpblock.h"
#include "pcapdevice.h"
#include "packet.h"
#include "appjson.h"
#include "json.h"
#include "obj.h"
struct Core : Obj
{
    Core();
    ~Core(){};
    std::string redirectpage_ = "http://wifievent.io";
    ArpSpoof arpspoof_;
    TcpBlock tcpblock_;
    PcapDevice writer_;
    PcapCapture capturer_;

    int infectionTime = 15; //15 seconds
    int reinfectionTime = 3600; // 60 minutes

    Ip host_;
    bool active = true;

    std::thread* readPacket_;

    void prepare();
    void captured(Packet *packet);
    void read();
    void checkForInfection();//check infection time
    void checkForReInfection();//check re_infection time
    void load(Json::Value& json) override;
    void save(Json::Value& json) override;
};


