
#pragma once
#include "utilities.h"
#include "TriStat.h"
#include <fstream>
#include <iostream>
	//Taken from SKSE64
//typedef RE::TESForm* (*_LookupFormByID)(RE::FormID id);
//REL::Relocation<_LookupFormByID> LookupFormByID(REL::ID(14461));
//


bool spellCharge = false;
	class DummyActorValue
	{
	public:
		static DummyActorValue* GetSingleton()
		{
			static DummyActorValue* instance;
			if (!instance)
				instance = new DummyActorValue;
			return instance;
		}
		virtual void unk0() {}
		virtual float PreCastGetAVO(RE::ActorValue, __int64*);  //__int64* avOwner, int av);
		virtual void unk1() {}
		virtual float PreCastGetBAV(RE::ActorValue, __int64*);
		inline static REL::Relocation<decltype(&PreCastGetAVO)> _PreCastGetAVO;
		inline static REL::Relocation<decltype(&PreCastGetBAV)> _PreCastGetBVO;
	};
	std::vector<std::string> storageKey{ "HEALTH_BUFFER", "HEALTH_NEGATIVE_BUFFER","HEALTH_HIGHEST_DAMAGE", "HEALTH_RECENT_HIGHEST_DAMAGE", "HEALTH_TOTAL_DAMAGED", "HEALTH_TOTAL_RESTORED",
		"MAGICKA_BUFFER", "MAGICKA_NEGATIVE_BUFFER", "MAGICKA_HIGHEST_DAMAGE", "MAGICKA_RECENT_HIGHEST_DAMAGE", "MAGICKA_TOTAL_DAMAGED", "MAGICKA_TOTAL_RESTORED",
		"STAMINA_BUFFER", "STAMINA__NEGATIVE_BUFFER", "STAMINA_HIGHEST_DAMAGE", "STAMINA_RECENT_HIGHEST_DAMAGE", "STAMINA_TOTAL_DAMAGED", "STAMINA_TOTAL_RESTORED" };
	template <typename T>
void TriStatTracker::bufferUpdate(float mod)
{
	std::map<std::string, std::map<std::string, RE::TESGlobal*>>::iterator it;
	//logger::info("Updating buffer");
	for (it = slccStorage.begin(); it != slccStorage.end(); it++) {
		if (!it->second.contains(storageKey[(int)T::BUFFER]))
			continue;
		RE::TESGlobal* globalVal = it->second[storageKey[(int)T::BUFFER]];
		logger::info("Buffer Update: " + SL::String(globalVal->value));
		if (globalVal) {
			globalVal->value += mod;
		} else {
			logger::critical("BufferUpdate: returned a nullPtr from globalvar storage, this is a severe error that affects the core of the mod.");
		}
	}
}
template <typename T>
void TriStatTracker::restored(float mod) 
{

		std::map<std::string, std::map<std::string, RE::TESGlobal*>>::iterator it;

	for (it = slccStorage.begin(); it != slccStorage.end(); it++) {
			if (!it->second.contains(storageKey[(int)T::TOTAL_RESTORED]))
				continue;
			RE::TESGlobal* glob = it->second[storageKey[(int)T::TOTAL_RESTORED]];
			if (glob) {
				glob->value += mod;

			} else {
				logger::critical("Failed to retrieve a restored value from globals, from mod: "+it->first);
			}
		}
}


template<typename T>
void TriStatTracker::damaged(float mod)
{
	mod = -1 * mod;
	std::map<std::string, std::map<std::string, RE::TESGlobal*>>::iterator it;

	for (it = slccStorage.begin(); it != slccStorage.end(); it++) {

		
		

		if (it->second.contains(storageKey[(int)T::TOTAL_DAMAGED])) {
			RE::TESGlobal* totalDamage = it->second[storageKey[(int)T::TOTAL_DAMAGED]];
			if (totalDamage) {
				totalDamage->value += mod;
			} else {
				logger::critical("Failed to retrieve TOTAL_DAMAGE from mod: " + it->first);
			}
		}
		if (it->second.contains(storageKey[(int)T::HIGHEST_DAMAGE])) {
			RE::TESGlobal* highbuff = it->second[storageKey[(int)T::HIGHEST_DAMAGE]];
			if (highbuff) {
				if (mod > highbuff->value)
					highbuff->value = mod;
			} else {
				logger::critical("Failed to retrieve HIGHEST_DAMAGE from mod: " + it->first);
			}
		}
		if (it->second.contains(storageKey[(int)T::RECENT_HIGHEST_DAMAGE])) {
			RE::TESGlobal* recentbuff = it->second[storageKey[(int)T::RECENT_HIGHEST_DAMAGE]];
			if (recentbuff) {
				if (mod > recentbuff->value)
					recentbuff->value = mod;
			} else {
				logger::critical("Failed to retrieve RECENT_DAMAGE from mod: " + it->first);
			}
		}
		if (it->second.contains(storageKey[(int)T::NEGATIVE_BUFFER])){
			RE::TESGlobal* damagebuff = it->second[storageKey[(int)T::NEGATIVE_BUFFER]];
			if (damagebuff) {
				damagebuff->value += mod;
			} else {
				logger::critical("Failed to retrieve DAMAGE_BUFF from mod: " + it->first);
			}
		}
		
		
		
		
		

		
		

		
	}
}
void UpdateTxt() {
	
	//std::string line;
	//std::ofstream TriStore("Data/SKSE/Plugins/TriStatTrackerData/TriStatTracker.txt");
	
	//if (TriStore.is_open()) {
	//	TriStore << "Health: " << (*g_thePlayer)->GetActorValue(RE::ActorValue::kHealth) << "\n";
	//	TriStore << "Magicka: " << (*g_thePlayer)->GetActorValue(RE::ActorValue::kMagicka) << "\n";
	//	TriStore << "Stamina: " << (*g_thePlayer)->GetActorValue(RE::ActorValue::kStamina) << "\n";
		
	//}
	//TriStore.close();
}
RE::MagicCaster* te;
//RE::ACTOR_VALUE_MODIFIERS::
bool stop{false};
bool mstop{ false };
bool ablock{ false };
bool ranb{ false };
void TriStatTracker::DamageActorValue(RE::PlayerCharacter* pCharacter, int AVModifiers, int actorVal, float mod, RE::TESObjectREFR* a5)
{
	
	
	TriStatTracker* track = TriStatTracker::GetSingleton();
	//
	//SKSE::sendNotification(SL::String("Recieved"+actorVal));
	
	//	SKSE::sendNotification(SL::StringHex(&a5));

	//	
	if (&(*pCharacter) == &(**g_thePlayer)) {
		//sizeof(TESObjectREFR) == 0x98)
		
		//if (a5)
			//logger::info(SL::String(static_cast<int>(a5->GetFormType())));
		
		switch (actorVal) {
		case 24:
			//UpdateTxt();
			//actorVal = 25;
			
			if (ablock && mod < 0) {
				logger::info("Blocked attack - Health");
				logger::info(SL::String(AVModifiers) + "::" + SL::String(actorVal) + "::" + SL::String(mod));
				if (ranb) {
					ablock = false;
					ranb = false;
				} else {
					ranb = true;
				}
			}
			
			if (!track->state)
				break;
			if (mod > 0) {
				track->restored<TriStatTracker::health>(mod);
				track->bufferUpdate<TriStatTracker::health>(mod);
			} else
				track->damaged<TriStatTracker::health>(mod);
			//AVal = TriStatTracker::HEALTH_EFFICIENCY;

			break;
		case 25:
			//UpdateTxt();
			
			if (te){
				if (te->currentSpell) {
					logger::info(te->currentSpell->GetFullName());
					logger::info(SL::String(AVModifiers) + "::" + SL::String(actorVal) + "::" + SL::String(mod));
					actorVal = 26;
				}
			}
			if (!track->state)
				break;
			if (mod > 0) {
				track->restored<TriStatTracker::magicka>(mod);
				track->bufferUpdate<TriStatTracker::magicka>(mod);
			} else
				track->damaged<TriStatTracker::magicka>(mod);
			//logger::info("Magicka: " + SL::String(mod));
			//logger::info("AV: Magicka "+avV);
			//AVal = TriStatTracker::MAGICKA_EFFICIENCY;
			break;
		case 26:
			//UpdateTxt();
			//actorVal = 25;
			//if (ablock) {}
			
			if (ablock && mod < 0) {
				logger::info("Blocked attack - Stamina");
				logger::info(SL::String(AVModifiers) + "::" + SL::String(actorVal) + "::" + SL::String(mod));
				if (ranb) {
					ablock = false;
					ranb = false;
				} else {
					ranb = true;
				}
			}
			
			if (stop && pCharacter->GetAttackingWeapon()) {
				logger::info(pCharacter->GetAttackingWeapon()->GetDisplayName());
				logger::info(SL::String(AVModifiers) + "::" + SL::String(actorVal) + "::" + SL::String(mod));
				stop = false;
			}
			if (mstop) {
				logger::info("Sprint");
				logger::info(SL::String(AVModifiers) + "::" + SL::String(actorVal) + "::" + SL::String(mod));
			}
			if (!track->state)
				break;
			if (mod > 0) {
				track->restored<TriStatTracker::stamina>(mod);
				track->bufferUpdate<TriStatTracker::stamina>(mod);
			} else
				track->damaged<TriStatTracker::stamina>(mod);
			//logger::info("AV: Stamina ");
			//AVal = TriStatTracker::STAMINA_EFFICIENCY;

			break;
		default:
			//logger::info("Unhandled AV, ignoring");
			break;
		}
	}
	//Write the 3 updated stats to txt file.
	te = NULL;
	_DamageActorVal(pCharacter, AVModifiers, actorVal, mod, a5);
	return;
}
bool forceAllow{ false };
float DummyActorValue::PreCastGetAVO(RE::ActorValue av, __int64* avO)
{  //__int64* avOwner, int av) {
	
	REL::Relocation<uintptr_t> caster{ reinterpret_cast<uintptr_t>(avO) };
	REL::Relocation<RE::Actor*> kActor{ *reinterpret_cast<uintptr_t*>(caster.address() + 0x0b8) };
	logger::info(SL::StringHex(kActor.address()));
	//logger::info(SL::StringHex(&(*caster)));
	//logger::info(SL::StringHex(&(*SI)));
	forceAllow = false;
	
	return kActor->GetActorValue(av);
	//logger::info(SL::String(checkpc));
	//_PreCastGetAVO(avOwner, av);
	
}
float DummyActorValue::PreCastGetBAV(RE::ActorValue av , __int64* avO) {
	REL::Relocation<uintptr_t> caster{ reinterpret_cast<uintptr_t>(avO) };
	REL::Relocation<RE::Actor*> kActor{ *reinterpret_cast<uintptr_t*>(caster.address() + 0x0b8) };
	logger::info(SL::StringHex(kActor.address()));
	return kActor->GetActorValue(av);
}
void TriStatTracker::SpellCast(RE::MagicCaster* unk1, float unk2)
{
	
	RE::MagicItem* ME = unk1->currentSpell;
	if (ME) {
		logger::info("called spellcast");
		//states: 3 = Holding, 4 = about to cast, 5 = unk, 6 = cast
		forceAllow = true;
		if (static_cast<int>(unk1->state.get()) != 3)
			te = unk1;
		//logger::info(SL::String(ME->GetFullName()));
		//logger::info(SL::String(unk1->currentSpellCost));
		//logger::info(SL::String(unk2));
		if (unk1 && unk2) {}
		//Until I understand assembly and the stack more, I don't feel comfortable doing anything more advanced than this hackjob
		//to get a spell to cast.
	}
	
	_SpellCast(unk1, unk2);
}
//unk1 - 0xB0
char TriStatTracker::PreCast(__int64* magicCaster) {
	//float magicka = *((*g_thePlayer)->avStorage.baseValues[RE::ActorValue::kStamina]);
		//quickly set magicka to what is required of the spell.
	//logger::info(*((*g_thePlayer)->avStorage.baseValues[RE::ActorValue::kStamina]));
	//*((*g_thePlayer)->avStorage.baseValues[RE::ActorValue::kStamina]) = 100;
	logger::info("Called PreCast");
	char ret = _PreCast(magicCaster);
	//*((*g_thePlayer)->avStorage.baseValues[RE::ActorValue::kStamina]) = 0;
	return ret;
}
void TriStatTracker::SpellDec(RE::TESForm* Caster, int unk2, int unk3, float unk4, __int64* unk5)
{
	logger::info("Decrease");
	if (Caster) {
		logger::info(Caster->GetName());
		
		
	}

	_SpellDec(Caster, unk2, unk3, unk4, unk5);
}
	TriStatTracker::TriStatTracker()
{
	logger::info("Starting plugin");
	if (varsLoaded) {
		logger::info("Vars are loaded but constructor was called anyway...?");
		return;
	}
	//RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESMagicEffectApplyEvent>(this);
	//RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESActiveEffectApplyRemoveEvent>(this);
	RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESHitEvent>(this);
	RE::PlayerCharacter** Player = g_thePlayer.get();
	RE::BSTSmartPointer<RE::BSAnimationGraphManager> grphmgr;
	(*Player)->GetAnimationGraphManager(grphmgr);
	logger::info(SL::String(grphmgr.get()->graphs.size()));
	RE::BSTSmartPointer<RE::BShkbAnimationGraph> graph = grphmgr.get()->graphs[0];
	graph->AddEventSink<RE::BSAnimationGraphEvent>(this);
	//*/
	hdle = RE::TESDataHandler::GetSingleton();

	//logger::info(SL::String(slccStorage.size()));
	logger::info("Hooking RestoreAV");
	
	//Hook Restore/DamageActorValue

	
	//RE::ActorMagicCaster* hold{ (*g_thePlayer)->magicCasters[1] };
	//logger::info(SL::String(&(*hold)));
	
	//*
	REL::Relocation<RE::Actor**> playerAV{ REL::ID(517014) };

	uintptr_t ne = reinterpret_cast<uintptr_t>(&(**playerAV));
	//REL::Relocation<RE::ActorValue**> pav {ne + 0xB0};
	REL::Relocation<RE::ActorMagicCaster**> pav{ ne + 0x1A8 };
	//*(*(ActorMagicCaster + 0B8) + 0B0) + 30 < Hooking this should give what we need.
	REL::Relocation<uintptr_t> pav2{ reinterpret_cast<uintptr_t>(&(**pav)) };
	logger::info(SL::StringHex(pav2.address()));
	
	REL::Relocation<uintptr_t**> pav3{ pav2.address() + 0x0B8 };
	logger::info(SL::StringHex(pav3.address()));
	REL::Relocation<uintptr_t> pav6{ reinterpret_cast<uintptr_t>(&**pav3) };
	logger::info(SL::String(pav6.address()));
	REL::Relocation<uintptr_t**> pav4{ pav6.address() + 0x0B0 };
	logger::info(SL::StringHex(pav4.address()));
	logger::info(SL::StringHex(&(**pav4)));
	REL::Relocation<uintptr_t> pav7{ reinterpret_cast<uintptr_t>(&(**pav4)) };
	//_SpellDec = pav7.write_vfunc(6, reinterpret_cast<uintptr_t>(SpellDec)); //This is actually restoreactorval T.T;
	//_PreCastGetAVO = pav7.write_vfunc(1, reinterpret_cast<uintptr_t>(PreCastGetAVO));
	logger::info(SL::StringHex(pav2.address()));
	REL::Relocation<uintptr_t> pav9{ *reinterpret_cast<uintptr_t*>(pav2.address()) };
	//_SpellCast = pav9.write_vfunc(29, reinterpret_cast<uintptr_t>(SpellCast));
	
	//_PreCast = pav9.write_vfunc(3, PreCast);
	//_SpellDec = pav2.write_vfunc(29, reinterpret_cast<uintptr_t>(SpellCast));
	//
	//*/
	//REL::Relocation<uintptr_t> pav5{ reinterpret_cast<uintptr_t>(&(*pav4)) }; //Then offset 30
	//logger::info(SL::StringHex(pav5.address()));
	
	//*/

	//REL::Relocation<std::uintptr_t> vTab{ playerAV.address(), 0x30 };
	//_DamageActorVal = pav2.write_vfunc(6, reinterpret_cast<uintptr_t>(DamageActorValue));
	
	
	//uintptr_t hol = pav2.write_vfunc(29, SpellCast);
	//logger::info(SL::StringHex(&hol));
	//logger::info(SL::StringHex(&_DamageActorVal));
	//= trampoline.write_call<5>(vTab, DamageActorValue)
	//_DamageActorVal = trampoline.write_call<5>(REL::ID(34284).address() + 0x9E, DamageActorValue);
	
	//auto& trampoline = SKSE::GetTrampoline();
	//_DamageActorVal = trampoline.write_call<5>(REL::ID(34284).address() + 0x117, DamageActorValue); //Damage from actor
	REL::Relocation<uintptr_t> pav10(*reinterpret_cast<uintptr_t*>(pav9.address() + 0x50));
	logger::info(SL::StringHex(pav10.address() + 0x1c6));
	//_PreCastGetAVO = trampoline.write_branch<5>(REL::ID(33364).address() + 0x1BC, PreCastGetAVO);

	//_DamageActorVal = trampoline.write_call<5>(REL::ID(36345).address() + 0x11D, DamageActorValue); //damage from actor?

	//_DamageActorVal = trampoline.write_call<5>(REL::ID(37522).address() + 0x14, DamageActorValue); //Generic call
	
	struct Code : Xbyak::CodeGenerator
	{
		Code(uintptr_t addr)
		{
			mov(rcx, addr); //Replace with my dummy class.
			mov(edx, ebp); //Fix bytes
			mov(r8, rbx); //Store MagicCaster in r8 to be able to get the actorvalueowner of the caller.
			//r8 does not appear to be used for anything important before the next time it is filled, so should be safe.
			//nop();
			nop();
			
		}
	};
	Code cc { reinterpret_cast<uintptr_t>(DummyActorValue::GetSingleton()) };
	cc.ready();
	logger::info(SL::StringHex(&(*DummyActorValue::GetSingleton())));
	REL::safe_write(REL::ID(33364).address() + 0x19F, cc.getCode(), cc.getSize());
	//trampoline.write_branch<6>(REL::Module::get().base() + 0x542258, trampoline.allocate(cc));
	
	//_DamageActorVal = trampoline.write_call<5>(REL::ID(34286).address() + 0x2D1, DamageActorValue); //Calls DAV constantly
	//_DamageActorVal = trampoline.write_call<5>(REL::ID(37633).address() + 601, DamageActorValue);
	// 
	//_DamageActorVal = trampoline.write_call<5>((REL::Module::get().base() + 0x56791E), DamageActorValue);
	//_DamageActorVal = trampoline.write_call<5>((REL::Module::get().base() + 0x567997), DamageActorValue);
	//_DamageActorVal = trampoline.write_call<5>((REL::Module::get().base() + 0x5D641D), DamageActorValue);
	//_DamageActorVal = trampoline.write_call<5>((REL::Module::get().base() + 0x0621104), DamageActorValue);
	
	// 
	// 
	//_DamageActorVal = trampoline.write_call<5>((REL::Module::get().base() + 0x567D51), DamageActorValue);  567A80+2D1
	//_DamageActorVal = trampoline.write_call<5>((REL::Module::get().base() + 0x626A01), DamageActorValue);
						//	_DamageActorVal = trampoline.write_call<5>((REL::Module::get().base() + 0x626A01), DamageActorValue);
	//UpdateTxt();

	
		
	varsLoaded = true;
}
TriStatTracker::TriStatTracker(TriStatTracker&) {}
RE::BSEventNotifyControl TriStatTracker::ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) 
{
	//SKSE::sendNotification("OnEffectStart");
	//logger::info("OnEffectStart");
	if (a_event && a_eventSource) {
		//RE::EffectSetting* mi2 = skyrim_cast<RE::EffectSetting*>(LookupFormByID(a_event->magicEffect));
	}
	
	//logger::info("Received event");
	return RE::BSEventNotifyControl::kContinue;
}

bool isBlocking{ false };
RE::BSEventNotifyControl TriStatTracker::ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
{
	if (a_eventSource) {}
	if (&(*a_event->holder) == &(**g_thePlayer)) {
		//logger::info(a_event->tag.c_str());
		//if (strcmp(a_event->tag.c_str(), "blockStartOut") == 0)
			//isBlocking = true;
		//if (strcmp(a_event->tag.c_str(), "blockStop") == 0)
			//isBlocking = false;
		//if (strcmp(a_event->tag.c_str(), "blockHitStop") == 0)
		//	ablock = true;
		if (strcmp(a_event->tag.c_str(), "HitFrame") == 0)
			stop = true;
		//if (strcmp(a_event->tag.c_str(), "tailMTIdle") == 0) {
		//	logger::info("Cast inter Stop");
		//	spellCharge = false;
		//}
		if (strcmp(a_event->tag.c_str(), "tailSprint") == 0)
			mstop = true;
		if (strcmp(a_event->tag.c_str(), "EndAnimatedCameraDelta") == 0)
			mstop = false;
	}
	/*
	if (strcmp(a_event->tag.c_str(), "BeginCastRight") == 0) {
		RE::SpellItem* rSpell = skyrim_cast<RE::SpellItem*>((*g_thePlayer)->GetEquippedObject(false));
		if (rSpell) {
			
			if (rSpell->GetCastingType() == RE::MagicSystem::CastingType::kFireAndForget) {
				spellCharge = true;
			}
		}
	}
	if (strcmp(a_event->tag.c_str(), "BeginCastLeft") == 0) {
		RE::SpellItem* lSpell = skyrim_cast<RE::SpellItem*>((*g_thePlayer)->GetEquippedObject(true));
		if (lSpell) {
			//logger::info(lSpell->data.castingPerk->GetName());
			if (lSpell->GetCastingType() == RE::MagicSystem::CastingType::kFireAndForget) {
				spellCharge = true;
			}
		}
	}
	*/
	return RE::BSEventNotifyControl::kContinue;
}
RE::BSEventNotifyControl TriStatTracker::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) {
	if (a_eventSource) {}
	if (a_event) {
		if (&(*a_event->target) == &(**g_thePlayer)) {
			logger::info(SL::String(static_cast<int>(a_event->flags.get())));
			if (1 << static_cast<int>(a_event->flags.get()) & (1 << 8 | 1 << 9)) {
				ablock = true;
			}
		}
	}
	return RE::BSEventNotifyControl::kContinue;
}
RE::BSEventNotifyControl TriStatTracker::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) {
	if (a_eventSource) {}
	if (a_event) {
		logger::info(SL::StringHex(a_event->baseObject));
	}
	return RE::BSEventNotifyControl::kContinue;
}

bool StartPlugin(RE::StaticFunctionTag*) {
	//SKSE::ModCallbackEvent tests;
	//RE::BSTEventSource<SKSE::ModCallbackEvent>* ModEvn = (RE::BSTEventSource<SKSE::ModCallbackEvent>*)SKSE::GetMessagingInterface()->GetEventDispatcher(SKSE::MessagingInterface::Dispatcher::kModEvent);
	//tests.eventName = "SKSEModEventTest";
	//tests.numArg = 0.0f;
	//tests.strArg = "";
	//tests.sender = NULL;
	//ModEvn->SendEvent(&tests);
	TriStatTracker* track = TriStatTracker::GetSingleton();

	
	//logger::info((**g_thePlayer).avStorage.baseValues.actorValues.c_str());
	//if (!((track->slccStorage.size()) > 0))
		//return false; 
	track->state = true;
	return true;
	
	
}
void StopPlugin(RE::StaticFunctionTag*) {
	
	TriStatTracker::GetSingleton()->state = false;
}

bool GetState(RE::StaticFunctionTag*) {
	return TriStatTracker::GetSingleton()->state;
}
bool AddStorageSingleGlobal(RE::StaticFunctionTag*, RE::TESGlobal* toAdd, int location, std::string mapKey) 
{
	if (!toAdd) {
		return false;
	}
	TriStatTracker* track = TriStatTracker::GetSingleton();
	track->slccStorage[mapKey][storageKey[location]] = toAdd;
	return true;
}
void RemoveStorageSingleGlobal(RE::StaticFunctionTag*, int location, std::string mapKey)
{
	TriStatTracker* track = TriStatTracker::GetSingleton();
	track->slccStorage[mapKey].erase(track->slccStorage[mapKey].find(storageKey[location]));

}
bool AddStorageGlobals(RE::StaticFunctionTag*, RE::BGSListForm* globalVals, std::string mapKey)
{
	
	
	if (globalVals->forms.size() == 18) {
		
		TriStatTracker* track = TriStatTracker::GetSingleton();
		for (uint32_t i = 0; i < globalVals->forms.size(); i++) {
			logger::info(SL::String(i));
			if (!globalVals->forms[i]) {
				return false;
			}
			track->slccStorage[mapKey][storageKey[i]] = skyrim_cast<RE::TESGlobal*>(globalVals->forms[i]);
				
			
				
		}
		return true;
			
	} else {
		return false;
	}
}
void RemoveStorageGlobals(RE::StaticFunctionTag*, std::string mapKey) {
	
	TriStatTracker* track = TriStatTracker::GetSingleton();
	track->slccStorage.erase(mapKey);
}
bool RegisterFuncs(RE::BSScript::IVirtualMachine* a_vm)
{
	a_vm->RegisterFunction("StartTracker", "TST_SKSE", StartPlugin);
	a_vm->RegisterFunction("StopTracker", "TST_SKSE", StopPlugin);
	a_vm->RegisterFunction("GetTrackerState", "TST_SKSE", GetState);
	a_vm->RegisterFunction("AddSingleGlobal", "TST_SKSE", AddStorageSingleGlobal);
	a_vm->RegisterFunction("AddStorageGlobals", "TST_SKSE", AddStorageGlobals);
	a_vm->RegisterFunction("RemoveStorageGlobals", "TST_SKSE", RemoveStorageGlobals);
	a_vm->RegisterFunction("RemoveSingleGlobal", "TST_SKSE", RemoveStorageSingleGlobal);
	return true;
}

//typedef void(__fastcall* DamageActorValue)(RE::Actor* pCharacter, int AVModifiers, RE::ActorValue actorVal, float mod, std::uintptr_t a5);




