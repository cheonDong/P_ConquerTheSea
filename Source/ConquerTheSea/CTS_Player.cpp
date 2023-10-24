// Fill out your copyright notice in the Description page of Project Settings.


#include "CTS_Player.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ACTS_Player::ACTS_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(20, 20, 20));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(100.0f, 0, 0));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void ACTS_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACTS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(), BoostValue);
}

// Called to bind functionality to input
void ACTS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent"));
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &ACTS_Player::Boost);
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &ACTS_Player::UnBoost);
		EnhancedInputComponent->BindAction(IA_PitchAndYaw, ETriggerEvent::Triggered, this, &ACTS_Player::PitchAndYaw);
	}
}

void ACTS_Player::Boost(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("EnhancedBoost"));

	BoostValue = 2.0f;
}

void ACTS_Player::UnBoost(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("EnhancedBoost"));

	BoostValue = 1.0f;
}

void ACTS_Player::PitchAndYaw(const FInputActionValue& Value)
{
	if (BoostValue == 0.0f)
	{
		BoostValue = 1.0f;
	}
	
	FVector VectorValue = Value.Get<FVector>();

	if (!VectorValue.IsZero())
	{
		AddActorLocalRotation(FRotator(VectorValue.Y * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 60.0f,
			VectorValue.Z * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 60.0f,
			0));
	}
}
