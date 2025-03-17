// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuantumFieldEditor : ModuleRules
{
	public QuantumFieldEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "QuantumField", "Blutility" });
	}
}
