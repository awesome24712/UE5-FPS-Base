// Fill out your copyright notice in the Description page of Project Settings.


#include "SFancyButton.h"
#include "SlateOptMacros.h"
#include "../../Util.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SFancyButton::Construct(const FArguments& InArgs)
{
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFancyButton::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	Msg("Mouse entered fancy button!\n");
}
