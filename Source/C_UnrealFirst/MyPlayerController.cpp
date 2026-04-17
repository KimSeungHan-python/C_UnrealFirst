// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AMyPlayerController::AMyPlayerController()
	: InputMapping(nullptr), Boost(nullptr), Fire(nullptr),Rotate(nullptr)
{
	
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMapping)//일단 해봐
			{
				InputSystem->AddMappingContext(InputMapping, 0);//왜 생성자가 아니라 BeginePlay에서 하는걸까?
			}
		}
	}
}
