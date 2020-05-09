#ifndef SDP_HANDLER_H
#define SDP_HANDLER_H


#include "sdp/RemoteSdp.h"
#include <json.hpp>
#include <map>
#include <string>

namespace SdpParse {
    
    class Peer;
    class Signaler;
    class Room;
    
    class Handler
    {
    
    public:
                
        Handler(Signaler *signaler, Room *room, Peer * peer):signaler(signaler),room(room),peer(peer)
        {
            
        }
        ~Handler()
        {
            if (remoteSdp) {
                delete remoteSdp;
                remoteSdp = nullptr;
            }
        }
        
        void transportCreate();
        void transportConnect(const nlohmann::json& sdpObject, const std::string& localDtlsRole);
        void raiseRequest( nlohmann::json &param , nlohmann::json& trans, nlohmann::json& ack_resp);

        Sdp::RemoteSdp *remoteSdp{nullptr};
        void createSdp(const nlohmann::json& iceParameters, const nlohmann::json& iceCandidates, const nlohmann::json& dtlsParameters);
        nlohmann::json  _setupTransport(const nlohmann::json & sdpObject, const std::string& localDtlsRole);
        
        
    protected:
      
        //std::string peerID;
        std::string transportId;
        //nlohmann::json dtlsParameters;
        bool forceTcp{false};
 
        Signaler *signaler;
        Room *room;
        Peer *peer;
        
        std::string classtype;
        
    };

    class Producers:public Handler
    {
       
        public:
        
        Producers(Signaler *signaler, Room *room, Peer *peer);

        void runit(std::string & answer );
        
        struct Producer
        {   std::string answer;
            nlohmann::json producer;
        };
        
        std::map<std::string, Producer*>  mapProducer;
        std::map<size_t, std::string>  mapProdMid;
        
    private:
        std::string GetAnswer(std::string & kind , nlohmann::json &sendingRtpParameters, Sdp::RemoteSdp::MediaSectionIdx &mediaSectionIdx);
        
    };

    class Consumers : public Handler
    {
        
    public:
        Consumers(Signaler *signaler, Room *room, Peer * peer, Producers *producers);
      
        void runit(std::string& offer);

        std::string GetOffer(const std::string& id, size_t  mid , const std::string& kind, const nlohmann::json & rtpParameters);

        void loadAnswer( std::string sdp);
        void resume(Signaler *signal, bool pause );
        

        struct Consumer
        {   std::string offer;
            nlohmann::json consumer;
        };
        
        std::map<std::string, Consumer*>  mapConsumer;
        
    private:
        //int mid{0};
      
        Producers *producers;

    };
    
    
} // namespace SdpParse

#endif
