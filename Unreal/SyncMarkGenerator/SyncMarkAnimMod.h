#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "SyncMarkAnimMod.generated.h"

UENUM()
enum AxisEnum
{
	X,
	Y,
	Z
};

UCLASS()
class RMC_PROTOTYPE_API USyncMarkAnimMod : public UAnimationModifier
{
	GENERATED_BODY()

	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	virtual void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;
	float GetAxisValueAtFrame(const UAnimSequence* AnimationSequence,  int Frame, FName BoneName, const AxisEnum Axis);
	bool CheckPlaceMarker(float AxisValue);
	void AddAnimationSyncMarkerToSkeleton(const UAnimSequence* AnimationSequence, FName MarkerName);

public:
	UPROPERTY(EditAnywhere)
	FName NotifyTrackName = "Sync";
	UPROPERTY(EditAnywhere)
	TArray<FString> BoneNames { "foot_l", "foot_r" };
	UPROPERTY(EditAnywhere)
	TEnumAsByte<AxisEnum> BoneForwardAxis = AxisEnum::Y;
	UPROPERTY(EditAnywhere)
	float MarkerTimeOffset = 0.0f;
	UPROPERTY(EditAnywhere)
	bool bAddMarkersToSkeletonAsset = true;
	UPROPERTY(EditAnywhere)
	bool bDebugCurves = false;

private:
	UPROPERTY()
	UAnimSequence* AnimSequence;
	UPROPERTY()
	int FrameIndex;
	UPROPERTY()
	float PrevBoneAxisValue = 0;
};
