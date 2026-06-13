#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
	struct StickyState {
		bool p1[4]{};
		bool p2[4]{};
	};

	StickyState g_sticky;

	void resetSticky(bool* sticky) {
		for (int i = 0; i < 4; i++) sticky[i] = false;
	}

	bool isStickyButton(GJBaseGameLayer* layer, int button) {
		if (button == (int)PlayerButton::Jump) return true;
		return layer->m_level->isPlatformer()
			&& (button == (int)PlayerButton::Left || button == (int)PlayerButton::Right);
	}
} // stickyyyy
// how many times did anyone get the popup of sticky keys when playing minecraft

class $modify(StickyGameLayer, GJBaseGameLayer) {
	void handleButton(bool down, int button, bool isPlayer1) {
		if (!Mod::get()->getSettingValue<bool>("enabled") || !isStickyButton(this, button)) {
			GJBaseGameLayer::handleButton(down, button, isPlayer1);
			return;
		}

		bool player1 = isPlayer1;
		if (!m_gameState.m_isDualMode) {
			player1 = true;
		}

		bool* sticky = player1 ? g_sticky.p1 : g_sticky.p2; // this was weird, during testing i pressed D then right arrow, this fixes it i guess

		if (down) {
			if (button == (int)PlayerButton::Left) {
				if (sticky[button]) {
					sticky[button] = false;
					GJBaseGameLayer::handleButton(false, button, isPlayer1);
				} else {
					if (sticky[(int)PlayerButton::Right]) {
						sticky[(int)PlayerButton::Right] = false;
						GJBaseGameLayer::handleButton(false, (int)PlayerButton::Right, isPlayer1);
					}
					sticky[button] = true;
					GJBaseGameLayer::handleButton(true, button, isPlayer1);
				}
			} else if (button == (int)PlayerButton::Right) {
				if (sticky[button]) {
					sticky[button] = false;
					GJBaseGameLayer::handleButton(false, button, isPlayer1);
				} else {
					if (sticky[(int)PlayerButton::Left]) {
						sticky[(int)PlayerButton::Left] = false;
						GJBaseGameLayer::handleButton(false, (int)PlayerButton::Left, isPlayer1);
					}
					sticky[button] = true;
					GJBaseGameLayer::handleButton(true, button, isPlayer1);
				}
			} else {
				if (!sticky[button]) {
					sticky[button] = true;
					GJBaseGameLayer::handleButton(true, button, isPlayer1);
				} else {
					sticky[button] = false;
					GJBaseGameLayer::handleButton(false, button, isPlayer1);
				}
			}
		}
	}
};

class $modify(StickyPlayLayer, PlayLayer) {
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

		resetSticky(g_sticky.p1);
		resetSticky(g_sticky.p2);
		return true;
	}

	void destroyPlayer(PlayerObject* player, GameObject* object) {
		PlayLayer::destroyPlayer(player, object);

		if (!Mod::get()->getSettingValue<bool>("enabled")) return;

		resetSticky(player == m_player1 ? g_sticky.p1 : g_sticky.p2);
	}
};
