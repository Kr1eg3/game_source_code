// Fill out your copyright notice in the Description page of Project Settings.


#include "RTriggerBox.h"
#include "DrawDebugHelpers.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

void ARTriggerBox::BeginPlay()
{
    Super::BeginPlay();

    //DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
}

ARTriggerBox::ARTriggerBox()
{
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ARTriggerBox::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ARTriggerBox::OnOverlapEnd);
}

void ARTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    // check if Actors do not equal nullptr and that
    if (OtherActor && (OtherActor != this)) {
        // print to screen using above defined method when actor enters trigger box
        //print("Overlap Begin");
        //printFString("Overlapped Actor = %s", *OverlappedActor->GetName());


        if (m_Spawner)
        {
            m_Spawner->SpawnEnemies();
        }

    }
}

void ARTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
    if (OtherActor && (OtherActor != this)) {
        // print to screen using above defined method when actor leaves trigger box
        //print("Overlap Ended");
        //printFString("%s has left the Trigger Box", *OtherActor->GetName());
    }
}
