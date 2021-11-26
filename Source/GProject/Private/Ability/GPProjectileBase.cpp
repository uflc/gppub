// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/GPProjectileBase.h"
#include "Components/ArrowComponent.h"
#include "Types/GPBPFuncLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGPProjectileBase::AGPProjectileBase(): bUseCustomInitSpeed(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	ProjMovementComp->InitialSpeed=1000.0f;
	ProjMovementComp->MaxSpeed= 1000.0f;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));

	RootComponent = SphereCollision;
	SphereCollision->InitSphereRadius(40.0f);

	SphereCollision->SetCanEverAffectNavigation(false);
	SphereCollision->SetCollisionProfileName(TEXT("WorldDynamic"));
	SphereCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	ArrowComp->SetupAttachment(RootComponent);

	//InitialLifeSpan = 7.777f;

	bReplicates = true;
	SetReplicateMovement(true);
}

void AGPProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->IgnoreActorWhenMoving(GetInstigator(), true);

	if (bUseCustomInitSpeed)
	{
		FTimerHandle InitHandle;
		FVector InitVel = ProjMovementComp->Velocity;
		GetWorld()->GetTimerManager().SetTimer(InitHandle, [this, InitVel]() {
			ProjMovementComp->Velocity = InitVel * ProjMovementComp->MaxSpeed;
			//ProjMovementComp->Velocity.Z /= ProjMovementComp->MaxSpeed;
		}, 0.01, false);
	}
}

bool AGPProjectileBase::ProjectileHit(AActor* OverlappedActor, AActor* OtherActor)
{
	//GP_LOG(Warning, TEXT("Exec ProjectileOverlapped"));

	//GP_LOG(Warning, TEXT("Name is %s"),*OtherActor->GetFName().ToString());

	bool retVal=false;

	if (!EffectContainerSpec.HasValidEffects()) return retVal;

	if (HitActors.Contains(OtherActor)) return retVal;

	HitActors.Add(OtherActor);

	if (OtherActor == GetInstigator()) return retVal;

	TArray<AActor*> TempActor;
	TArray<FHitResult> EmptyResult;

	TempActor.Add(OtherActor);

	retVal = ( (UGPBPFuncLibrary::ApplyExternalEffectContainerSpec(
	UGPBPFuncLibrary::AddTargetsToEffectContainerSpec(EffectContainerSpec,EmptyResult,TempActor)).
	Num()) != 0 );


	return retVal;
	//SetActorEnableCollision(false);
	//GP_LOG(Warning, TEXT("Success"));
}

// Called every frame
void AGPProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FORCEINLINE void AGPProjectileBase::AddIgnoreActorWhenMoving(AActor* inActor)
{
	SphereCollision->IgnoreActorWhenMoving(inActor,true);
}
