// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterShooterCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

// Sets default values
AMonsterShooterCharacter::AMonsterShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(40.f, 95.f);

	TurnRate = 45.f;
	LookUpRate = 45.f;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->AddRelativeLocation(FVector(-39.65f, 1.75f, 64.f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));

	// This code will make only the owner or us having this component see those hands
	HandsMesh->SetOnlyOwnerSee(true);

	HandsMesh->SetupAttachment(FirstPersonCamera);
	HandsMesh->bCastDynamicShadow = false;
	HandsMesh->CastShadow = false;
	HandsMesh->AddRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	HandsMesh->AddRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(GunMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	GunOffset = FVector(100.f, 0.f, 10.f);
	


	
}

// Called when the game starts or when spawned
void AMonsterShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*
	 * SnapToTargetNotIncludingScale: This rule locks the position and rotation of the object to be linked to the
	 * target object, but does not affect its size. That is, the inserted object does not adapt to the size of
	 * the target object, but retains its own size.
	 */
	GunMesh->AttachToComponent(HandsMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("GripPoint"));
	
}

// Called every frame
void AMonsterShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMonsterShooterCharacter::OnFire);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AMonsterShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMonsterShooterCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMonsterShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AMonsterShooterCharacter::LookAtRate);


}

void AMonsterShooterCharacter::OnFire()
{
	
}

void AMonsterShooterCharacter::MoveForward(float Value)
{
	if(Value!=0.f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMonsterShooterCharacter::MoveRight(float Value)
{
	if(Value!=0.f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMonsterShooterCharacter::TurnAtRate(float Rate)
{
	// GetWorld()->GetDeltaSeconds() is for smooth turns
	AddControllerYawInput(Rate * TurnRate * GetWorld()->GetDeltaSeconds()); 
}

void AMonsterShooterCharacter::LookAtRate(float Rate)
{
	AddControllerPitchInput(Rate * LookUpRate * GetWorld()->GetDeltaSeconds());
}

