#include "captiveportal.h"

CaptivePortal::CaptivePortal(QWidget *parent) : GStateObj(parent)
{
    QObject::connect(
                &capturer_,
                SIGNAL(captured(GPacket*)),
                this,
                SLOT(processPacket(GPacket*)),
                Qt::DirectConnection
                );
}

void CaptivePortal::setComponent()
{
    capturer_.intfName_ = intfname_;

    tcpblock_.backwardRst_ = false;
    tcpblock_.backwardFin_ = true;
    tcpblock_.backwardFinMsg_ = QStringList{"HTTP/1.0 302 Redirected\r\n"
                                            "Location: "+redirectpage_+"\r\n"
                                            "\r\n"};
    tcpblock_.writer_ = &writer_;
}

bool CaptivePortal::doOpen()
{
    QUrl url = redirectpage_;
    struct addrinfo *servinfo;
    struct addrinfo hints;
    char host[16];

    QString domain = url.host();

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(qPrintable(domain), NULL, &hints, &servinfo))
    {
        qDebug() << "failed to get host ip";
        exit(1);
    }

    getnameinfo(servinfo->ai_addr,
                servinfo->ai_addrlen,
                host, sizeof(host),
                NULL,
                0,
                NI_NUMERICHOST);

    QString ip = QString(host);
    host_ = GIp(ip);
    qInfo() << "domain=" << redirectpage_ << "," << "ip=" << QString(host_);

    setComponent();

    if(!(writer_.open()))
    {
        qDebug() << "failed to open writer";
        return false;
    }
    if(!(tcpblock_.open()))
    {
        qDebug() << "failed to open tcpblock";
        return false;
    }
    if(!(capturer_.open()))
    {
        qDebug() << "failed to open arpspoof";
        return false;
    }
    return true;
}

bool CaptivePortal::doClose()
{
    if(!(writer_.close()))
    {
        qDebug() << "failed to close writer";
        return false;
    }
    if(!(tcpblock_.close()))
    {
        qDebug() << "failed to close tcpblock";
        return false;
    }
    if(!(capturer_.close()))
    {
        qDebug() << "failed to close arpspoof";
        return false;
    }
    return true;
}

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
}

void CaptivePortal::processPacket(GPacket *packet)
{
    const char* castedtcpdata = nullptr;
    if(packet->ethHdr_->type() != GEthHdr::Ip4)
    {
        return;
    }
    if(packet->ipHdr_->p() != GIpHdr::Tcp)
    {
        return;
    }
    if(packet->tcpHdr_->dport() == 443)
    {
        packet->ctrl.block_ = true;
        return;
    }
    else if(packet->tcpHdr_->dport() == 80)
    {
        if(packet->tcpData_.valid())
        {
            castedtcpdata = reinterpret_cast<const char*>(packet->tcpData_.data_);
        }
        else
        {
            qDebug() << "there is no tcpdata in packet";
            return;
        }
        qDebug() << uint32_t(packet->ipHdr_->dip())
                 << ", "
                 << uint32_t(host_) << "check both ip address";
        if(strncmp(castedtcpdata, "GET ", 4) == 0 && packet->ipHdr_->dip() != host_)
        {
            qDebug() << "Send redirect page data to client";
            tcpblock_.block(packet);
        }
        if(strncmp(castedtcpdata, "POST ", 5) == 0 && packet->ipHdr_->dip() == host_)
        {
            //if (auto it = std::search(haystack.begin(), haystack.end(), std::boyer_moore_searcher(needle.begin(), needle.end())); it != haystack.end())
            qDebug() << "infection off";
            capturer_.removeFlows(packet->ipHdr_->dip(), packet->ipHdr_->sip(), packet->ipHdr_->sip(), packet->ipHdr_->dip());
        }
    }
}
