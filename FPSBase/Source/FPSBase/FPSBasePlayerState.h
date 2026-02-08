#pragma once
#include "GameFramework/PlayerState.h"
#include "PODT/ChatMessage.h"
#include "FPSBasePlayerState.generated.h"

/**
 *
 */
UCLASS()
class FPSBASE_API AFPSBasePlayerState : public APlayerState
{
    GENERATED_BODY()

public:

    AFPSBasePlayerState();

    UFUNCTION(Server, Reliable, WithValidation) // for player to player rpc you need to first call the message on the server
        virtual void UserChatRPC(const FChatMsg& newMessage); // first rpc for the server
    UFUNCTION(NetMulticast, Reliable, WithValidation) // then the server calls the function with a multicast that executes on all clients and the server
        virtual void UserChat(const FChatMsg& newMessage); // second rpc for all the clients
};