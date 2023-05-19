// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class MONSTER_SHOOTER_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* DamageCollision;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& Hit);
	
	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	class UAIPerceptionComponent* AIPerComp;

	// To enable sight detection
	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	class UAISenseConfig_Sight* SightConfig;

	// To inform if any actors are detected by UAIPerceptionComponent
	UFUNCTION()
	void OnSensed(const TArray<AActor*> &UpdatedActors);

	UPROPERTY(VisibleAnywhere, Category=Movement)
	FRotator EnemyRotation;

	/*
	 * The enemy is going to be able to go and try to attack the player,
	 * but if he doesn't find the player, then he is going to back to base location
	 */
	UPROPERTY(VisibleAnywhere, Category=Movement)
	FVector BaseLocation;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category=Movement)
	FVector CurrentVelocity;

	UPROPERTY(VisibleAnywhere, Category=Movement)
	float MovementSpeed;

	void SetNewRotation(FVector TargetPosition, FVector CurrentPosition);

	bool bBackToBaseLocation;
	FVector NewLocation;
	float DistanceSquared;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = 100.f;

	UPROPERTY(EditAnywhere)
	float DamageValue = 5.f;

public:
	void DealDamage(float DamageAmount);



	
};
