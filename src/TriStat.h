#pragma once
#include <vector>
#include <map>
#include "xbyak.h"



//void loadConsumables();

//The Player
extern REL::Relocation<RE::PlayerCharacter**> g_thePlayer;

class TriStatTracker : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>, public RE::BSTEventSink<RE::BSAnimationGraphEvent>, public RE::BSTEventSink<RE::TESHitEvent>, public RE::BSTEventSink<RE::TESEquipEvent>
{
public:
	
	
	static TriStatTracker* GetSingleton()
	{
		static TriStatTracker* instance;
		if (!instance) {
			instance = new TriStatTracker;
			
	
		}
		return instance;
	}
	

	enum struct health
	{
		//EFFICIENCY = 3,				
		BUFFER = 0,	
		NEGATIVE_BUFFER,
		HIGHEST_DAMAGE,		
		RECENT_HIGHEST_DAMAGE,	
		TOTAL_DAMAGED,			
		TOTAL_RESTORED,			
	};
	enum struct magicka
	{
		//EFFICIENCY = 9,		
		BUFFER = 6,	
		NEGATIVE_BUFFER,
		HIGHEST_DAMAGE,			
		RECENT_HIGHEST_DAMAGE,	
		TOTAL_DAMAGED,			
		TOTAL_RESTORED			
	};
	enum struct stamina
	{
		//EFFICIENCY = 15,				
		BUFFER = 12,
		NEGATIVE_BUFFER,
		HIGHEST_DAMAGE,			
		RECENT_HIGHEST_DAMAGE,	
		TOTAL_DAMAGED,			
		TOTAL_RESTORED			
	};
	RE::TESDataHandler* hdle{};
	
	std::map<std::string, std::map<std::string, RE::TESGlobal*>> slccStorage;
	
	//RE::TESGlobal* Energy;
	//std::vector<RE::TESGlobal*> Efficiency;
	//std::vector<RE::TESGlobal*> efficiencyMod;
	
	std::vector<RE::SpellItem*> spl;
	
	bool varsLoaded = false;
	bool state = false;
	


	//void load(SKSE::SerializationInterface* a_intfc);
	
	//void save(SKSE::SerializationInterface* a_intfc);
	
	template <typename T>
	void bufferUpdate(float mod);

	template <typename T>
	void restored(float mod);

	template <typename T>
	void damaged(float mod);

	static void DamageActorValue(RE::PlayerCharacter* pCharacter, int AVModifiers, int actorVal, float mod, RE::TESObjectREFR* a5);
	static void SpellCast(RE::MagicCaster* unk1, float unk2);
	static float PreCastGetAVO(__int64*, int);  //__int64* avOwner, int av);
	static void SpellDec(RE::TESForm* unk1, int unk2, int unk3, float unk4, __int64* unk5);
	static char PreCast(__int64* magicCaster);
	inline static REL::Relocation<decltype(DamageActorValue)> _DamageActorVal;
	inline static REL::Relocation<decltype(SpellCast)> _SpellCast;
	inline static REL::Relocation<decltype(SpellDec)> _SpellDec;
	inline static REL::Relocation<decltype(PreCast)> _PreCast;
	inline static REL::Relocation<decltype(PreCastGetAVO)> _PreCastGetAVO;

protected:
	RE::BSEventNotifyControl ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) override;
	//RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) override;
	//RE::BSEventNotifyControl ProcessEvent(const RE::TESActiveEffectApplyRemoveEvent* a_event, RE::BSTEventSource<RE::TESActiveEffectApplyRemoveEvent>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;

private :
	TriStatTracker();
	TriStatTracker(TriStatTracker&);
	~TriStatTracker() {}
};
	void load(SKSE::SerializationInterface* a_intfc);
	void save(SKSE::SerializationInterface* a_intfc);
bool RegisterFuncs(RE::BSScript::IVirtualMachine* a_vm);

