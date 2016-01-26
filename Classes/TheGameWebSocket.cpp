//
// Created by Andrei Nechaev on 1/23/16.
//

#include <iostream>
#include "TheGameWebSocket.h"

TheGameWebSocket::~TheGameWebSocket() {
    _socket.close();
}

TheGameWebSocket *TheGameWebSocket::create(std::string url) {
    return new TheGameWebSocket(url);
}

void TheGameWebSocket::connect() {
    cocos2d::log("Connecting... to %s", _url.c_str());
    _socket.init(*this, _url);
}

void TheGameWebSocket::close() {
    if (_socket.getReadyState() == cocos2d::network::WebSocket::State::OPEN) {
        _socket.close();
    }
}

void TheGameWebSocket::onOpen(cocos2d::network::WebSocket *ws) {
    cocos2d::log("WebSocket opened");
}

void TheGameWebSocket::onMessage(cocos2d::network::WebSocket *ws, const cocos2d::network::WebSocket::Data &data) {
    std::string json(data.bytes);
    receive(json);
}

void TheGameWebSocket::onClose(cocos2d::network::WebSocket *ws) {
    cocos2d::log("WebSocket closed");
}

void TheGameWebSocket::onError(cocos2d::network::WebSocket *ws, const cocos2d::network::WebSocket::ErrorCode &error) {
    cocos2d::log("Error on websocket");
}

void TheGameWebSocket::send(std::string string) {
    _socket.send(string);
}