// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class XMLParsingApproach1EditorTarget : TargetRules
{
	public XMLParsingApproach1EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "XMLParsingApproach1" } );
	}
}
