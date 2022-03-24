// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RB_UserWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class REBORN_2022_API URB_UserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  virtual bool Initialize();

  UPROPERTY(meta = (BindWidget))
  UProgressBar* ProgressHealthBar;
	
};
