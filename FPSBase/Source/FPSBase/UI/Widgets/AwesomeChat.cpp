
#include "AwesomeChat.h"
#include "../AwesomeGlass.h"
#include "../UIConductor.h"
#include "../../FPSBasePlayerState.h"
#include "Components/Widget.h"

#define LOCTEXT_NAMESPACE "SAwesomeChatWidget"

void SAwesomeChatWidget::Construct(const FArguments& InArgs)
{
    OwnerHUD = InArgs._OwnerHUD;

    //static ConstructorHelpers::FObjectFinder RobotoFontObj(*UWidget::GetDefaultFontName());
    fontinfo = FSlateFontInfo(SharedAssets::corsiva, 20);

    ChildSlot // Build the base for the chatbox
        .VAlign(VAlign_Bottom)
        .HAlign(HAlign_Left)
        .Padding(15, 200) // move the chat box out from the corner 
        [
            SNew(SVerticalBox) // outter container
                + SVerticalBox::Slot()
                .AutoHeight()
                .MaxHeight(408.f)
                .VAlign(VAlign_Bottom)
                [
                    SAssignNew(ListViewWidget, SListView< TSharedPtr< FChatMsg > >) // a ListView widget that takes the array of messages and draws them on the hud
                        .ListItemsSource(&Items) //The Items array is the source of this listview
                        //.OnGenerateRow()
                        .OnGenerateRow(this, &SAwesomeChatWidget::OnGenerateRowForList) // The widget is trying to draw, give the elements
                        .ScrollbarVisibility(EVisibility::Hidden)
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                .FillHeight(30.f)
                [
                    SNew(SHorizontalBox)
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .MaxWidth(600.f)
                        [
                            SAssignNew(ChatInput, SEditableText) // the widget for player input
                                .OnTextCommitted(this, &SAwesomeChatWidget::OnChatTextCommitted) // function to call when text is entered
                                .OnTextChanged(this, &SAwesomeChatWidget::OnChatTextChanged) // function to call when text is changed
                                .ClearKeyboardFocusOnCommit(true)
                                .Text(FText::FromString(""))
                                .Font(FSlateFontInfo(SharedAssets::corsiva, fontinfo.Size + 2)) // set the font for the input and add 2 font size
                                .ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.9f)) // send color and alpha R G B A
                                .HintText(FText::FromString("Send a message to everyone, press Y")) // hint message (optional)
                        ]
                ]
        ];
}

TSharedRef<ITableRow> SAwesomeChatWidget::OnGenerateRowForList(TSharedPtr< FChatMsg > Item, const TSharedRef<STableViewBase>& OwnerTable)
{
    if (!Items.IsValidIndex(0) || !Item.IsValid() || !Item.Get()) // Error catcher
        return
        SNew(STableRow< TSharedPtr< FChatMsg > >, OwnerTable)
        [
            SNew(SBox)
        ];

    if (Item.Get()->m_type == 1) // Type 1 is for player chat messages
        return
        SNew(STableRow< TSharedPtr< FChatMsg > >, OwnerTable)
        [
            SNew(SWrapBox)
                .PreferredSize(600.f)
                + SWrapBox::Slot()
                [
                    SNew(STextBlock) // places the timestamp
                        .Text(Item.Get()->m_tTimestamp)
                        .ColorAndOpacity(FLinearColor(0.25f, 0.25f, 0.25f, 1.f))
                        .Font(fontinfo)
                        .ShadowColorAndOpacity(FLinearColor::Black)
                        .ShadowOffset(FIntPoint(1, 1))
                ]
                + SWrapBox::Slot()
                [
                    SNew(STextBlock) // places the username
                        .Text(Item.Get()->m_username)
                        .ColorAndOpacity(FLinearColor::White)
                        .Font(fontinfo)
                        .ShadowColorAndOpacity(FLinearColor::Black)
                        .ShadowOffset(FIntPoint(1, 1))
                ]
                + SWrapBox::Slot()
                [
                    SNew(STextBlock) // adds the : between the username and chat text
                        .Text(FText::FromString(" :  "))
                        .ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f))
                        .Font(fontinfo)
                        .ShadowColorAndOpacity(FLinearColor::Black)
                        .ShadowOffset(FIntPoint(1, 1))
                ]
                + SWrapBox::Slot()
                [
                    SNew(STextBlock) // places the user text
                        .Text(Item.Get()->m_text)
                        .ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f))
                        .Font(fontinfo)
                        .ShadowColorAndOpacity(FLinearColor::Black)
                        .ShadowOffset(FIntPoint(1, 1))
                ]
        ];
    else // 2 is for server messages, add more types for whispers friendslists etc
        return
        SNew(STableRow< TSharedPtr< FChatMsg > >, OwnerTable)
        [
            SNew(SWrapBox)
                .PreferredSize(600.f)
                + SWrapBox::Slot()
                [
                    SNew(STextBlock)
                        .Text(Item.Get()->m_tTimestamp)
                        .ColorAndOpacity(FLinearColor(0.25f, 0.25f, 0.25f, 1.f))
                        .Font(fontinfo)
                        .ShadowColorAndOpacity(FLinearColor::Black)
                        .ShadowOffset(FIntPoint(1, 1))
                ]
                + SWrapBox::Slot()
                [
                    SNew(STextBlock)
                        .Text(Item.Get()->m_text)
                        .ColorAndOpacity(FLinearColor(0.75f, 0.75f, 0.75f, 1.f))
                        .Font(fontinfo)
                        .ShadowColorAndOpacity(FLinearColor::Black)
                        .ShadowOffset(FIntPoint(1, 1))
                ]
        ];
}

void SAwesomeChatWidget::OnChatTextChanged(const FText& InText) // Called everytime the user presses a key on the input bar
{
    FString SText = InText.ToString();
    if (SText.Len() > 120) // if there are more that 120 characters in the char box, remove the rest
    {
        SText = SText.Left(120);
        if (ChatInput.IsValid())
            ChatInput->SetText(FText::FromString(SText));
    }
}

void SAwesomeChatWidget::OnChatTextCommitted(const FText& InText, ETextCommit::Type CommitMethod) // The chat box is submitted
{
    if (CommitMethod != ETextCommit::OnEnter) // only complete if the textbox was comitted with enter
        return;

    if (ChatInput.IsValid())
    {
        FText NFText = FText::TrimPrecedingAndTrailing(InText); // remove whitespace
        if (!NFText.IsEmpty())
        {
            AFPSBasePlayerState* MyPS = Cast<AFPSBasePlayerState>(OwnerHUD->GetOwningPlayerController()->PlayerState); // cast to our player state that contains the rpc functions
            if (MyPS)
            {
                // Insert code here if you wish to have / commands
                FChatMsg newmessage; // make a new struct to send for replication
                newmessage.Init(1, FText::FromString(MyPS->GetPlayerName()), NFText); // initialize the message struct for replication
                if (newmessage.m_type > 0)
                    MyPS->UserChatRPC(newmessage); // Send the complete chat message to the PlayerState so it can be replicated then displayed
            }
        }
        ChatInput->SetText(FText()); // clear the chat box now were done with it
    }

    FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::SetDirectly); // set the players focus back to the gameport
}

void SAwesomeChatWidget::AddMessage(const FChatMsg& newmessage) // this function is the last in line and does the actual placing of the message
{
    int32 index = Items.Add(MakeShareable(new FChatMsg())); // add a new message to the chatbox array
    if (Items[index].IsValid())
    {
        Items[index]->Init(newmessage.m_type, newmessage.m_username, newmessage.m_text); // intiate our new message with the passed message

        int32 Year, Month, Day, DayOfWeek, Hour, Minute, Second, Millisecond; // set the timestamp and decay timer
        FPlatformTime::SystemTime(Year, Month, DayOfWeek, Day, Hour, Minute, Second, Millisecond);
        Items[index]->SetTime(FText::FromString(FString::Printf(TEXT("[ %02d:%02d:%02d ] "), Hour, Minute, Second)), FPlatformTime::Seconds()); // Comment this line to remove timestamps or replace FPlatformTime::Seconds() with 0 to slow decay the messages

        ListViewWidget->RequestListRefresh(); // update the chatbox widget with our new array element
        ListViewWidget->ScrollToBottom(); // scroll the chatbox to the bottom so our new message pops up
    }
}

void SAwesomeChatWidget::Tick(const FGeometry&AllottedGeometry, const double InCurrentTime, const float InDeltaTime) // called everyframe and used for our gamelogic
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

    if (Items.Num()) // make sure there is atleast one element in the chatbox array
    {
        if (!Items[0]->m_dTimestamp) // this element doesnt have a creation time and will last forever so lets set the creation time now and it was start decaying
            Items[0]->m_dTimestamp = InCurrentTime;
        if (InCurrentTime - Items[0]->m_dTimestamp > 20) // the first message in the array is older that 20 seconds
        {
            Items[0]->Destroy(); // clear the vars and pointers
            Items.RemoveAt(0); // remove the item from the array
            Items.Shrink();
        }
    }
}