// Fill out your copyright notice in the Description page of Project Settings.


#include "ForceDemo/RB_AC_ForceActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ARB_AC_ForceActor::ARB_AC_ForceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Force Mesh"));
  StaticMeshComp->SetupAttachment(RootComponent);

  ForceStrength = 10000.0f;

}

// Called when the game starts or when spawned
void ARB_AC_ForceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARB_AC_ForceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


  FVector UpForce = StaticMeshComp->GetUpVector();
  StaticMeshComp->AddForce(UpForce * ForceStrength * StaticMeshComp->GetMass());

}

