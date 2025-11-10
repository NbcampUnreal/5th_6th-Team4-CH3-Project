#include "Attribute/AttributeTags.h"
#include "GameplayTagsManager.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Damage, "Attribute.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Size, "Attribute.Size");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_MoveSpeed, "Attribute.MoveSpeed");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_AttackSpeed, "Attribute.AttackSpeed");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_ProjectileSpeed, "Attribute.ProjectileSpeed");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_AttackProjectiles, "Attribute.AttackProjectiles");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Duration, "Attribute.Duration");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_ExpGain, "Attribute.ExpGain");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_GoldGain, "Attribute.GoldGain");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_MaxHP, "Attribute.MaxHP");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_PickupRange, "Attribute.PickupRange");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_JumpCount, "Attribute.JumpCount");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_JumpHeight, "Attribute.JumpHeight");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_CriticalChance, "Attribute.CriticalChance");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_CriticalMultiplier, "Attribute.CriticalMultiplier");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Knockback, "Attribute.Knockback");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_LifeSteal, "Attribute.LifeSteal");

const TArray<FGameplayTag>& GetAttributeTags()
{
	static TArray<FGameplayTag> AttributeTags;
	if (AttributeTags.IsEmpty() == true)
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag ParentTag = FGameplayTag::RequestGameplayTag(FName("Attribute"));
		if (ParentTag.IsValid() == true)
		{
			const auto& ParentNode = Manager.FindTagNode(ParentTag);
			if (ParentNode.IsValid() == true)
			{
				const auto& TagNodes = ParentNode->GetChildTagNodes();
				for (const auto& TagNode : TagNodes)
				{
					if (TagNode == nullptr)
						continue;

					const FGameplayTag& Tag = TagNode->GetCompleteTag();
					if (Tag.IsValid() == false)
						continue;

					AttributeTags.Add(Tag);
				}
			}
		}
	}
	return AttributeTags;
}

FString GetTagName(const FGameplayTag& Tag)
{
	FString TagName = Tag.ToString();
	FString LeftPart;
	FString RightPart;
	TagName.Split(TEXT("."), &LeftPart, &RightPart, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	return RightPart;
}