// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"//이거 보다 위에 하면 빌드가 안됨 



#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "MyStaticMeshComponent.h"
#include "MyRocket.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"


// Sets default values
AMyPawn::AMyPawn()
{



 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(39.96f, 44.82f, 12.42f));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Game/FirstMap/Blueprints/Meshes/SM_P38_Body.SM_P38_Body"));

	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	Left = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);


	Right = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	
	Left->SetRelativeLocation(FVector(37.f, -21.f, 0.0f));
	Right->SetRelativeLocation(FVector(37.f, 21.f, 0.0f));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	
	Arrow->SetRelativeLocation(FVector(200.f, 0, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	SpringArm->SetupAttachment(Box);

	SpringArm->SocketOffset = FVector(0, 0, 33.33f);
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagMaxDistance = 10;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 1000;

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(),BoostValue);

	//RotatePropeller(Left, PropellerRotationSpeed);
	//RotatePropeller(Right, PropellerRotationSpeed);

}

void AMyPawn::RotatePropeller(USceneComponent* Where, float InRotationSpeed)
{
	Where->AddRelativeRotation(FRotator(0, 0, InRotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

void AMyPawn::Pitch(float InValue)
{
	AddActorLocalRotation(FRotator(RotationSpeed * InValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0, 0));
}

void AMyPawn::Roll(float InValue)
{
	AddActorLocalRotation(FRotator(0, 0, RotationSpeed * InValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));

}

//void AMyPawn::Fire()
//{
//	AMyRocket* SpawnedRocket = GetWorld()->SpawnActor<AMyRocket>(AMyRocket::StaticClass(), Arrow->K2_GetComponentToWorld());
//	if (!SpawnedRocket)
//	{
//		return;
//	}
//
//	// 카메라 시야각(FOV) 기준으로 타겟 선별
//	const FVector ViewLocation = IsValid(Camera) ? Camera->GetComponentLocation() : GetActorLocation();
//	const FVector ViewForward = IsValid(Camera) ? Camera->GetForwardVector() : GetActorForwardVector();
//	const float CameraFovDeg = IsValid(Camera) ? Camera->FieldOfView : 90.0f;
//
//	const float HalfFovRad = FMath::DegreesToRadians(CameraFovDeg * 0.5f);
//	const float MinDot = FMath::Cos(HalfFovRad);
//
//	TArray<AActor*> Pawns;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), Pawns);
//
//	AActor* BestTarget = nullptr;
//	float BestDistSq = TNumericLimits<float>::Max();
//
//	for (AActor* Candidate : Pawns)
//	{
//		
//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Candidate->GetActorNameOrLabel());
//		if (!IsValid(Candidate) || Candidate == this)
//		{
//			continue;
//		}
//
//		FVector ToTarget = Candidate->GetActorLocation() - ViewLocation;
//		const float DistSq = ToTarget.SizeSquared();
//		if (DistSq <= KINDA_SMALL_NUMBER)
//		{
//			continue;
//		}
//
//		ToTarget.Normalize();
//		const float Dot = FVector::DotProduct(ViewForward, ToTarget);
//
//		// 카메라 시야각 밖이면 제외
//		if (Dot < MinDot)
//		{
//			continue;
//		}
//
//		// 시야각 내에서는 가장 가까운 대상 선택
//		if (DistSq < BestDistSq)
//		{
//			BestDistSq = DistSq;
//			BestTarget = Candidate;
//		}
//	}
//
//	if (IsValid(BestTarget))
//	{
//		SpawnedRocket->SetHomingTarget(BestTarget->GetRootComponent());
//	}
//}

//void AMyPawn::Boost()
//{
//	Movement->MaxSpeed = 100;
//}
//
//void AMyPawn::UnBoost()
//{
//	
//}

void AMyPawn::Rotate(const FInputActionValue& Value)
{
	const FVector2D MoveInput = Value.Get<FVector2D>();
	AddActorLocalRotation(FRotator(0, 0, MoveInput.X * RotationSpeed * GetWorld()->GetDeltaSeconds()));
	AddActorLocalRotation(FRotator(MoveInput.Y * RotationSpeed * GetWorld()->GetDeltaSeconds(), 0, 0));
}

void AMyPawn::Fire(const FInputActionValue& Value)
{
	GetWorld()->SpawnActor<AMyRocket>(RocketTemplate, Arrow->K2_GetComponentToWorld());
}

void AMyPawn::Boost(const FInputActionValue& Value)
{
	BoostValue = 1.0f;
}

void AMyPawn::UnBoost(const FInputActionValue& Value)
{
	BoostValue = 0.5f;
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//조건이 되면 자동으로 실행되는 것이 이벤트 <- 엔진이 자동으로 해줌

	//PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	//PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
	//PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyPawn::Fire);
	//PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Pressed, this, &AMyPawn::Boost);
	//PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Released, this, &AMyPawn::UnBoost);
	//
	if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->Rotate)
			{
				EnhancedInput->BindAction(
					PlayerController->Rotate,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Rotate
				);
			}

			if (PlayerController->Boost)
			{
				EnhancedInput->BindAction(
					PlayerController->Boost,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Boost
				);

				EnhancedInput->BindAction(
					PlayerController->Boost,
					ETriggerEvent::Completed,
					this,
					&AMyPawn::UnBoost
				);
			}

			if (PlayerController->Fire)
			{
				EnhancedInput->BindAction(
					PlayerController->Fire,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Fire
				);
			}
		}
	}
}




// cpp에서...


