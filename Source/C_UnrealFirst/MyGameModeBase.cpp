#include "MyGameModeBase.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	// 생성자 초기화-> CDO(Class Default Object) 초기화
	// 에셋 로딩X
	// 값만 초기화, 에디터 기본 값



	//UE_LOG(LogTemp, Warning, TEXT("안녕하세요"));
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("안녕하세요"));
	//}

	DefaultPawnClass = AMyPawn::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();

}
