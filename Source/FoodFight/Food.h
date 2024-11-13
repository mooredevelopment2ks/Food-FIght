#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"

#include "Engine/TimerHandle.h"

#include "Food.generated.h"

UCLASS()
class FOODFIGHT_API AFood : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* SpriteComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestroyTimerDuration = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed {100.0f};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D MovementDirection;

	FTimerHandle DestroyTimer;
	
	AFood();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	
	void OnDestroyTimerTimeout();
};
