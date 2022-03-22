// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/RB_ACC_HealthComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
URB_ACC_HealthComponent::URB_ACC_HealthComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = false;

  DefaultHealth = 100;
  Health = DefaultHealth;

  // ...
}


// Called when the game starts
void URB_ACC_HealthComponent::BeginPlay()
{
  Super::BeginPlay();

  AActor* Owner = GetOwner();
  if (Owner) {
    Owner->OnTakeAnyDamage.AddDynamic(this, &URB_ACC_HealthComponent::HandleTakeAnyDamage);
  }

  SetIsReplicated(true);

  // ...
  
}

void URB_ACC_HealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
  UKismetSystemLibrary::PrintString(GetWorld(), TEXT("URB_ACC_HealthComponent: HandleTakeAnyDamage: "), true, false, FColor::Red, 5.0f);
  //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("HandleTakeAnyDamage! here1 "));
  if (Damage < 0) {
    //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("HandleTakeAnyDamage! Damage is less than 0 "));
    return;
  }

  Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

}

// Called every frame
//void URB_ACC_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

