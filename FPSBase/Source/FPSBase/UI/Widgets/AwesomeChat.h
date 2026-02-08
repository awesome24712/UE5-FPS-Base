#pragma once

#include "../../PODT/ChatMessage.h"
#include "SlateBasics.h"
#include "../UIConductor.h"

/**
 *
 */
class FPSBASE_API SAwesomeChatWidget : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SAwesomeChatWidget) : _OwnerHUD() {} // the OwnerHUD var is passed to the widget so the owner can be set.

    SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

    SLATE_END_ARGS()

public:

    void Construct(const FArguments& InArgs);

    TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FChatMsg> Item, const TSharedRef<STableViewBase>& OwnerTable); // the function that is called for each chat element to be displayed in the chatbox
    TArray<TSharedPtr<FChatMsg>> Items; // array of all the current items in this players chat box
    TSharedPtr<SListView<TSharedPtr<FChatMsg>>> ListViewWidget; // the acutall widgets for each chat element

    FSlateFontInfo fontinfo;// = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("UI/Fonts/Comfortaa-Regular.ttf"), 15); // Font, Font Size  for the chatbox

    TWeakObjectPtr<AHUD> OwnerHUD;

    TSharedPtr< SVerticalBox > ChatBox;
    TSharedPtr< SEditableText > ChatInput;

    void OnChatTextChanged(const FText& InText);
    void OnChatTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);

    void AddMessage(const FChatMsg& newmessage); // the final stage, this function takes the input and does the final placement in the chatbox

    void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime); // The full widget ticks and deletes messages
};