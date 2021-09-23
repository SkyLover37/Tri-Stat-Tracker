#include "TriStat.h"

//TriStatTracker* track;
class VMClassRegistry;
const SKSE::PapyrusInterface* ppsInterface;
const SKSE::MessagingInterface* msginterface = nullptr;
	//

void MessageHandler(SKSE::MessagingInterface::Message* amsg)
{
	switch (amsg->type) {
	case SKSE::MessagingInterface::kNewGame:

		logger::info("NewGame");
		//TriStatTracker::GetSingleton();
		break;
	case SKSE::MessagingInterface::kPostLoad:
		logger::info("PostLoad");
		//TriStatTracker::GetSingleton();
		break;
	}
}
bool registerAllFunctions(VMClassRegistry*)
{
	//auto funcName = "sortByName";
	//auto className = PLUGIN_NAME;

	//registry->RegisterFunction(
		//new NativeFunction1<StaticFunctionTag, void, SInt32>(funcName, className, sortByName, registry));

	//registry->SetFunctionFlags(className, funcName, VMClassRegistry::kFunctionFlag_NoWait);

	//_MESSAGE("registering functions");

	return true;
}
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= "TriStatTracker.log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::warn);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::critical("TriStatTracker v0.1.0");

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "TriStatTracker";
	a_info->version = 1;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}
	
	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}
	
	return true;
}





REL::Relocation<RE::PlayerCharacter**> g_thePlayer(REL::ID(517014));

	
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("TriStatTracker Loaded");
	
	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(400);
	
	ppsInterface = SKSE::GetPapyrusInterface();
	//RE::BSScript::IVirtualMachine* a_vm
	
	if (!ppsInterface->Register(RegisterFuncs)) {
		logger::critical("failed to retrieve papyrus interface");
		return true;
	}
	
	msginterface = SKSE::GetMessagingInterface();
	
	
	if (!msginterface->RegisterListener(MessageHandler)) {
		logger::critical("Failed to retrieve messaging interface");
		return false;
	}
	//track = TriStatTracker::GetSingleton();
	//auto serialization = SKSE::GetSerializationInterface();
	//serialization->SetUniqueID('SLBM');
	//serialization->SetSaveCallback(save );
	//serialization->SetLoadCallback(load);

	return true;
}

