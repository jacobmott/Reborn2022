// Copyright Vortex Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RB_A_ActorUI.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;
class UCapsuleComponent;
class UTextBlock;

UCLASS()
class REBORN_2022_API ARB_A_ActorUI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARB_A_ActorUI();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collisions")
  TObjectPtr<UCapsuleComponent> CapsuleComponent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh");
  TObjectPtr<UStaticMeshComponent> StaticMeshComp;

  //Floating widget
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextHud")
  TObjectPtr<UWidgetComponent> TextHudWidget;

	/**
	 * The font size is a measure in point values. The conversion of points to Slate Units is done at 96 dpi.  So if
	 * you're using a tool like Photoshop to prototype layouts and UI mock ups, be sure to change the default dpi
	 * measurements from 72 dpi to 96 dpi.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SlateStyleRules, meta = (ClampMin = 1, ClampMax = 1000))
	int32 Size;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  virtual void UpdateFloatingTextHud(float DeltaTime);

  virtual void ToggleHidden();

	UTextBlock* TB;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
  bool IsHidden = true;




};
