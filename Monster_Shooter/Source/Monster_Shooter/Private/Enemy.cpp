// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/BoxComponent.h"
#include "MonsterShooterCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = 1250.f;
	SightConfig->LoseSightRadius = 1280.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	
	// Activates the character's ability to spot enemies
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;

	// Activates the character's ability to detect allies
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Activates the character's ability to detect neutral entities
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	SightConfig->SetMaxAge(.1f);

	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	// Binding OnSensed function to AIPerceptionComponent
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnSensed);

	CurrentVelocity = FVector::ZeroVector;
	MovementSpeed = 375.f;

	DistanceSquared = BIG_NUMBER;
	
	

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHit);

	BaseLocation = this->GetActorLocation();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!CurrentVelocity.IsZero())
	{
		NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;

		if(bBackToBaseLocation)
		{
			if((NewLocation - BaseLocation).SizeSquared2D() < DistanceSquared)
			{
				DistanceSquared = (NewLocation - BaseLocation).SizeSquared2D();
			}else
			{
				CurrentVelocity = FVector::ZeroVector;
				DistanceSquared = BIG_NUMBER;
				bBackToBaseLocation = false;

				SetNewRotation(GetActorForwardVector(), GetActorLocation());
			}
		}

		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AMonsterShooterCharacter* Char = Cast<AMonsterShooterCharacter>(OtherActor);

	if(Char)
	{
		Char->DealDamage(DamageValue);
	}
}

void AEnemy::OnSensed(const TArray<AActor*>& UpdatedActors)
{
	/*
	 * UpdatedActors array will contain information
	 * about that's been sensed by the AI perception
	 */
	for(int i = 0; i < UpdatedActors.Num(); ++i)
	{
		FActorPerceptionBlueprintInfo Info;
		// Retrieves whatever has been sensed about given actor (UpdatedActors[i])
		AIPerComp->GetActorsPerception(UpdatedActors[i], Info);
		
		if(Info.LastSensedStimuli[0].WasSuccessfullySensed()) 
		{
			FVector Dir = UpdatedActors[i]->GetActorLocation() - GetActorLocation();
			Dir.Z = 0.f;

			CurrentVelocity = Dir.GetSafeNormal() * MovementSpeed;

			SetNewRotation(UpdatedActors[i]->GetActorLocation(), GetActorLocation());
		}else
		{
			FVector Dir = BaseLocation - GetActorLocation();
			Dir.Z = 0.f;

			if(Dir.SizeSquared2D() > 1.f)
			{
				CurrentVelocity = Dir.GetSafeNormal() * MovementSpeed;
				bBackToBaseLocation = true;

				SetNewRotation(BaseLocation, GetActorLocation());
			}
		}
	}
}

void AEnemy::SetNewRotation(FVector TargetPosition, FVector CurrentPosition)
{
	FVector NewDirection = TargetPosition - CurrentPosition;
	NewDirection.Z = 0.f;

	/*
	 * Calculates the rotation of the enemy character using the value "NewDirection" and assigns
	 * its rotation to the variable "EnemyRotation".
	 * This represents the enemy's character's turn towards target.
	 */
	EnemyRotation = NewDirection.Rotation();

	SetActorRotation(EnemyRotation);
}

void AEnemy::DealDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if(Health <= 0.f)
	{
		Destroy();
	}
}

