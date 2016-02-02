#pragma once

namespace EngineSettings
{
	const int maxQuadsCnt		= 8192;
	const int spriteDefaultSize = 32;

	const int   logicThreadUpdateDurationInMs = 10;
	const float logicThreadUpdateDurationInSec = (float)logicThreadUpdateDurationInMs / 1000.0f;

	const int   eventsThreadUpdateDurationInMs = 10;
	const float eventsThreadUpdateDurationInSec = (float)eventsThreadUpdateDurationInMs / 1000.0f;

	const int   physicsThreadUpdateDurationInMs = 5;
	const float physicsThreadUpdateDurationInSec = (float)physicsThreadUpdateDurationInMs / 1000.0f;

	const int	orbitPastLength		= 100;
	const int	orbitFutureLength	= 100;
	const float	phyOrbitUpdateTimeInSec = 0.5f;

	const int	phyFutureOrbitCalcSteps = 5000;
	const float phyFutureOrbitCalcStepDuration = 0.01f;
}