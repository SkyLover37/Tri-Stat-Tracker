#include <PCH.h>
namespace SL
{
	template <class T>
	std::string String(T a)
	{
		std::stringstream t;
		t << a;
		return t.str();
	}
	template <class T>
	std::string StringHex(T a)
	{
		std::stringstream t;
		t << std::hex << a;
		return t.str();
	}
}
namespace SKSE
{
	void SetNthEffectMagnitude(RE::MagicItem* thisMagic, std::uint32_t index, float value)
	{
		if (thisMagic) {
			logger::info("Spell exists");
			RE::Effect* pEI = NULL;
			pEI = thisMagic->effects[index];
			if (pEI) {
				logger::info("Found ME");
				logger::info("Old ME: " + SL::String<float>(pEI->effectItem.magnitude));
				pEI->effectItem.magnitude = value;
				logger::info("New ME: " + SL::String<float>(pEI->effectItem.magnitude));
			}

			//RE::SpellItem* hol = skyrim_cast<RE::SpellItem*>(thisMagic);
		}
	}
	void sendNotification(std::string aMessage) {
		auto task = SKSE::GetTaskInterface();
		task->AddUITask([aMessage]() {
			enum
			{
				kMessageText
			};
			auto ui = RE::UI::GetSingleton();
			auto hud = ui->GetMenu<RE::HUDMenu>();
			auto view = hud ? hud->uiMovie : nullptr;
			auto delegate = hud ? hud->fxDelegate : nullptr;
			if (view && delegate) {
				RE::FxResponseArgsEx<1> sarg;
				sarg[kMessageText] = aMessage;

				delegate->Invoke(view.get(), "ShowMessage", sarg);
			}
		});
	}
}