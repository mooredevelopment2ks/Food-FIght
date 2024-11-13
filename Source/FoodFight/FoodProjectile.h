#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/TimerHandle.h"

#include "PlayableCharacter.h"
#include "Food.h"
#include "MyHUD.h"

#include "FoodProjectile.generated.h"

UCLASS()
class FOODFIGHT_API AFoodProjectile : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D GameAreaSize;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMyHUD> HUDClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMyHUD* HUDWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFood> FoodActorToProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UPaperSprite*> FoodSprites;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileTimerDuration = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RestartTimerDuration = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameStartTimerDuration = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameTimerDuration = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Timer = 0;
	FTimerHandle ProjectileTimer;
	FTimerHandle RestartTimer;
	FTimerHandle GameStartTimer;
	FTimerHandle GameTimer;
	APlayableCharacter* PlayableCharacter;
	
	AFoodProjectile();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

	void StartProjectile();
	void StopProjectile();
	void OnInstantiateFoodFightTimerTimeout();
	void OnRestartFoodFightTimerTimeout();
	void OnGameStartFoodFightTimerTimeout();
	void OnGameFoodFightTimerTimeout();
	void InstantiateFoodFightProjectile();
	void SetTimer(int NewScore);

	UFUNCTION()
	void OnPlayableCharacterGameOver();

	void StartNewGame();
};
