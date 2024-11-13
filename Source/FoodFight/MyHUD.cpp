#include "MyHUD.h"

void UMyHUD::SetTimer(int NewTimer)
{
	FString TimerString{FString::Printf(TEXT("TIMER:%d"), NewTimer)};
	TimerText->SetText(FText::FromString(TimerString));
}