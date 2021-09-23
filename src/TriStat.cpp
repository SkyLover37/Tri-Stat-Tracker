
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
		virtual float PreCastGetAVO(RE::ActorValue, __int64*, UINT64 kType);  //__int64* avOwner, int av);
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
RE::TESGlobal* GetReplaceActorValue(int type)
{
	TriStatTracker* track = TriStatTracker::GetSingleton();
	switch (type) {
	case 0:
		return track->MVStorage[static_cast<int>(T::REPLACE)];
	case 1:
		return track->MVStorage[static_cast<int>(T::REDIRECT)];
	case 2:
		return track->MVStorage[static_cast<int>(T::REV_REDIRECT)];
	}
	return NULL;
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
bool HasKeywordString(T* toCheck, std::string name) {

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
	//Need to hook all the individual calls to this from their respective sources to better accurately tell what the damage is actually from. 
	
	TriStatTracker* track = TriStatTracker::GetSingleton();
	//
	//SKSE::sendNotification(SL::String("Recieved"+actorVal));
	
	//	SKSE::sendNotification(SL::StringHex(&a5));

	//	
	RE::ActorValue AV2{};
	RE::TESGlobal* glob{};
	RE::TESForm* obj{};
	if (&(*pCharacter) == &(**g_thePlayer)) {
		//sizeof(TESObjectREFR) == 0x98)
		
		//if (a5)
			//logger::info(SL::String(static_cast<int>(a5->GetFormType())));
		
		switch (actorVal) {
		case 24:
			//UpdateTxt();
			//actorVal = 25;
			if (glob) {
			} else 
				if ((glob = GetReplaceActorValue<TriStatTracker::health>(0)) != NULL) {}
			
			else
			
				if ((glob = GetReplaceActorValue<TriStatTracker::health>(1)) != NULL) {}
			
			else
			
				if ((glob = GetReplaceActorValue<TriStatTracker::health>(2)) != NULL) {}
			
			if (ablock && mod < 0 && pCharacter->currentProcess && pCharacter->currentProcess->middleHigh && pCharacter->currentProcess->middleHigh->leftHand 
				&& (obj = pCharacter->currentProcess->middleHigh->leftHand->object) != NULL && (glob = track->UniqueItemStorageGlobal[obj]) != NULL) {
				logger::info("Blocked attack - Stamina");
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
			if (glob) {
			} else if ((glob = GetReplaceActorValue<TriStatTracker::magicka>(0)) != NULL) {
			}

			else

				if ((glob = GetReplaceActorValue<TriStatTracker::magicka>(1)) != NULL) {
			}

			else

				if ((glob = GetReplaceActorValue<TriStatTracker::magicka>(2)) != NULL) {
			}
			
			if (te){
				if (te->currentSpell) {
					logger::info(te->currentSpell->GetFullName());
					logger::info(SL::String(AVModifiers) + "::" + SL::String(actorVal) + "::" + SL::String(mod));
					AV2 = track->UniqueItemStorageInt[skyrim_cast<RE::TESForm*>(te->currentSpell)];
					if (static_cast<int>(AV2) >= 24 && static_cast<int>(AV2) <= 26) {
						actorVal = static_cast<int>(AV2);
						logger::info("return was: " + SL::String(static_cast<int>(AV2)));
					} else
						(glob = track->UniqueItemStorageGlobal[skyrim_cast<RE::TESForm*>(te->currentSpell)]);
					//_StopSpellCast(te);
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
			//actorVal = 25;
			if (glob) {
			} else if ((glob = GetReplaceActorValue<TriStatTracker::stamina>(0)) != NULL) {
			}

			else

				if ((glob = GetReplaceActorValue<TriStatTracker::stamina>(1)) != NULL) {
			}

			else

				if ((glob = GetReplaceActorValue<TriStatTracker::stamina>(2)) != NULL) {
			}
			
			if (ablock && mod < 0 && pCharacter->currentProcess && pCharacter->currentProcess->middleHigh && pCharacter->currentProcess->middleHigh->leftHand 
				&& (obj = pCharacter->currentProcess->middleHigh->leftHand->GetOwner()) != NULL && (glob = track->UniqueItemStorageGlobal[obj]) != NULL)
			{
				logger::info("Blocked attack - Stamina" + SL::String(obj->GetName()) + "::" + SL::String(AVModifiers) + "::" + SL::String(actorVal) + "::" + SL::String(mod));
				if (ranb) {
					ablock = false;
					ranb = false;
				} else {
					ranb = true;
				}
				break;
			}
			if (stop && pCharacter->GetAttackingWeapon()) {
				logger::info("Name: " + SL::String(pCharacter->GetAttackingWeapon()->object->GetName()));
				AV2 = track->UniqueItemStorageInt[skyrim_cast<RE::TESForm*>((pCharacter->GetAttackingWeapon())->object)];
				if (static_cast<int>(AV2) >= 24 && static_cast<int>(AV2) <= 26) {
					actorVal = static_cast<int>(AV2);
					logger::info("return was: " + SL::String(static_cast<int>(AV2)));
				}
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
	
	if (!glob) {
		_DamageActorVal(pCharacter, AVModifiers, actorVal, mod, a5);
		if (te && static_cast<int>(AV2) >= 24 && static_cast<int>(AV2) <= 26 && pCharacter->GetActorValue(AV2) <= 0.0)
			_StopSpellCast(te);
	}

	else {
		logger::info("Reducing global");
		float val = glob->value;
		val += mod;
		logger::info("Global val: " + SL::String(val));
		if (val < 0.0)
			val = 0;
		glob->value = val;
		if (val == 0 && te)
			_StopSpellCast(te);
	}
	te = NULL;
	return;
}
bool forceAllow{ false };
float DummyActorValue::PreCastGetAVO(RE::ActorValue av, __int64* avO, UINT64)
{  //__int64* avOwner, int av) {
	RE::MagicItem* spell{};
	//logger::info(SL::String(hType));
	//logger::info(SL::String(hType == 1));
	REL::Relocation<RE::MagicCaster*> caster{ reinterpret_cast<uintptr_t>(avO) };
	REL::Relocation<RE::Actor*> kActor{ *reinterpret_cast<uintptr_t*>(caster.address() + 0x0b8) };

	TriStatTracker* track = TriStatTracker::GetSingleton();
	RE::TESGlobal* glob{};
	if (av == RE::ActorValue::kMagicka) {
		//logger::info(SL::StringHex(kActor.address()));
		RE::ActorValue tempAV{};
		if (caster.get() && (spell = caster->currentSpell) != NULL) {
			tempAV = track->UniqueItemStorageInt[skyrim_cast<RE::TESForm*>(spell)];
			 if (static_cast<int>(tempAV) >= 24 && static_cast<int>(tempAV) <= 26)
			{
				logger::info("Form is a unqiue item");
				av = tempAV;
			} else if ((glob = track->UniqueItemStorageGlobal[skyrim_cast<RE::TESForm*>(spell)]) != NULL)
				logger::info("Form is unique global item");
			
			//states: 3 = Holding, 4 = about to cast, 5 = unk, 6 = cast
			
			if (static_cast<int>(caster->state.get()) != 3)
				te = caster.get();	
				
		}
		

	}
	else {  //If not magic caster, convert avO as if it's actor
		kActor = REL::Relocation<RE::Actor*>{ reinterpret_cast<uintptr_t>(avO) };  //assume avO is magicCaster first.
		//logger::info(SL::StringHex(kActor.address()));
	}
	
	
	switch (av) {
	case RE::ActorValue::kHealth:
		if (glob)
			return glob->value;
		if ((glob = GetReplaceActorValue<TriStatTracker::health>(0)) != NULL && glob->value > 0.0)
			return glob->value;
		if ((glob = GetReplaceActorValue<TriStatTracker::health>(1)) != NULL && glob->value > 0.0)
			return glob->value;
		if ((glob = GetReplaceActorValue<TriStatTracker::health>(2)) != NULL && glob->value > 0.0)
			return glob->value;
	case RE::ActorValue::kMagicka:
		if (glob)
			return glob->value;
		if ((glob = GetReplaceActorValue<TriStatTracker::magicka>(0)) != NULL && glob->value > 0.0)
			return glob->value;
		if ((glob = GetReplaceActorValue<TriStatTracker::magicka>(1)) != NULL && glob->value > 0.0)
			return glob->value;
		if ((glob = GetReplaceActorValue<TriStatTracker::magicka>(2)) != NULL && glob->value > 0.0)
			return glob->value;
	case RE::ActorValue::kStamina:
		if (glob)
			return glob->value;
		if ((glob = GetReplaceActorValue<TriStatTracker::stamina>(0)) != NULL && glob->value > 0.0)
			return glob->value;
		if ((glob = GetReplaceActorValue<TriStatTracker::stamina>(1)) != NULL && glob->value > 0.0)
			return glob->value;
		if ((glob = GetReplaceActorValue<TriStatTracker::stamina>(2)) != NULL && glob->value > 0.0)
			return glob->value;
	}
	
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
// 0: Sprint check
// 1: Magick focus chech
// 2: MAgick continuous check
// 3: Block check
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

	//uintptr_t ne = reinterpret_cast<uintptr_t>(&(**playerAV));
	
	//REL::Relocation<RE::ActorValue**> pav {ne + 0xB0};
	/*
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
	REL::Relocation<uintptr_t> pav10(*reinterpret_cast<uintptr_t*>(pav9.address() + 0x50));
	logger::info(SL::StringHex(pav10.address() + 0x1c6));
	REL::Relocation<uintptr_t> pav9{ *reinterpret_cast<uintptr_t*>(pav2.address()) };
	_SpellCast = pav9.write_vfunc(29, reinterpret_cast<uintptr_t>(SpellCast));
	*/
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
	
	auto& trampoline = SKSE::GetTrampoline();
	
	
	//_PreCastGetAVO = trampoline.write_branch<5>(REL::ID(33364).address() + 0x1BC, PreCastGetAVO);
	//Shield AV check: (626400 + 0x8B5) 626CB5 - 626CC8 ID: 37633
	//Run AV check = (63CAA0 + 0x204) 63CCA4 - 63CCB3 ID: 38046
	// Run AvCheck = (6B8F20 + 0x103) 6B9023 - 6B9032 0xF ID: 39673
	// Run AVCheck = (709580 + 0x1F) 70959F - 7095AE 0xA ID: 41358
	// Allow hold to cast =  (541FC0 + 0x254) 542214 - 542228 0x14  ID: 33362
	// SpellCAst call to AV = (541FC0 + 0x1B0) 542170 - 542184 0x14 ID: 33362
	//Stop casting at 0 = 6214B4
	struct alignFocus : Xbyak::CodeGenerator  //RAX, EDX
	{
		alignFocus()
		{
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			
		}
	};
	struct hookFocus : Xbyak::CodeGenerator
	{
		hookFocus(uintptr_t addr, uintptr_t rtn)
		{
			Xbyak::Label rtnLbl;

			mov(rax, ptr [ addr ]);
			mov(edx, 0x19);
			mov(r9, 0x1);
			mov(r8, rbx);
			jmp(ptr[rip + rtnLbl]);
			L(rtnLbl);
			dq(rtn);
		}
	};
	alignFocus af{};
	af.ready();
	hookFocus hf{ reinterpret_cast<uintptr_t>(DummyActorValue::GetSingleton()), REL::ID(33362).address() + 0x254 + 0x14 };
	REL::safe_write(REL::ID(33362).address() + 0x254, af.getCode(), af.getSize());
	trampoline.write_branch<6>(REL::ID(33362).address() + 0x254, trampoline.allocate(hf));
	struct alignSprint : Xbyak::CodeGenerator //RAX, EDX
	{
		alignSprint()
		{
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
		}
	};
	struct hookSprint : Xbyak::CodeGenerator
	{
		hookSprint(uintptr_t addr, uintptr_t rtn) {
			Xbyak::Label rtnLbl;

			mov(rax, ptr[addr]);
			mov(edx, 0x1A);
			mov(r9, 0x0);
			mov(r8, rcx);
			jmp(ptr[rip + rtnLbl]);
			L(rtnLbl);
			dq(rtn);
		}
	};
	alignSprint as{};
	as.ready();
	hookSprint hs{ reinterpret_cast<uintptr_t>(DummyActorValue::GetSingleton()), REL::ID(39673).address() + 0x103 + 0xA };
	REL::safe_write(REL::ID(39673).address() + 0x103, as.getCode(), as.getSize());
	trampoline.write_branch<6>(REL::ID(39673).address() + 0x103, trampoline.allocate(hs));
	struct hookSprint2 : Xbyak::CodeGenerator
	{
		hookSprint2(uintptr_t addr, uintptr_t rtn)
		{
			Xbyak::Label rtnLbl;

			mov(rax, ptr[addr]);
			mov(edx, 0x1A);
			mov(r9, 0x0);
			mov(r8, rdi);
			jmp(ptr[rip + rtnLbl]);
			L(rtnLbl);
			dq(rtn);
		}
	};
	hookSprint2 hs2{ reinterpret_cast<uintptr_t>(DummyActorValue::GetSingleton()), REL::ID(38046).address() + 0x204 + 0xF };
	REL::safe_write(REL::ID(38046).address() + 0x204, as.getCode(), as.getSize());
	trampoline.write_branch<6>(REL::ID(38046).address() + 0x204, trampoline.allocate(hs2));
	struct hookSprint3 : Xbyak::CodeGenerator
	{
		hookSprint3(uintptr_t addr, uintptr_t rtn)
		{
			Xbyak::Label rtnLbl;

			mov(rax, ptr[addr]);
			mov(edx, 0x1A);
			mov(r9, 0x0);
			mov(r8, rcx);
			jmp(ptr[rip + rtnLbl]);
			L(rtnLbl);
			dq(rtn);
		}
	};
	hookSprint3 hs3{ reinterpret_cast<uintptr_t>(DummyActorValue::GetSingleton()), REL::ID(41358).address() + 0x1F + 0xF };
	REL::safe_write(REL::ID(41358).address() + 0x1F, as.getCode(), as.getSize());
	trampoline.write_branch<6>(REL::ID(41358).address() + 0x24, trampoline.allocate(hs3));
	struct alignBlock : Xbyak::CodeGenerator//RAX, EDX
	{
		alignBlock()
		{
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
		}
	};
	struct hookBlock : Xbyak::CodeGenerator
	{
		hookBlock(uintptr_t addr, uintptr_t rtn)
		{
			Xbyak::Label rtnLbl;

			mov(rax, ptr[addr]);
			mov(edx, 0x1A);
			mov(r8, r14);
			mov(r9, 0x3);
			jmp(ptr[rip + rtnLbl]);
			L(rtnLbl);
			dq(rtn);
		}
	};
	alignBlock ab{};
	ab.ready();
	hookBlock hb{ reinterpret_cast<uintptr_t>(DummyActorValue::GetSingleton()), REL::ID(37633).address() + 0x8B5 + 0xC };
	REL::safe_write(REL::ID(37633).address() + 0x8B5, ab.getCode(), ab.getSize());
	trampoline.write_branch<6>(REL::ID(37633).address() + 0x8B5, trampoline.allocate(hb));
	struct alignCaster : Xbyak::CodeGenerator
	{
		alignCaster() {
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
		}
	};
	struct Code : Xbyak::CodeGenerator
	{
		Code(uintptr_t addr, uintptr_t rtn)
		{
			Xbyak::Label artn;
			mov(rcx, addr); //Replace with my dummy class.
			mov(edx, ebp);
			mov(r8, rbx); //Store MagicCaster in r8 to be able to get the actorvalueowner of the caller.
			//r8 does not appear to be used for anything important before the next time it is filled, so should be safe.
			mov(r9, 0x2);
			//nop();
			jmp(ptr[rip + artn]);
			L(artn);
			dq(rtn);
		}
	};
	alignCaster aC{};
	aC.ready();
	Code cc{ reinterpret_cast<uintptr_t>(DummyActorValue::GetSingleton()), REL::ID(33364).address() + 0x19F + 0x7};
	cc.ready();
	logger::info(SL::StringHex(&(*DummyActorValue::GetSingleton())));
	REL::safe_write(REL::ID(33364).address() + 0x19F, aC.getCode(), aC.getSize());
	trampoline.write_branch<6>(REL::ID(33364).address() + 0x19F, trampoline.allocate(cc));
	struct alignBy17 : Xbyak::CodeGenerator
	{
		alignBy17()
		{
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
		}
	};
	struct hookContinue : Xbyak::CodeGenerator
	{
		hookContinue(uintptr_t addr, uintptr_t rtn)
		{
			Xbyak::Label rtnLbl;

			mov(rax, ptr[addr]);
			mov(edx, r15d);
			mov(r9, 0x2);
			mov(r8, rbx);
			jmp(ptr[rip + rtnLbl]);
			L(rtnLbl);
			dq(rtn);
		}
	};
	// SpellCAst call to AV = (541FC0 + 0x1B0) 542170 - 542184 0x14 ID: 33362
	alignBy17 a17{};
	aC.ready();
	hookContinue hc{ reinterpret_cast<uintptr_t>(DummyActorValue::GetSingleton()), REL::ID(33362).address() + 0x1B0 + 0x14 };
	hc.ready();
	logger::info(SL::StringHex(&(*DummyActorValue::GetSingleton())));
	REL::safe_write(REL::ID(33362).address() + 0x1B0, a17.getCode(), a17.getSize());
	trampoline.write_branch<6>(REL::ID(33362).address() + 0x1B0, trampoline.allocate(hc));


	struct alignPowerAttk : Xbyak::CodeGenerator
	{
		alignPowerAttk()
		{
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();

		};
	};
	struct Code2 : Xbyak::CodeGenerator
	{
		Code2(uintptr_t addr, uintptr_t rtn) {
			
			Xbyak::Label hookaddr;
			Xbyak::Label returnaddr;
			//L(hookaddr);
			//L(".call");
			//acall;
			//L(".cmp");
			//comp;
			mov(rax, ptr[addr]);
			mov(r8, rbx);
			mov(r9, 0x4);
			mov(edx, 0x1a);
			jmp(ptr[rip + returnaddr]);

			L(hookaddr);
			dq(addr);

			L(returnaddr);
			dq(rtn);  //0x16);

			
			

			//jmp(hookaddr.getAddress());
		}
	};
	Code2 cc2{ reinterpret_cast<uintptr_t>(DummyActorValue::GetSingleton()), REL::ID(38047).address() + 0xC8 + 0xc };  //, REL::Module::get().base() + 0x63D07F
	cc2.ready();
	alignPowerAttk apa{};
	apa.ready();
	REL::safe_write(REL::ID(38047).address() + 0xC8, apa.getCode(), apa.getSize());
	trampoline.write_branch<6>(REL::ID(38047).address() + 0xC8, trampoline.allocate(cc2));
	

	_DamageActorVal = trampoline.write_call<5>(REL::ID(34284).address() + 0x117, DamageActorValue);  //Damage from actor
	_DamageActorVal = trampoline.write_call<5>(REL::ID(36345).address() + 0x11D, DamageActorValue);  //damage from actor?

	_DamageActorVal = trampoline.write_call<5>(REL::ID(37522).address() + 0x14, DamageActorValue);  //Generic call
	// address of stop casting 33630 SpellCast
	_StopSpellCast = REL::ID(33630).address();
	logger::info(SL::String("Function address" + SL::StringHex(_StopSpellCast.address())));
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

template<typename T>
void ReplaceActorValue(RE::TESGlobal* toAdd)
{
	TriStatTracker* track = TriStatTracker::GetSingleton();
	track->MVStorage[static_cast<int>(T::REPLACE)] = toAdd;
}
void ReplaceActorValuePap(RE::StaticFunctionTag*, int type, RE::TESGlobal* toAdd)
{
	
	switch (type) {
	case 0:
		ReplaceActorValue<TriStatTracker::health>(toAdd);
		break;
	case 1:
		ReplaceActorValue<TriStatTracker::magicka>(toAdd);
		break;
	case 2:
		ReplaceActorValue<TriStatTracker::stamina>(toAdd);
		break;
	}
}
void ReplaceActorValueInt(RE::StaticFunctionTag*, int type, int AV) 
{
	logger::info("Replacing AV");
	TriStatTracker* track = TriStatTracker::GetSingleton();
	switch (AV) {
	case 24:
		track->MVIntStorage[type] = RE::ActorValue::kHealth;
		break;
	case 25:
		track->MVIntStorage[type] = RE::ActorValue::kMagicka;
		break;
	case 26:
		track->MVIntStorage[type] = RE::ActorValue::kStamina;
		break;
	}
}
void RemoveReplaceActorValueInt(RE::StaticFunctionTag*, int type) 
{
	TriStatTracker* track = TriStatTracker::GetSingleton();
	track->MVIntStorage.erase(type);
}

void RemoveReplaceActorValuePap(RE::StaticFunctionTag*, int type, RE::TESGlobal* toAdd) {
	TriStatTracker* track = TriStatTracker::GetSingleton();
	switch (type) {
	case 0:
		if (track->MVStorage[static_cast<int>(TriStatTracker::health::REPLACE)] == toAdd)
			track->MVStorage.erase(static_cast<int>(TriStatTracker::health::REPLACE));
		break;
	case 1:
		if (track->MVStorage[static_cast<int>(TriStatTracker::magicka::REPLACE)] == toAdd)
			track->MVStorage.erase(static_cast<int>(TriStatTracker::magicka::REPLACE));
		break;
	case 2:
		if (track->MVStorage[static_cast<int>(TriStatTracker::stamina::REPLACE)] == toAdd)
			track->MVStorage.erase(static_cast<int>(TriStatTracker::stamina::REPLACE));
		break;
	}
}
template<typename T>
void RedirectActorValue(RE::TESGlobal* toAdd, bool reverse)
{
	TriStatTracker* track = TriStatTracker::GetSingleton();
	if (reverse) {
		track->MVStorage[static_cast<int>(T::REV_REDIRECT)] = toAdd;
		return;
	}
	track->MVStorage[static_cast<int>(T::REDIRECT)] = toAdd;
}
void RedirectActorValuePap(RE::StaticFunctionTag*, int type, RE::TESGlobal* toAdd, bool reverse)
{
	TriStatTracker* track = TriStatTracker::GetSingleton();
	switch (type) {
	case 0:
		if (!reverse)
			track->MVStorage[static_cast<int>(TriStatTracker::health::REDIRECT)] = toAdd;
		else
			track->MVStorage[static_cast<int>(TriStatTracker::health::REV_REDIRECT)] = toAdd;
		break;
	case 1:
		if (!reverse)
			track->MVStorage[static_cast<int>(TriStatTracker::magicka::REDIRECT)] = toAdd;
		else
			track->MVStorage[static_cast<int>(TriStatTracker::magicka::REV_REDIRECT)] = toAdd;
		break;
	case 2:
		if (!reverse)
			track->MVStorage[static_cast<int>(TriStatTracker::stamina::REDIRECT)] = toAdd;
		else
			track->MVStorage[static_cast<int>(TriStatTracker::stamina::REV_REDIRECT)] = toAdd;
		break;

	}
	

}
//void RedirectActorValueInt(RE::StaticFunctionTag*, int type, int AV, RE::TESGlobal*) 
//{
//
//}
//void RemoveRedirectActorValueInt(RE::StaticFunctionTag*, int type, int AV) 
//{

//}


void RemoveRedirectActorValue(RE::StaticFunctionTag*, int type, RE::TESGlobal* toAdd, bool reverse)
{
	TriStatTracker* track = TriStatTracker::GetSingleton();
	RE::TESGlobal* check{};
	switch (type) {
	case 0:

		if (!reverse) {
			if ((check = GetReplaceActorValue<TriStatTracker::health>(1)) != NULL && check == toAdd)
				track->MVStorage.erase(static_cast<int>(TriStatTracker::health::REDIRECT));
		}
		else {
			if ((check = GetReplaceActorValue<TriStatTracker::health>(2)) != NULL && check == toAdd)
				track->MVStorage.erase(static_cast<int>(TriStatTracker::health::REV_REDIRECT));
		}
		break;
	case 1:
		if (!reverse) {

			if ((check = GetReplaceActorValue<TriStatTracker::magicka>(1)) != NULL && check == toAdd)
				track->MVStorage.erase(static_cast<int>(TriStatTracker::magicka::REDIRECT));
		} else {
			if ((check = GetReplaceActorValue<TriStatTracker::magicka>(2)) != NULL && check == toAdd)
				track->MVStorage.erase(static_cast<int>(TriStatTracker::magicka::REV_REDIRECT));
		}
		break;
	case 2:
		if (!reverse) {
			if ((check = GetReplaceActorValue<TriStatTracker::stamina>(1)) != NULL && check == toAdd)
				track->MVStorage.erase(static_cast<int>(TriStatTracker::stamina::REDIRECT));
		} else {
			if ((check = GetReplaceActorValue<TriStatTracker::stamina>(2)) != NULL && check == toAdd)
				track->MVStorage.erase(static_cast<int>(TriStatTracker::stamina::REV_REDIRECT));
		}
		break;
	}
}
void AddUniqueItem(RE::StaticFunctionTag*, RE::TESForm* addForm, int actorVal) 
{
	logger::info("Adding unique shield");
	RE::ActorValue AV{};
	TriStatTracker* track = TriStatTracker::GetSingleton();
	switch (actorVal) {
	case 24:
		AV = RE::ActorValue::kHealth;
		break;
	case 25:
		AV = RE::ActorValue::kMagicka;
		break;
	case 26:
		AV = RE::ActorValue::kStamina;
		break;
	}
	track->UniqueItemStorageInt[addForm] = AV;
	
}
void AddUniqueItemGlobal(RE::StaticFunctionTag*, RE::TESForm* addForm, RE::TESGlobal* addGlobal)
{
	TriStatTracker* track = TriStatTracker::GetSingleton();
	track->UniqueItemStorageGlobal[addForm] = addGlobal;
}
void RemoveUniqueItem(RE::StaticFunctionTag*, RE::TESForm* removeForm) 
{
	TriStatTracker* track = TriStatTracker::GetSingleton();
	track->UniqueItemStorageInt.erase(removeForm);

}
void RemoveUniqueItemGlobal(RE::StaticFunctionTag*, RE::TESForm* removeForm)
{
	TriStatTracker* track = TriStatTracker::GetSingleton();
	track->UniqueItemStorageGlobal.erase(removeForm);
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
	a_vm->RegisterFunction("AddUniqueItem", "TST_SKSE", AddUniqueItem);
	a_vm->RegisterFunction("AddUniqueItemGlobal", "TST_SKSE", AddUniqueItemGlobal);

	a_vm->RegisterFunction("RemoveUniqueItem", "TST_SKSE", RemoveUniqueItem);
	a_vm->RegisterFunction("RemoveUniqueItemGlobal", "TST_SKSE", RemoveUniqueItemGlobal);

	//a_vm->RegisterFunction("RemoveRedirectActorValue", "TST_SKSE", RemoveRedirectActorValue);
	//a_vm->RegisterFunction("RedirectActorValue", "TST_SKSE", RedirectActorValuePap);
	
	a_vm->RegisterFunction("RemoveReplaceActorValue", "TST_SKSE", RemoveReplaceActorValuePap);
	a_vm->RegisterFunction("ReplaceActorValue", "TST_SKSE", ReplaceActorValuePap);
	a_vm->RegisterFunction("RemoveReplaceActorValueWithAV", "TST_SKSE", RemoveReplaceActorValueInt);
	a_vm->RegisterFunction("ReplaceActorValueWithAV", "TST_SKSE", ReplaceActorValueInt);


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




