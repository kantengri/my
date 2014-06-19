
#include "widgets.h"
#include "assert_impl.h"
#include "math.h"

using namespace myvi;

bool globals::gui_debug = false;
modal_overlay_t globals::modal_overlay;

u32 rasterizer_t::colors[4] = {0x00ff00, 0x0000ff, 0xff0000, 0xff00ff};
u32 rasterizer_t::deepLevel = 0;
bool rasterizer_t::debug = false;

focus_manager_t focus_manager_t::instance;


void focus_manager_t::key_event(key_t::key_t key, gobject_t *root) {

	_MY_ASSERT(root, return);
	

	if (captured.length()) {
		focus_aware_t *captured_child = captured.last();

		gobject_t *captured_child_g = dynamic_cast<gobject_t*>(captured_child);
		_MY_ASSERT(captured_child_g && captured_child_g->visible && captured_child_g->enabled, return);

		if (captured_child_g != root) {
			captured_child->key_event(key);
			return;
		}
	}


	if (key != key_t::K_LEFT && key != key_t::K_RIGHT && key != key_t::K_UP && key != key_t::K_DOWN) {

		// �������� ��� selected �� �������� ������ ������ ��� (combobox_t)
		if (selected) {
			gobject_t *selected_g = dynamic_cast<gobject_t*>(selected);
			_MY_ASSERT(selected_g && selected_g->visible && selected_g->enabled, return);
			focus_aware_t * focus_aware = dynamic_cast<focus_aware_t*>(selected);
			if (focus_aware) {
				focus_aware->key_event(key);
			}
		}
		return ;
	}


	if (!selected) {
		gobject_t::iterator_selectable_deep_t iter = root->iterator_selectable_deep();
		focus_client_t *p = dynamic_cast<focus_client_t*>(iter.next());
		select(p);
		return;
	}

	focus_intention_t intention;

	if (key == key_t::K_UP) {
		intention.direction = direction_t::UP;
	}
	if (key == key_t::K_DOWN) {
		intention.direction = direction_t::DOWN;
	}
	if (key == key_t::K_LEFT) {
		intention.direction = direction_t::LEFT;
	}
	if (key == key_t::K_RIGHT) {
		intention.direction = direction_t::RIGHT;
	}

	intention.current = selected;
	intention.next = locate_next(intention.direction, root);

	if (intention.next) {
		if (intention.current) {
			gobject_t *current_g = dynamic_cast<gobject_t*>(intention.current);
			_MY_ASSERT(current_g, return);

			focus_master_t *focus_master = current_g->get_focus_master();

			if (focus_master) {
				focus_master->alter_focus_intention(intention);
			}
		}
		select(intention.next);
	}
}

focus_client_t * focus_manager_t::locate_next(direction_t::direction_t direction, gobject_t *root) {

	// ���������� ��������� ������� ����� � selected
	gobject_t *next = 0;
	gobject_t *next_up = 0;
	gobject_t *next_down = 0;
	gobject_t *next_left = 0;
	gobject_t *next_right = 0;

	s32 sx,sy;
	gobject_t *selected_g = dynamic_cast<gobject_t*>(selected);
	selected_g->translate(sx,sy);

	float sxf = (float)sx, syf = (float)sy;
	float ud,dd,ld,rd;

	gobject_t::iterator_selectable_deep_t iter = root->iterator_selectable_deep();
	gobject_t *p =  iter.next();
	while (p) {
		if (p == selected_g) {
			p = iter.next();
			continue;
		}

		s32 px,py;
		p->translate(px,py);
		float pxf = (float)px, pyf = (float)py;
		float pxd = pxf-sxf, pyd = pyf-syf;
		float pd = sqrtf(pxd*pxd + pyd*pyd);

		if (py < sy) {
			if (!next_up || (next_up && (pd < ud))) {
				next_up = p;
				ud = pd;
			}
		}
		if (py > sy) {
			if (!next_down || (next_down && (pd < dd))) {
				next_down = p;
				dd = pd;
			}
		}
		if (px < sx) {
			if (!next_left || (next_left && (pd < ld))) {
				next_left = p;
				ld = pd;
			}
		}
		if (px > sx) {
			if (!next_right || (next_right && (pd < rd))) {
				next_right = p;
				rd = pd;
			}
		}
		p = iter.next();
	}

	if (direction == direction_t::UP) {
		next = next_up;
	}
	if (direction == direction_t::DOWN) {
		next = next_down;
	}
	if (direction == direction_t::LEFT) {
		next = next_left;
	}
	if (direction == direction_t::RIGHT) {
		next = next_right;
	}
	return dynamic_cast<focus_client_t *>(next);

}

void focus_manager_t::select(focus_client_t *p) {


	if (selected) {
		_MY_ASSERT(dynamic_cast<focus_client_t*>(selected), return);
		_MY_ASSERT(dynamic_cast<gobject_t*>(selected), return);

		dynamic_cast<focus_client_t*>(selected)->selected = false;
		dynamic_cast<gobject_t*>(selected)->dirty = true;
	}

	selected = p;

	if (selected) {
		_MY_ASSERT(dynamic_cast<focus_client_t*>(selected), return);
		_MY_ASSERT(dynamic_cast<gobject_t*>(selected), return);

		dynamic_cast<focus_client_t*>(selected)->selected = true;
		dynamic_cast<gobject_t*>(selected)->dirty = true;
		notify(selected);
	}
}


void focus_aware_t::key_event(key_t::key_t key) {
	gobject_t * _this = dynamic_cast<gobject_t*>(this);
	_MY_ASSERT(_this && _this->visible && _this->enabled, return);
	// ������ ������� �����
	focus_manager_t::instance.key_event(key, _this);
}
