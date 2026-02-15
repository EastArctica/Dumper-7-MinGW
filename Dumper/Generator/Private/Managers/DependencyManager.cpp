#include "Managers/DependencyManager.h"

DependencyManager::DependencyManager(int32 ObjectToTrack)
{
	AllDependencies.try_emplace(ObjectToTrack, IndexDependencyInfo{ });
}

void DependencyManager::SetExists(const int32 DepedantIdx)
{
	AllDependencies[DepedantIdx];
}

void DependencyManager::AddDependency(const int32 DepedantIdx, int32 DependencyIndex)
{
	AllDependencies[DepedantIdx].DependencyIndices.insert(DependencyIndex);
	AllDependencies[DependencyIndex]; // ensure the dependency node itself is registered
}

void DependencyManager::SetDependencies(const int32 DepedantIdx, std::unordered_set<int32>&& Dependencies)
{
	// Ensure every dependency index has a node entry before moving the set
	for (int32 DepIdx : Dependencies)
		AllDependencies[DepIdx];

	AllDependencies[DepedantIdx].DependencyIndices = std::move(Dependencies);
}

size_t DependencyManager::GetNumEntries() const
{
	return AllDependencies.size();
}

void DependencyManager::VisitIndexAndDependencies(int32 Index, OnVisitCallbackType Callback) const
{
	auto& [IterationHitCounter, Dependencies] = AllDependencies.at(Index);

	if (IterationHitCounter >= CurrentIterationHitCount)
		return;

	IterationHitCounter = CurrentIterationHitCount;

	for (int32 Dependency : Dependencies)
	{
		VisitIndexAndDependencies(Dependency, Callback);
	}

	Callback(Index);
}

void DependencyManager::VisitIndexAndDependenciesWithCallback(int32 Index, OnVisitCallbackType Callback) const
{
	CurrentIterationHitCount++;

	VisitIndexAndDependencies(Index, Callback);
}

void DependencyManager::VisitAllNodesWithCallback(OnVisitCallbackType Callback) const
{
	CurrentIterationHitCount++;

	for (const auto& [Index, DependencyInfo] : AllDependencies)
	{
		VisitIndexAndDependencies(Index, Callback);
	}
}
