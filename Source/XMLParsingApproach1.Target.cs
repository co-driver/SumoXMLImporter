// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class XMLParsingApproach1Target : TargetRules
{
	public XMLParsingApproach1Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "XMLParsingApproach1" } );
	}
}
