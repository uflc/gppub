// Fill out your copyright notice in the Description page of Project Settings.


#include "GPSaveGame.h"
#include "GPGameInstanceBase.h"

void UGPSaveGame::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsLoading() && SavedDataVersion != EGPSaveGameVersion::LatestVersion)
	{
		if (SavedDataVersion < EGPSaveGameVersion::AddedItemData)
		{
			// Convert from list to item data map
			for (const FPrimaryAssetId& ItemId : InventoryItems_DEPRECATED)
			{
				InventoryData.Add(ItemId, FGPItemData(1, 1));
			}

			InventoryItems_DEPRECATED.Empty();
		}

		SavedDataVersion = EGPSaveGameVersion::LatestVersion;
	}
}

void UGPSaveGame::ResetSavedData()
{
	InventoryData.Reset();
	SlottedItems.Reset();
	AttachmentData.Reset();
	SavedStageNodes.Reset();
	ReplicableItems.Reset();
	SavedMessages.Reset();
}
