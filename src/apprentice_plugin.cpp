#include <SkyrimScripting/Plugin.h>

class EventSink : public RE::BSTEventSink<SKSE::CrosshairRefEvent>,
                  public RE::BSTEventSink<RE::TESActivateEvent> {
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
};

SKSEPlugin_Entrypoint {
    SKSE::GetCrosshairRefEventSource()->AddEventSink(&EventSink::instance());

    auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
    if (eventSourceHolder) {
        eventSourceHolder->AddEventSink<RE::TESActivateEvent>(&EventSink::instance());
    } else {
        Log("Failed to get ScriptEventSourceHolder!");
    }
}
