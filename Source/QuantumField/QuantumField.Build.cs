// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuantumField : ModuleRules
{
	public QuantumField(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] {"GeometryFramework", "GeometryScriptingCore", "Niagara" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ProceduralMeshComponent" });
	}
}
