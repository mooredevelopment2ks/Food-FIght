#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"

#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class FOODFIGHT_API UMyHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TimerText;

	void SetTimer(int NewTimer);
};
