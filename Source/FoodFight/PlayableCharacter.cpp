#include "PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Food.h"

APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComp"));
	SpriteComp->SetupAttachment(RootComponent);
}

void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APlayableCharacter::OverlapBegin);
}

void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move the player
	if(IsHit)
	{
		if(MovementDirection.Length() > 0.0f)
		{
			if(MovementDirection.Length() > 1.0f)
			{
				MovementDirection.Normalize();
			}
			FVector2D DistanceToMove{MovementDirection * MovementSpeed * DeltaTime};
			FVector NewLocation{GetActorLocation() + FVector(DistanceToMove.X, 0.0f, 0.0f)};
			if(!IsInMapBoundsHorizontal(NewLocation.X))
			{
				NewLocation -= FVector(DistanceToMove.X, 0.0f, 0.0f);
			}
			NewLocation += FVector(0.0f, DistanceToMove.Y, 0.0f);
			if(!IsInMapBoundsVertical(NewLocation.Y))
			{
				NewLocation -= FVector(0.0f, DistanceToMove.Y, 0.0f);
			}
			SetActorLocation(NewLocation);
		}
	}
}

bool APlayableCharacter::IsInMapBoundsHorizontal(float XPosition)
{
	bool Result = true;

	Result = (XPosition > HorizontalLimits.X) && (XPosition < HorizontalLimits.Y);
	
	return Result;
}
bool APlayableCharacter::IsInMapBoundsVertical(float YPosition)
{
	bool Result = true;
	
	Result = (YPosition > VerticalLimits.X) && (YPosition < VerticalLimits.Y);
	
	return Result;
}

void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add the mapping context
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	// Bind actions to functions
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayableCharacter::MoveTrigger);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayableCharacter::MoveCompleted);
	}
}

void APlayableCharacter::MoveTrigger(const FInputActionValue& Value)
{
	MovementDirection = Value.Get<FVector2d>();
}
void APlayableCharacter::MoveCompleted(const FInputActionValue& Value)
{
	MovementDirection = FVector2d(0.0f, 0.0f);
}

void APlayableCharacter::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFood* Food = Cast<AFood>(OtherActor);

	if(Food)
	{
		if(IsHit)
		{
			IsHit = false;
			SpriteComp->SetVisibility(false);

			PlayableCharacterGameOverDelegate.Broadcast();

			UGameplayStatics::PlaySound2D(GetWorld(), GameOverSound);
		}
	}
}
