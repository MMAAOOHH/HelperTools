#include "Animation/SyncMarkAnimMod.h"
#include "AnimationBlueprintLibrary.h"
#include "AnimPose.h"
#include "BlueprintActionDatabase.h"
#include "ISkeletonEditorModule.h"
#include "IEditableSkeleton.h"

void USyncMarkAnimMod::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	if (!AnimationSequence)
		return;

	Super::OnApply_Implementation(AnimationSequence);
	UAnimationBlueprintLibrary::AddAnimationNotifyTrack(AnimationSequence, NotifyTrackName);
	
	int32 NumFrames = 0;
	UAnimationBlueprintLibrary::GetNumFrames(AnimationSequence, NumFrames);
	for (const auto& BoneName : BoneNames)
	{
		FString CurveName;
		if (bDebugCurves)
		{
			CurveName = FString::Printf(TEXT("%s.%s"), *BoneName, *UEnum::GetValueAsString(BoneForwardAxis));
			UAnimationBlueprintLibrary::AddCurve(AnimationSequence, FName(CurveName));
		}
	
		PrevBoneAxisValue = GetAxisValueAtFrame(AnimationSequence, NumFrames -1, FName(BoneName),BoneForwardAxis);
		for (int32 Frame = 0; Frame < NumFrames; Frame++)
		{
			const float AxisValue  = GetAxisValueAtFrame(AnimationSequence, Frame, FName(BoneName),BoneForwardAxis);

			if (CheckPlaceMarker(AxisValue))
			{
				float CurTime = 0;
				UAnimationBlueprintLibrary::GetTimeAtFrame(AnimationSequence, Frame, CurTime);
				UAnimationBlueprintLibrary::AddAnimationSyncMarker(AnimationSequence, FName(BoneName), CurTime + MarkerTimeOffset, NotifyTrackName);
			}

			if (bDebugCurves)
			{
				float CurTime = 0;
				UAnimationBlueprintLibrary::GetTimeAtFrame(AnimationSequence, Frame, CurTime);
				UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, FName(CurveName), CurTime, AxisValue );
			}
			
			PrevBoneAxisValue = AxisValue;
		}
		
		if (bAddMarkersToSkeletonAsset)
		{
			AddAnimationSyncMarkerToSkeleton(AnimationSequence, FName(BoneName));
		}
	}
}

void USyncMarkAnimMod::OnRevert_Implementation(UAnimSequence* AnimationSequence)
{
	Super::OnRevert_Implementation(AnimationSequence);
	UAnimationBlueprintLibrary::RemoveAnimationNotifyTrack(AnimationSequence, NotifyTrackName);
	for (const auto& BoneName : BoneNames)
	{
		UAnimationBlueprintLibrary::RemoveCurve(AnimationSequence, FName(BoneName + "." + UEnum::GetValueAsString(BoneForwardAxis)));
	}
}

float USyncMarkAnimMod::GetAxisValueAtFrame(const UAnimSequence* AnimationSequence, const int32 Frame, const FName BoneName, const AxisEnum Axis)
{
	FAnimPose Pose;
	UAnimPoseExtensions::GetAnimPoseAtFrame(AnimationSequence, Frame, FAnimPoseEvaluationOptions(),Pose);
	const FTransform BonePose = UAnimPoseExtensions::GetBonePose(Pose, BoneName, EAnimPoseSpaces::World);
	
	const FVector BoneLocation = BonePose.GetLocation();
	float AxisValue = 0;
	switch (Axis)
	{
	case AxisEnum::X:
		AxisValue = BoneLocation.X;
		break;
	case AxisEnum::Y:
		AxisValue = BoneLocation.Y;
		break;
	case AxisEnum::Z:
		AxisValue = BoneLocation.Z;
		break;
	default:
		AxisValue = 0;
	}
	return AxisValue;
}

bool USyncMarkAnimMod::CheckPlaceMarker(float AxisValue)
{
	return  AxisValue <= 0 && PrevBoneAxisValue > 0;
}

void USyncMarkAnimMod::AddAnimationSyncMarkerToSkeleton(const UAnimSequence* AnimationSequence, const FName MarkerName)
{
	USkeleton* SeqSkeleton = AnimationSequence->GetSkeleton();
	ISkeletonEditorModule& SkeletonEditorModule = FModuleManager::LoadModuleChecked<ISkeletonEditorModule>("SkeletonEditor");
	TSharedRef<IEditableSkeleton> EditableSkeleton = SkeletonEditorModule.CreateEditableSkeleton(SeqSkeleton);
	EditableSkeleton->AddSyncMarker(MarkerName);
}
