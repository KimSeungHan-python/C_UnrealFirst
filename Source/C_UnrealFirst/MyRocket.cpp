// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRocket.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMyRocket::AMyRocket()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(39.96f, 44.82f, 12.42f));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);
	Body->SetRelativeRotation(FRotator(-90.f, 0, 0));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Game/FirstMap/Blueprints/Meshes/SM_Rocket.SM_Rocket"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 2000.0f;
	Movement->MaxSpeed = 2000.0f;
	Movement->ProjectileGravityScale = 0;
	Movement->bRotationFollowsVelocity = true;
	Movement->bIsHomingProjectile = false;
}

// Called when the game starts or when spawned
void AMyRocket::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AMyRocket::ProcessActorBeginOverlap);

	if (Movement && bUseHoming && IsValid(HomingTarget))
	{
		Movement->bIsHomingProjectile = true;
		Movement->HomingTargetComponent = HomingTarget;
		Movement->HomingAccelerationMagnitude = HomingAcceleration;
	}

	SetLifeSpan(3.0f);
}

// Called every frame
void AMyRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyRocket::SetHomingTarget(USceneComponent* InTarget)
{
	HomingTarget = InTarget;

	if (!Movement)
	{
		return;
	}

	const bool bCanHome = bUseHoming && IsValid(HomingTarget);
	if(IsValid(HomingTarget))
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("안녕하세요"));
	
	Movement->bIsHomingProjectile = bCanHome;
	Movement->HomingTargetComponent = bCanHome ? HomingTarget.Get() : nullptr;
	Movement->HomingAccelerationMagnitude = HomingAcceleration;
}

void AMyRocket::ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Destroy();
}

