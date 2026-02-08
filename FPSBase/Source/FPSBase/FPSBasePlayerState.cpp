#include "FPSBasePlayerState.h"
#include "PODT/ChatMessage.h"
#include "UI/AwesomeGlass.h"
#include "BGController.h"
#include "UI/Widgets/AwesomeChat.h"

AFPSBasePlayerState::AFPSBasePlayerState()
{

}

bool AFPSBasePlayerState::UserChatRPC_Validate(const FChatMsg& newmessage) {
    return true;
}

void AFPSBasePlayerState::UserChatRPC_Implementation(const FChatMsg& newmessage) {
    UserChat(newmessage);
}

bool AFPSBasePlayerState::UserChat_Validate(const FChatMsg& newmessage) {
    return true;
}

void AFPSBasePlayerState::UserChat_Implementation(const FChatMsg& newmessage) {
    ABGController* MyCon;
    AUIConductor* MyHud;

    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) // find all controllers
    {
        MyCon = Cast<ABGController>(*Iterator);
        if (MyCon)
        {
            MyHud = Cast<AUIConductor>(MyCon->GetHUD());
            if (MyHud && MyHud->m_chatWidget.IsValid())
                MyHud->GetChatWidget()->AddMessage(newmessage); // place the chat message on this player controller
        }
    }
}