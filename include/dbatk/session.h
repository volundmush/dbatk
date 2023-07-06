#pragma once
#include "dbatk/coordinates.h"
#include <chrono>

namespace dbat {
    struct Message;
    struct ObjectId;
    struct Connection;
    // The Session class represents a specific session of play while a Character is online.
    // A Session is created when a Character logs in, and is destroyed when the Character logs out.
    // It acts as a middle-man and also a repository for data relevant to just this session, and
    // as such it also allows multiple connections to be linked to the same Character simultaneously,
    // all seeing the same thing and accepting input from all of them in first-come-first-served order.
    // This is useful for allowing a player to be logged in from multiple devices at once, or from
    // multiple locations.
    class Session {
    public:
        Session(ObjectId id, int64_t account, entt::entity character);
        ~Session() = default;

        void start();
        void end();

        // Send a message to the linked connections.
        void send(const Message &msg);
        void atObjectDeleted(entt::entity ent);

        // Add a connection to this session.
        void addConnection(int64_t connId);
        void addConnection(const std::shared_ptr<Connection>& conn);
        void onAddConnection(const std::shared_ptr<Connection>& conn);
        // Remove a connection from this session.
        void removeConnection(int64_t connId);
        void removeConnection(const std::shared_ptr<Connection>& conn);
        void onRemoveConnection(const std::shared_ptr<Connection>& conn);
        // Remove all connections from this session.
        void removeAllConnections();

        void onFirstConnection();

        void onLinkDead();

        void onNetworkDisconnected(int64_t connId);

        void changePuppet(entt::entity ent);

        void handleText(const std::string& text);

        void sendText(const std::string& txt);
        void sendLine(const std::string& txt);
        void onHeartbeat(double deltaTime);
        void sendOutput(double deltaTime);

        entt::entity getCharacter() const { return character;};
        entt::entity getPuppet() const { return puppet;};
        int64_t getAccount() const { return account; };
        int16_t getAdminLevel() const { return adminLevel; };
        std::chrono::steady_clock::time_point getLastActivity() const { return lastActivity; };
        std::chrono::system_clock::time_point getCreated() const { return created; };
        ObjectId getId() const { return id; };

        std::size_t getNumClients() const { return clients.size(); };

    protected:
        // The ID of the character is the ID of the session.
        ObjectId id;
        // The character this Session is linked to.
        entt::entity character{entt::null};
        // The object this session is currently controlling. That's USUALLY going to be the Character,
        // but it might not be. For example, if the Character is in a vehicle, the vehicle might be
        // the puppet.
        entt::entity puppet{entt::null};
        // The account we are using the permissions of.
        int64_t account{-1};
        int64_t adminLevel{0};

        // This is a map of all the connections that are currently linked to this session.
        std::unordered_map<int64_t, std::shared_ptr<Connection>> clients;

        std::chrono::steady_clock::time_point lastActivity{};
        std::chrono::system_clock::time_point created{};

        std::string outText;
        int totalConnections{0};
    };

    extern std::function<std::shared_ptr<Session>(ObjectId, int64_t, entt::entity)> makeSession;
    std::shared_ptr<Session> defaultMakeSession(ObjectId id, int64_t, entt::entity character);

    extern std::unordered_map<ObjectId, std::shared_ptr<Session>> sessions;

}