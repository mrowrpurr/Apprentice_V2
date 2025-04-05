#include <SkyrimScripting/Plugin.h>

// BGSEventProcessedEvent
// TESActivateEvent
// TESActiveEffectApplyRemoveEvent
// TESActorLocationChangeEvent
// TESBookReadEvent
// TESCellAttachDetachEvent
// TESCellFullyLoadedEvent
// TESCellReadyToApplyDecalsEvent
// TESCombatEvent
// TESContainerChangedEvent
// TESDeathEvent
// TESDestructionStageChangedEvent
// TESEnterBleedoutEvent
// TESEquipEvent
// TESFormDeleteEvent
// TESFurnitureEvent
// TESGrabReleaseEvent
// TESHitEvent
// TESInitScriptEvent
// TESLoadGameEvent
// TESLockChangedEvent
// TESMagicEffectApplyEvent
// TESMagicWardHitEvent
// TESMoveAttachDetachEvent
// TESObjectLoadedEvent
// TESObjectREFRTranslationEvent
// TESOpenCloseEvent
// TESPackageEvent
// TESPerkEntryRunEvent
// TESQuestInitEvent
// TESQuestStageEvent
// TESQuestStageItemDoneEvent
// TESQuestStartStopEvent
// TESResetEvent
// TESResolveNPCTemplatesEvent
// TESSceneEvent
// TESSceneActionEvent
// TESScenePhaseEvent
// TESSellEvent
// TESSleepStartEvent
// TESSleepStopEvent
// TESSpellCastEvent
// TESPlayerBowShotEvent
// TESTopicInfoEvent
// TESTrackedStatsEvent
// TESTrapHitEvent
// TESTriggerEvent
// TESTriggerEnterEvent
// TESTriggerLeaveEvent
// TESUniqueIDChangeEvent
// TESWaitStartEvent
// TESWaitStopEvent
// TESSwitchRaceCompleteEvent
// TESFastTravelEndEvent

class EventSink : public RE::BSTEventSink<SKSE::CrosshairRefEvent>,
                  public RE::BSTEventSink<RE::TESActivateEvent>,
                  public RE::BSTEventSink<RE::InputEvent*> {
public:
    static EventSink& instance() {
        static EventSink singleton;
        return singleton;
    }

    RE::BSEventNotifyControl
    ProcessEvent(const SKSE::CrosshairRefEvent* event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*) {
        if (event->crosshairRef) {
            Log("Crosshair is over {}", event->crosshairRef->GetBaseObject()->GetName());
            if (std::string{event->crosshairRef->GetBaseObject()->GetName()}.contains("Forge")) {
                Log("Crosshair is over a forge!");
                event->crosshairRef->SetActivationBlocked(true);
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
    ProcessEvent(const RE::TESActivateEvent* event, RE::BSTEventSource<RE::TESActivateEvent>*) {
        if (event && event->actionRef && event->actionRef->IsPlayerRef()) {
            if (event->objectActivated) {
                Log("Player activated {}", event->objectActivated->GetName());
                if (std::string{event->objectActivated->GetName()}.contains("Forge")) {
                    Log("Player activated a forge!");
                }
            } else {
                Log("Player deactivated {}", event->actionRef->GetName());
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
    ProcessEvent(RE::InputEvent* const* eventPtr, RE::BSTEventSource<RE::InputEvent*>*) {
        if (!eventPtr) return RE::BSEventNotifyControl::kContinue;

        auto* event = *eventPtr;
        if (!event) return RE::BSEventNotifyControl::kContinue;

        const auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) return RE::BSEventNotifyControl::kContinue;

        auto attackState = player->AsActorState()->GetAttackState();
        if (attackState == RE::ATTACK_STATE_ENUM::kDraw) Log("Player is drawing a weapon!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kSwing) Log("Player is swinging a weapon!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kHit) Log("Player hit with a weapon!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kNextAttack)
            Log("Player is preparing the next attack!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kFollowThrough)
            Log("Player is following through with an attack!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kBash) Log("Player is bashing!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kBowDraw) Log("Player is drawing a bow!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kBowAttached)
            Log("Player has attached an arrow to the bow!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kBowDrawn)
            Log("Player has fully drawn the bow!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kBowReleasing)
            Log("Player is releasing the bowstring!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kBowReleased)
            Log("Player has released the bowstring!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kBowNextAttack)
            Log("Player is preparing the next bow attack!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kBowFollowThrough)
            Log("Player is following through with a bow attack!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kFire) Log("Player is firing a weapon!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kFiring)
            Log("Player is in the process of firing!");
        else if (attackState == RE::ATTACK_STATE_ENUM::kFired) Log("Player has fired a weapon!");

        // if (event->GetEventType() == RE::INPUT_EVENT_TYPE::kButton) {
        //     auto* buttonEvent = event->AsButtonEvent();
        //     auto  dxScanCode  = buttonEvent->GetIDCode();
        // }

        return RE::BSEventNotifyControl::kContinue;
    }
};

SKSEPlugin_OnDataLoaded {
    RE::BSInputDeviceManager::GetSingleton()->AddEventSink((&EventSink::instance()));

    SKSE::GetCrosshairRefEventSource()->AddEventSink(&EventSink::instance());

    auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
    if (eventSourceHolder) {
        eventSourceHolder->AddEventSink<RE::TESActivateEvent>(&EventSink::instance());
    } else {
        Log("Failed to get ScriptEventSourceHolder!");
    }
}
