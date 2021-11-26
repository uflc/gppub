// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GProject.h"
#include "Types/GPTypes.h"
#include "GPInventoryInterface.h"
#include "GameFramework/PlayerController.h"
//#include <iosfwd>
#include "GProjectPlayerController.generated.h"

UCLASS()
class AGProjectPlayerController : public APlayerController, public IGPInventoryInterface
{
	GENERATED_BODY()

public:
	AGProjectPlayerController();

	/** Map of all items owned by this player, from definition to data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<UGPItem*, FGPItemData> InventoryData;

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on GPGameInstanceBase */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FGPItemSlot, UGPItem*> SlottedItems;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TMap<UGPItem*, EItemReplicateFlag> ReplicableItems;

	//UPROPERTY(BlueprintReadOnly, Category = SaveGame)
	//TMap<FGPItemSlot, FGPWAttachmentedData> AttachmentData;

	/** Delegate called when an inventory item has been added or removed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryItemChanged OnInventoryItemChanged;

	/** Native version above, called before BP delegate */
	FOnInventoryItemChangedNative OnInventoryItemChangedNative;

	/** Delegate called when an inventory slot has changed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnSlottedItemChanged OnSlottedItemChanged;

	/** Native version above, called before BP delegate */
	FOnSlottedItemChangedNative OnSlottedItemChangedNative;

	/** Called after the inventory was changed and we notified all delegates */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Inventory)
	void InventoryItemChanged(UGPItem* Item, FGPItemData ItemData);

	/** Called after an item was equipped and we notified all delegates */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Inventory)
	void SlottedItemChanged(FGPItemSlot ItemSlot, UGPItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Inventory)
	void InventoryLoaded();

	/** Adds a new inventory item, will add it to an empty slot if possible. If the item supports count you can add more than one count. It will also update the level when adding if required */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddInventoryItem(UGPItem* NewItem, int32 ItemCount = 1, int32 ItemLevel = 1, bool bAutoSlot = true);

	/** Make item Replicable, true = add, false = remove */
	UFUNCTION(BlueprintCallable, Category = ItemReplicate)
	void UpdateReplicableItem(UGPItem* NewItem, EItemReplicateFlag Flag);

	UFUNCTION(BlueprintCallable, Category = ItemReplicate)
	void UpdateReplicableItems(TArray<UGPItem*> NewItem, EItemReplicateFlag Flag);

	UFUNCTION(BlueprintCallable, Category = ItemReplicate)
	TMap<UGPItem*, EItemReplicateFlag> GetReplicableItems();

	//UFUNCTION(BlueprintCallable, Category = Inventory)
	//bool AddInventoryItemWithData(UGPItem* NewItem, FGPItemData& ItemData, bool bAutoSlot = true);

	/** Remove an inventory item, will also remove from slots. A remove count of <= 0 means to remove all copies */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 RemoveInventoryItem(UGPItem* RemovedItem, int32 RemoveCount = 1);

	/** Returns all inventory items of a given type. If none is passed as type it will return all */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetInventoryItems(TArray<UGPItem*>& Items, FPrimaryAssetType ItemType);

	/** Returns number of instances of this item found in the inventory. This uses count from GetItemData */
	UFUNCTION(BlueprintPure, Category = Inventory)
	int32 GetInventoryItemCount(UGPItem* Item) const;

	/** Returns the item data associated with an item. Returns false if none found */
	UFUNCTION(BlueprintPure, Category = Inventory)
	bool GetInventoryItemData(UGPItem* Item, FGPItemData& ItemData) const;

	/** Sets slot to item, will remove from other slots if necessary. If passing null this will empty the slot */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SetSlottedItem(FGPItemSlot ItemSlot, UGPItem* Item);

	/** Returns item in slot, or null if empty */
	UFUNCTION(BlueprintPure, Category = Inventory)
	UGPItem* GetSlottedItem(FGPItemSlot ItemSlot) const;

	/** Returns all slotted items of a given type. If none is passed as type it will return all */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetSlottedItems(TArray<UGPItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes);

	/** Fills in any empty slots with items in inventory */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void FillEmptySlots();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool IsThereEmptySlotWithType(FPrimaryAssetType inType);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SaveInventory();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventory();

	/** Delegate called when the inventory has been loaded/reloaded */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryLoaded OnInventoryLoaded;

	/** Native version above, called before BP delegate */
	FOnInventoryLoadedNative OnInventoryLoadedNative;

	// Implement IGPInventoryInterface
	virtual const TMap<UGPItem*, FGPItemData>& GetInventoryDataMap() const override
	{
		return InventoryData;
	}
	virtual const TMap<FGPItemSlot, UGPItem*>& GetSlottedItemMap() const override
	{
		return SlottedItems;
	}
	virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDelegate() override
	{
		return OnInventoryItemChangedNative;
	}
	virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() override
	{
		return OnSlottedItemChangedNative;
	}
	virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() override
	{
		return OnInventoryLoadedNative;
	}



protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void OnPossess(APawn* InPawn) override; //Possess ��ü�� final�̶� override ����. setpawn
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	//=========================================================================//

	/** Auto slots a specific item, returns true if anything changed */
	bool FillEmptySlotWithItem(UGPItem* NewItem);

	/** Calls the inventory update callbacks */
	void NotifyInventoryItemChanged(UGPItem* Item, FGPItemData ItemData);
	void NotifySlottedItemChanged(FGPItemSlot ItemSlot, UGPItem* Item);
	void NotifyInventoryLoaded();

	/** Called when a global save game as been loaded */
	void HandleSaveGameLoaded(UGPSaveGame* NewSaveGame);

private:
	class FGPClient* GPClient;

protected:
	//std::stringstream& CreateStringStreamPawnData(std::stringstream& ss);

	UFUNCTION()
	void SendMovementInfo(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);
protected:
	TQueue<FString, EQueueMode::Spsc> ChatMessages;
public:
	UPROPERTY(BlueprintReadWrite)
	class UChatWindow* ChatWindow; //err gced //currently BP class does CreateWidget on BeginPlay.

	UFUNCTION(BlueprintCallable, Category = GPClient)
	void UpdateChat();
	void AddChat(const FString& ChatMsg);

	void SetGPPlayer();

	UFUNCTION(Client, Reliable, Category = GPClient)
	void ClientJoinGP(); //

	UFUNCTION(BlueprintCallable, Category = GPClient)
	void HostGP(bool bHost);

	UFUNCTION(BlueprintCallable, Category = GPClient)
	void JoinParty(bool bJoin);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = GPClient)
	void RecieveHostGP(bool bHost);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = GPClient)
	void RecieveJoinParty(bool bJoin);

public:
	virtual void GetSeamlessTravelActorList(bool bToEntry, TArray<class AActor*>& ActorList) override;

protected:
	FTimerHandle SendTimer;

	void SendData();
};


