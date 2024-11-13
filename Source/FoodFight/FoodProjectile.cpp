#include "FoodProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

AFoodProjectile::AFoodProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFoodProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Get the game area size from the camera
	AActor* CameraSearch{UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass())};
	if (CameraSearch)
	{
		ACameraActor* CameraActor = Cast<ACameraActor>(CameraSearch);
		UCameraComponent* CameraComponent = CameraActor->GetCameraComponent();

		GameAreaSize.X = CameraComponent->OrthoWidth;
		GameAreaSize.Y = CameraComponent->OrthoWidth / CameraComponent->AspectRatio;

		UE_LOG(LogTemp, Display, TEXT("%f, %f"), GameAreaSize.X, GameAreaSize.Y);
	}

	// Give the player the limits
	AActor* PlayableCharaterSearch{UGameplayStatics::GetActorOfClass(GetWorld(), APlayableCharacter::StaticClass())};
	if (PlayableCharaterSearch)
	{
		PlayableCharacter = Cast<APlayableCharacter>(PlayableCharaterSearch);

		PlayableCharacter->HorizontalLimits.X = -GameAreaSize.X / 2.0f;
		PlayableCharacter->HorizontalLimits.Y = GameAreaSize.X / 2.0f;
		
		PlayableCharacter->VerticalLimits.X = -GameAreaSize.Y / 2.0f;
		PlayableCharacter->VerticalLimits.Y = GameAreaSize.Y / 2.0f;

		PlayableCharacter->PlayableCharacterGameOverDelegate
			.AddDynamic(this, &AFoodProjectile::OnPlayableCharacterGameOver);
	}
	if (HUDClass)
	{
		HUDWidget =
			CreateWidget<UMyHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0), HUDClass);
		if(HUDWidget)
		{
			HUDWidget->AddToViewport();
			SetTimer(0);
		}
	}
	StartNewGame();
}

void AFoodProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodProjectile::StartProjectile()
{
	GetWorldTimerManager().SetTimer(
	ProjectileTimer,
	this,
	&AFoodProjectile::OnInstantiateFoodFightTimerTimeout,
	ProjectileTimerDuration,
	true,
	ProjectileTimerDuration
	);
}

void AFoodProjectile::StopProjectile()
{
	GetWorldTimerManager().ClearTimer(ProjectileTimer);
}

void AFoodProjectile::OnInstantiateFoodFightTimerTimeout()
{
	InstantiateFoodFightProjectile();
}

void AFoodProjectile::OnRestartFoodFightTimerTimeout()
{
	StartNewGame();
}

void AFoodProjectile::OnGameStartFoodFightTimerTimeout()
{
	StartProjectile();
}

void AFoodProjectile::OnGameFoodFightTimerTimeout()
{
	SetTimer(Timer + 1);
}

void AFoodProjectile::InstantiateFoodFightProjectile()
{
	// Calculate a random projectile position for the food
	FVector2D ProjectilePosition{0.0f, 0.0f};
	float InstantiateChance{FMath::FRand()};
	if(InstantiateChance < 0.5f)
	{
		ProjectilePosition.X = GameAreaSize.X / 2.0f;
		if(InstantiateChance < 0.25f)
		{
			ProjectilePosition.X *= -1.0f;
		}
		ProjectilePosition.Y = FMath::FRandRange(-GameAreaSize.Y /2.0f, GameAreaSize.Y / 2.0f);
	}
	else
	{
		ProjectilePosition.Y = GameAreaSize.Y / 2.0f;
		if(InstantiateChance < 0.75f)
		{
			ProjectilePosition.Y *= -1.0f;
		}
		ProjectilePosition.X = FMath::FRandRange(-GameAreaSize.X /2.0f, GameAreaSize.X / 2.0f);
	}
	
	// Calculate the movement direction for the food
	FVector2D MovementDirection{0.0f, 0.0f};
	FVector2D CentrePosition{0.0f, 0.0f};
	MovementDirection = CentrePosition - ProjectilePosition;
	FVector PlayableCharacterPosition{PlayableCharacter->GetActorLocation()};
	MovementDirection = FVector2D(PlayableCharacterPosition.X, PlayableCharacterPosition.Z) - ProjectilePosition;
	MovementDirection.Normalize();
	
	// Instantiate the food and setup
	AFood* Food{
		GetWorld()->SpawnActor<AFood>(
			FoodActorToProjectile, 
			FVector(ProjectilePosition.X, 0.0f, ProjectilePosition.Y),
			FRotator::ZeroRotator
		)
	};
	if(Food)
	{
		Food->MovementDirection = MovementDirection;

		int RandomIndex{FMath::RandRange(0, FoodSprites.Num() - 1)};
		Food->SpriteComp->SetSprite(FoodSprites[RandomIndex]);
	}
}

void AFoodProjectile::SetTimer(int NewScore)
{
	Timer = NewScore;
	HUDWidget->SetTimer(Timer);
}

void AFoodProjectile::OnPlayableCharacterGameOver()
{
	StopProjectile();

	GetWorldTimerManager().ClearTimer(GameTimer);
	
	GetWorldTimerManager().SetTimer(
	RestartTimer,
	this,
	&AFoodProjectile::OnRestartFoodFightTimerTimeout,
	1.0f,
	false,
	RestartTimerDuration
	);
}

void AFoodProjectile::StartNewGame()
{
	// Destroy all the current food projectiles
	TArray<AActor*> FoodSearchArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFood::StaticClass(), FoodSearchArray);
	if(FoodSearchArray.Num() > 0)
	{
		for(AActor* FoodActor : FoodSearchArray)
		{
			AFood* Food = Cast<AFood>(FoodActor);
			if(Food)
			{
				Food->Destroy();
			}
		}
	}
	
	// Reset the playable character
	if(PlayableCharacter)
	{
		PlayableCharacter->IsHit = true;
		PlayableCharacter->SpriteComp->SetVisibility(true);
		PlayableCharacter->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	}
	
	// Reset the timer
	SetTimer(0);
	GetWorldTimerManager().SetTimer(
	GameTimer,
	this,
	&AFoodProjectile::OnGameFoodFightTimerTimeout,
	1.0f,
	true,
	GameTimerDuration
	);
	
	// Initiate the food projectiles again
	GetWorldTimerManager().SetTimer(
	GameStartTimer,
	this,
	&AFoodProjectile::OnGameStartFoodFightTimerTimeout,
	1.0f,
	false,
	GameStartTimerDuration
	);
}
