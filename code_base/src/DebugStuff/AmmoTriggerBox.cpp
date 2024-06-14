// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugStuff/AmmoTriggerBox.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

AAmmoTriggerBox::AAmmoTriggerBox()
{
	//Register Events
	OnActorBeginOverlap.AddUniqueDynamic(this, &AAmmoTriggerBox::OnOverlapBegin);
}

void AAmmoTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    // check if Actors do not equal nullptr and that
    if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag(FName("V9")))
    {
        // print to screen using above defined method when actor enters trigger box
        print("Ammo Refilled");
        //printFString("Overlapped Actor = %s", *OverlappedActor->GetName());

        auto player = Cast<ARChar>(OtherActor);
        if (player)
            player->GetCombatComponent()->ResetFullAmmo();

    }
}
