// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class FPSBASE_API SFancyButton : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFancyButton)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/**
	 * The system will use this event to notify a widget that the cursor has entered it. This event is uses a custom bubble strategy.
	 *
	 * @param MyGeometry The Geometry of the widget receiving the event
	 * @param MouseEvent Information about the input event
	 */
	 void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
};
