// Fill out your copyright notice in the Description page of Project Settings.


#include "Flagpoint.h"

#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "PaperSprite.h"
#include "Game/SuperBlockioGameModeBase.h"
#include "Components/FlagComponent.h"

// Sets default values
AFlagpoint::AFlagpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    /*What kind of Flag is it*/
	//SET Flagtype to  EFlagEnum::Checkpoint
    Flagtype = EFlagEnum::Checkpoint;

	//CREATE the BoxCollision Component
    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	//CALL SetCollisionProfileName() on BoxCollision passing in "OverlappAll"
    BoxCollision->SetCollisionProfileName("OverlappAll");
	//CALL BoxCollision->OnComponentBeginOverlap.AddDynamic() passing in this, &AFlagpoint::OnOverlapBegin
    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFlagpoint::OnOverlapBegin);
	//SET the RootComponent to BoxCollision
    SetRootComponent(BoxCollision);

	//CREATE the FlagSprite Component
    FlagSprite = CreateDefaultSubobject<UPaperSpriteComponent>("FlagSprite");
	//CALL SetCollisionProfileName() on FlagSprite passing in "NoCollision"
    FlagSprite->SetCollisionProfileName("NoCollision");
	//CALL SetCollisionEnabled() on FlagSprite passing in ECollisionEnabled::NoCollision
    FlagSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CALL SetupAttachment() on FlagSprite passing in RootComponent
    FlagSprite->SetupAttachment(RootComponent);

	//CREATE the FlagComponent
    FlagComponent = CreateDefaultSubobject< UFlagComponent>("FlagComponent");

}

// Called when the game starts or when spawned
void AFlagpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlagpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlagpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//IF OtherActor->ActorHasTag("Player") AND  NOT FlagComponent->CheckpointHasBeenReached()
    if (OtherActor->ActorHasTag("Player") && ! FlagComponent->CheckpointHasBeenReached())
    {
    	//IF Flagtype IS EFlagEnum::Checkpoint
        if (Flagtype == EFlagEnum::Checkpoint)
        {
        	//CALL OnCheckpoint() on FlagComponent passing in this->GetActorLocation()
            FlagComponent->OnCheckpoint(this->GetActorLocation());
        	//CALL SetSprite() on FlagSprite passin in --> Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), NULL, TEXT("/Game/Textures/8Bit_Checkpoint_ON_Sprite")))
            FlagSprite->SetSprite(Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), NULL, TEXT("/Game/Textures/8Bit_Checkpoint_ON_Sprite"))));
        }
    	//ELSE IF Flagtype IS EFlagEnum::Goal
        else if (Flagtype == EFlagEnum::Goal)
        {
        	//CALL OnGoal() on FlagComponent
            FlagComponent->OnGoal();
        }
    	//ENDIF
    }
}
