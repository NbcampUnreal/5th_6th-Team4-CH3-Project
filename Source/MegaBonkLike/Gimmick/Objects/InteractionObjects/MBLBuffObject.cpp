#include "Gimmick/Objects/InteractionObjects/MBLBuffObject.h"

AMBLBuffObject::AMBLBuffObject()
{
}

void AMBLBuffObject::OnObjectActivated(AActor* Activator)
{
	if (SantuaryOptions.IsEmpty()) return;

	TArray<FGameplayTag> OptionTags;
	SantuaryOptions.GetKeys(OptionTags);

	TArray<FGameplayTag> OptionsToShow;
	int Count = FMath::Max(3, OptionTags.Num());

	for (int i = 0; i < Count; ++i)
	{
		int32 RandomIdx = FMath::RandRange(0, OptionTags.Num() - 1);
		OptionsToShow.Add(OptionTags[RandomIdx]);
		OptionTags.RemoveAt(RandomIdx);
	}
}
