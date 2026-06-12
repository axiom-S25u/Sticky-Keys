#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class $modify(StickyGameLayer, GJBaseGameLayer) {
	struct Fields {
		bool m_p1Sticky = false;
		bool m_p2Sticky = false;
	};

	void handleButton(bool down, int button, bool isPlayer1) {
		if (button != 1 || m_level->isPlatformer() || !Mod::get()->getSettingValue<bool>("enabled")) { // as i said in autoragequit, im never touching platformer again
			GJBaseGameLayer::handleButton(down, button, isPlayer1);
			return;
		}

		bool& sticky = isPlayer1 ? m_fields->m_p1Sticky : m_fields->m_p2Sticky;

		if (down) {
			if (!sticky) {
				sticky = true;
				GJBaseGameLayer::handleButton(true, button, isPlayer1);
			} else {
				sticky = false;
				GJBaseGameLayer::handleButton(false, button, isPlayer1); // simple and it works, idk what u can complain about now
			}
		}
	}
};