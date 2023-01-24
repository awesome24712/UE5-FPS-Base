#pragma once
#include "../Util/JSON/PropertyBinder.h"

enum EMenuItemType {
	MIT_OnOff,
	MIT_Scaler_Numbered,
	MIT_Scaler_Text,
	MIT_Dropdown,
	MIT_Keybind,
	MIT_MenuNode, //menu node which can be clicked on to open a submenu
};

class MenuItemProfile : public IJsonBindable {
private:
	EMenuItemType m_type;

	FString m_lineItemCodeName; //the code name for this menu which other menus can use to refer to this menu
	FString m_lineItemText; //the label for this menu item
	FString m_lineItemTooltip; //the hover over tooltip for this menu item. Empty string if there isn't one

	FString m_value; //the value of this menu item, ex. "On", "Off", "Hardcore" etc
	int m_valueIndex; //between 0 and m_options.Num(), or either 0 or 1 if type is MIT_OnOff

	//union m_lineItemData {
		TArray<MenuItemProfile*> m_children; //MIT_MenuNode, list of submenu visible when this menu is opened
		TArray<FString> m_options;
	//};


	MenuItemProfile() {

	}

};