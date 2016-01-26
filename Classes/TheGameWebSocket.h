//
// Created by Andrei Nechaev on 1/23/16.
//
#include "cocos2d.h"
#include <network/WebSocket.h>

#pragma once

class TheGameWebSocket: public cocos2d::network::WebSocket::Delegate {
private:
    std::string _url;
    cocos2d::network::WebSocket _socket;

    TheGameWebSocket(const std::string &url) : _url(url){ }
    
public:

    static TheGameWebSocket* create(std::string url);

    void connect();

    void send(std::string data);

    typedef std::function<void(std::string&)> wsReceiveCallback;
    wsReceiveCallback receive;

    void close();

    virtual ~TheGameWebSocket();

    virtual void onOpen(cocos2d::network::WebSocket *ws) override;

    virtual void onMessage(cocos2d::network::WebSocket *ws, const cocos2d::network::WebSocket::Data &data) override;

    virtual void onClose(cocos2d::network::WebSocket *ws) override;

    virtual void onError(cocos2d::network::WebSocket *ws, const cocos2d::network::WebSocket::ErrorCode &error) override;
};

