// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyRocket.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UArrowComponent;
class UProjectileMovementComponent;
class USceneComponent;

UCLASS()
class C_UNREALFIRST_API AMyRocket : public AActor
{
	GENERATED_BODY()
	
public:
	AMyRocket();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UBoxComponent> Box;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UArrowComponent> Arrow;
					
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UProjectileMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Homing)
	bool bUseHoming = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Homing)
	float HomingAcceleration = 12000.0f;

	void SetHomingTarget(USceneComponent* InTarget);

	UFUNCTION()
	void ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTarget;
};
