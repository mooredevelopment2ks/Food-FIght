#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"

#include "Sound/SoundBase.h"

#include "PlayableCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayableCharacterGameOverDelegate);

UCLASS()
class FOODFIGHT_API APlayableCharacter : public APawn
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* SpriteComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* GameOverSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsHit{true};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed {100.0f};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D MovementDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D HorizontalLimits;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D VerticalLimits;
	FPlayableCharacterGameOverDelegate PlayableCharacterGameOverDelegate;
	
	APlayableCharacter();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTrigger(const FInputActionValue& Value);
	void MoveCompleted(const FInputActionValue& Value);

	bool IsInMapBoundsHorizontal(float XPosition);
	bool IsInMapBoundsVertical(float YPosition);

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
