#ifndef _WIDGETS_H
#define _WIDGETS_H

#include "surface.h"
#include "truetype.h"
#include "app_events.h"
#include "assert_impl.h"
#include "basic.h"


namespace myvi {


class gobject_t;

class layout_t {
public:
public:
	virtual void get_preferred_size(gobject_t *parent, s32 &pw, s32 &ph) = 0;
	virtual void layout(gobject_t *parent) = 0;
};


namespace globals {
	extern bool gui_debug;
}

#define _MAX_GOBJECT_TREE_DEPTH 32
#define _MAX_FOCUS_MANAGER_SUBSCRIBERS 32

namespace direction_t {
	typedef enum {
		UP,
		DOWN,
		LEFT,
		RIGHT
	} direction_t;
}

class focus_client_t;
class focus_aware_t;

// ���������� �������� ������
class focus_manager_t : public publisher_t<focus_client_t *,_MAX_FOCUS_MANAGER_SUBSCRIBERS> {
public:
	focus_client_t *selected;
	stack_t<focus_aware_t *, _MAX_GOBJECT_TREE_DEPTH> captured;

	static focus_manager_t instance;

private:

	focus_manager_t() {
		selected = 0;
	}

public:
	void key_event(key_t::key_t key, gobject_t *root);
	void select(focus_client_t *p);

	void capture_child(focus_aware_t *child) {
		_MY_ASSERT(child,return);
		if (captured.length()) {
			_MY_ASSERT(captured.last() != child, return);
		}
		captured.push(child);
	}

	void release_child(focus_aware_t *child) {
		_MY_ASSERT(child,return);
		_MY_ASSERT(captured.last()  == child,return);
		captured.pop();
	}

	focus_client_t * locate_next(direction_t::direction_t direction, gobject_t *root);

};

class focus_intention_t {
public:
	focus_client_t *current;
	focus_client_t *next;
	direction_t::direction_t direction;
public:
	focus_intention_t() {
		current = 0;
		next = 0;
		direction = direction_t::UP;
	}
};


// ����� �������� ���������� ������
class focus_master_t {
public:
public:
	virtual void alter_focus_intention(focus_intention_t &intention) = 0;
};


// ����� ����������� �����, ������������ ��������� ����������
class focus_aware_t {
public:
	virtual void key_event(key_t::key_t key);

	void capture_focus() {
		focus_manager_t::instance.capture_child(this);
	}

	void release_focus() {
		focus_manager_t::instance.release_child(this);
	}

};



// ����� ���� � ������, "�������"
class focus_client_t {
public:
	property_t<bool, focus_client_t> selected;
private:
	bool _selected;

private:
	bool get_selected() {
		return _selected;
	}
public:
	focus_client_t() {
		_selected = false;
		selected.init(this,&focus_client_t::get_selected, &focus_client_t::set_selected);
	}


	virtual void set_selected(bool selected) {
		_selected = selected;
	}

};



// ������� ����� ���� �������� ������
class gobject_t { 
public:
	s32 x; // ���������� ������������ ������������� ����������
	s32 y;
	s32 w; // ������� �������
	s32 h;

	bool visible;

	gobject_t *parent;

	layout_t *layout;

	property_t<bool, gobject_t> enabled;
	property_t<bool, gobject_t> dirty;

private:
	bool _enabled;
	bool _dirty; // if true - shall render
	gobject_t *last_selectable;


public:
	gobject_t() {
		parent = 0;
		x=(0),y=(0),w=(0),h=(0),_dirty=(false), 
		visible = true, layout = 0,  _enabled = true;
		enabled.init(this,&gobject_t::get_enabled, &gobject_t::set_enabled);
		dirty.init(this,&gobject_t::get_dirty, &gobject_t::set_dirty);
		last_selectable = 0;
	}

	virtual void init() {
		init_children();
	}
private:	
	bool get_dirty() {
		return _dirty;
	}
	bool get_enabled() {
		return _enabled;
	}
public:


	virtual void set_dirty(bool dirty) {
		_dirty = dirty;
	}
	virtual void set_enabled(bool enabled) {
		_enabled = enabled;
	}
	
	void init_children() {		// must be called from derived class ctor
		gobject_t *p = next_all(0);
		while(p) {
			p->parent = this;
			p->init();
			p = next_all(p);
		}
	}

	void translate(s32 &ax, s32 &ay) {
		gobject_t *p = parent;
		ax = x;
		ay = y;
		while (p) {
			ax += p->x;
			ay += p->y;
			p = p->parent;
		}
	}


	virtual void render(surface_t &dst) {
	}
	
	// ������������� ���� �������� ��������
	virtual gobject_t* next_all(void* prev) {
		return 0;
	}

	focus_master_t * get_focus_master() {

		gobject_t *p = parent;
		while (p) {
			focus_master_t *ret = dynamic_cast<focus_master_t*>(p);
			if (ret) {
				return ret;
			}
			p = p->parent;
		}
		return 0;
	}

	// ������������� ����  ������� �������� ��������
	gobject_t* next_visible(void* prev) {

		gobject_t* p = next_all(prev);

		while (p && !p->visible) {
			p = next_all(p);
		}
		return p;
	}

	// ������������� ���� �������� ��������, ��������� � ������ � ������� � �������
	gobject_t * next_selectable_deep(void* prev) {

		gobject_t* p = next_visible(prev);
		while (p) {
			focus_client_t *ret = dynamic_cast<focus_client_t*>(p);
			if (p->enabled && ret) {
				return p;
			}
			p = next_visible(p);
		}

		// � ����� ����� �� �����, ������ � ����� �����
		p = next_visible(0);

		while (p) {
			if (p->enabled) {
				gobject_t* pp = p->next_selectable_deep(prev);
				if (pp) {
					last_selectable = p;
					return pp;

				} else if (last_selectable == p) {
					// �� ����� � ����, � ������� ������ �������� - ��������� � �������� ����. ����
					last_selectable = 0;
					prev = 0;
				}
			}
			p = next_visible(p);
		}

		return 0;
	}


	// �������� do_layout ��� ������� �� �����
	void layout_children() {
		_MY_ASSERT(w && h, return);
		gobject_t *bt = next_visible(0);
		while (bt) {
			bt->do_layout();
			bt = next_visible(bt);
		}
	}
	// ��� ����������� ��������� ������ �������
	virtual void child_request_size_change(gobject_t *child, s32 new_w, s32 new_h) {
	}


	virtual void get_preferred_size(s32 &pw, s32 &ph) {
		_MY_ASSERT(layout,return);
		layout->get_preferred_size(this, pw,ph);
	}

	// ��������� ����� � ������������ ��������
	virtual void do_layout() {
		_MY_ASSERT(w && h,return);
		// ���� layout �� ������, �� �������, ��� ��� ���� ���� ����������� � init
		if (layout) {
			layout->layout(this);
		}
		layout_children();	
	}

};



class rasterizer_t {
public:
	static u32 colors[4];
	static u32 deepLevel;
public:

	// ������� dirty ��� ������� � ���� ��������, �.�. �� ��� �������������
	static bool render(gobject_t *p, surface_t &dst, bool force_redreaw = false) {
		return render(p,dst,force_redreaw,-1,-1,-1,-1);
	}

	static bool render(gobject_t *p, surface_t &dst, bool force_redreaw, s32 pax, s32 pay, s32 paw, s32 pah) {
		deepLevel++;
		_MY_ASSERT(p->visible,return false);
		bool ret = false;

		if (p->dirty || force_redreaw) {
			s32 x1,y1, w1,h1;
			p->translate(x1,y1);
			w1 = p->w;
			h1 = p->h;
			if (pah >= 0) {
				if (!surface_t::trim_to(x1,y1,w1,h1, pax,pay,paw,pah)) {
					return false;
				};
			}
			pax = x1;
			pay = y1;
			paw = w1;
			pah = h1;
			dst.set_allowed_area(pax,pay,paw,pah);
// DEBUG DRAW:
			dst.ctx.pen_color = colors[deepLevel & 0x03];
//			dst.rect(x1,y1,p->w,p->h);

			p->render(dst);
			clear_dirty(p);
			force_redreaw = true;
			ret = true;
//			return true;
		} 
		// ������ ��������
		gobject_t *pp = p->next_visible(0);
		while (pp) {
			bool ret1 = render(pp, dst, force_redreaw, pax, pay, paw, pah);
			if (!ret && ret1)
				ret = true;
			pp = p->next_visible(pp);
		}
		deepLevel--;
		return ret;
	}
private:
	static void clear_dirty(gobject_t *p) {
		p->dirty = (false);
		gobject_t *pp = p->next_visible(0);
		while (pp) {
			clear_dirty(pp);
			pp = p->next_visible(pp);
		}
	}

};
namespace font_size_t {
	enum font_size_t {
		FS_8 = 12,
		FS_15 = 16,
		FS_20 = 21,
		FS_30 = 30
	};
}

class label_context_t {
public:
//	u32 text_color; // ���� ������ � �����
	font_size_t::font_size_t font_size;    // ������ ������ \ ����� � ��������
	ttype_font_t *font;	// ����� ������ \ �����
	surface_context_t sctx;
public:
	label_context_t() {
		font_size=font_size_t::FS_8;
		font=(0);
		sctx.pen_color = 0xffffff;
	}
};

class label_t  : public gobject_t {
public:
	u32 glyph_code;	// ����
	string32_t text32;  // �����
	string_t text;  // �����
	label_context_t ctx;
public:
	label_t() {
		glyph_code=(0x0000);
		visible = false;
	}

	virtual void render(surface_t &dst) OVERRIDE {
		s32 ax = 0, ay = 0;
		_MY_ASSERT(visible && ctx.font,return);
		ctx.font->char_surface.ctx = ctx.sctx;
		ctx.font->set_char_size_px(0,ctx.font_size);

		translate(ax,ay);

		if (!text32.is_empty()) {
			// glyph string
			ctx.font->print_to(ax,ay,dst,text32);	
		} else if (!text.is_empty()) {
			// text
			ctx.font->print_to(ax,ay,dst,text);	
		} else if (this->glyph_code) {
			// glyph
			ctx.font->gly_to(ax,ay,dst,glyph_code);
		} else {
			// do nothing
		}
//		render_children(dst);
	}

	virtual void get_preferred_size(s32 &aw, s32 &ah) OVERRIDE {
		_MY_ASSERT(visible,return);
		_MY_ASSERT(ctx.font,return);
		ctx.font->set_char_size_px(0,ctx.font_size);
		if (!text32.is_empty()) {
			// glyph string
			ctx.font->get_string_size(text32, aw, ah);
		} else if (!text.is_empty()) {
			// text
			ctx.font->get_string_size(this->text, aw, ah);
		} else if (this->glyph_code) {
			// glyph
			ctx.font->get_gly_size(this->glyph_code, aw, ah);
		} else {
			// no content - try measure something
			string_t cs1 = string_t("1");
			ctx.font->get_string_size(cs1, aw, ah);
		}

	}
};

class button_context_t {
public:
	u32 bk_color;	// ���� �� ��������� ������
	u32 bk_sel_color;	// ���� ��������� ������
	u32 bk_pressed_color;	// ���� ������� ������
	u32 bk_disabled_color; // ���� ���������� ������
	surface_context_t sctx;
public:
	button_context_t() {
		bk_color=(0xB8BDCF),bk_sel_color=(0x848794),bk_pressed_color=(0x6B8896);
		bk_disabled_color = 0xA0A0A0;
	}
};


// ����������� �������� ������� �� ������
class center_layout_t : public layout_t {
public:
	s32 spx;
	s32 spy;
	bool vertical;
public:
	center_layout_t() {
		spx = 5, spy = 5;
		vertical = true;
	}


	virtual void get_preferred_size(gobject_t *parent, s32 &aw, s32 &ah) OVERRIDE {
		_MY_ASSERT(0,return);
	}

	virtual void layout(gobject_t *parent) OVERRIDE {
		gobject_t *p = parent->next_visible(0);
		s32 py = 0, px = 0;
		while (p) {
			_WEAK_ASSERT(p->visible,return);
			if (vertical) {
				if (p->w > parent->w) 
					p->w = parent->w;
				p->x = (parent->w - p->w)/2;
				_WEAK_ASSERT(p->x >= 0,return);
				p->y = py;
				py += p->h + spy;
			} else {
				if (p->h > parent->h) 
					p->h = parent->h;
				p->y = (parent->h - p->h)/2;
				_WEAK_ASSERT(p->y >= 0,return);
				p->x = px;
				px += p->w + spx;
			}
			_WEAK_ASSERT(p->y + p->h <= parent->h && p->x + p->w <= parent->w,return);
			p = parent->next_visible(p);
		}
		if (!vertical && px) px -= spx;
		if (vertical && py) py -= spy;
		_WEAK_ASSERT((vertical && py <= parent->h) || (!vertical && px <= parent->w),return);

		s32 dy = (parent->h - py)/2;
		s32 dx = (parent->w - px)/2;
		p = parent->next_visible(0);
		while (p) {
			if (vertical) {
				p->y += dy;
			} else {
				p->x += dx;
			}
			_WEAK_ASSERT(p->y + p->h <= parent->h && p->x + p->w <= parent->w,return);
			p = parent->next_visible(p);
		}
	}
};


// ����������� �������� ���������� ������

class stack_layout_t : public layout_t {
public:
	s32 spx;
	s32 spy;  // ������ �������
	s32 bw;   // button width
	s32 bh;
	bool vertical;
	bool preferred_item_size;  // ������������ �������������� ������ ���������� ������ ������� ������
public:
	stack_layout_t() {
		spx=5,spy=5,vertical=(true);
		bw=20,bh=20;
		preferred_item_size = false;
	}

	virtual void get_preferred_size(gobject_t *parent, s32 &aw, s32 &ah) OVERRIDE {
		gobject_t *bt = parent->next_visible(0);
		aw = 0;
		ah = 0;
		while (bt) {
			s32 btw, bth;
			bt->get_preferred_size(btw, bth);

			if (vertical) {
				if (btw > aw) {
					aw = btw;
				}
				ah += bth + spy;
			} else {
				if (bth > ah) {
					ah = bth;
				}
				aw += btw + spx;
			}
			bt = parent->next_visible(bt);
		}
		if (vertical && ah) ah -= spy;
		if (!vertical && aw) aw -= spx;
	}



	virtual void layout(gobject_t *parent) OVERRIDE {

		_WEAK_ASSERT((vertical && bh) || (!vertical && bw),return);

		gobject_t *child = parent->next_visible(0);
		s32 px = 0;
		s32 py = 0;
		while (child) {
			_WEAK_ASSERT(child->visible,return);
			if (vertical) {
				child->x = 0;
				child->y = py;
				if (preferred_item_size) {
					s32 cw, ch;
					child->get_preferred_size(cw,ch);
					child->h = ch;
				} else child->h = bh;
				child->w = parent->w;
				py += child->h + spy;
			} else {
				child->x = px;
				child->y = 0;
				if (preferred_item_size) {
					s32 cw, ch;
					child->get_preferred_size(cw,ch);
					child->w = cw;
				} else child->w = bw;
				child->h = parent->h;
				px += child->w + spx;
			}
			child = parent->next_visible(child);
		}
		if (!vertical && px) px -= spx;
		if (vertical && py) py -= spy;
		_WEAK_ASSERT((py <= parent->h) && (px <= parent->w),return);
	}
};


// ����������� �������� ������� �� ���� ������ ��������
class stretch_layout_t : public layout_t {
public:
public:
	virtual void get_preferred_size(gobject_t *parent, s32 &aw, s32 &ah) OVERRIDE {
		_MY_ASSERT(0, return);
	}

	virtual void layout(gobject_t *parent) OVERRIDE {
		gobject_t *child = parent->next_visible(0);
		while (child) {
			child->x = 0;
			child->y = 0;
			child->h = parent->h;
			child->w = parent->w;
			child = parent->next_visible(child);
			_WEAK_ASSERT(!child,return); // ������ ��������� ��� �������
		}
	}
};

// ����������� ������� �� �������� ��������
#define LL_LEV_SIZE 4
class levels_layout_t : public layout_t {
public:
	s32 levels[LL_LEV_SIZE];
public:
	levels_layout_t() {
		levels[0] = 5;
		levels[1] = 30;
		levels[2] = 75;
		levels[3] = -1;
	}

	virtual void get_preferred_size(gobject_t *parent, s32 &aw, s32 &ah) OVERRIDE {
		_MY_ASSERT(0, return);
	}

	virtual void layout(gobject_t *parent) OVERRIDE {
		gobject_t *child = parent->next_visible(0);
		gobject_t *pchild = 0;
		s32 i = 0;
		while (child) {
//			child->set_preferred_size();
			if (child->w > parent->w)
				child->w = parent->w;
			child->x = (parent->w - child->w)/2;

			_WEAK_ASSERT(i < LL_LEV_SIZE && levels[i] >= 0 && levels[i] < 100,return);
			child->y = (parent->h * levels[i])/100;
			if (pchild && pchild->y + pchild->h > child->y) {
				pchild->h = child->y - pchild->y;
				_MY_ASSERT(pchild->h >= 0,return);
			}
			i++;
			pchild = child;
			child = parent->next_visible(child);
		}
		if (pchild && pchild->y + pchild->h > parent->h) {
			pchild->h = parent->h - pchild->y;
			_WEAK_ASSERT(pchild->h >= 0,return);
		}
	}
};



class button_t : public gobject_t, public focus_client_t {
public:
	label_t l_top;
	label_t l_mid;
	label_t l_bot;
	button_context_t ctx;
	levels_layout_t levels_layout;
	property_t<bool, button_t> pressed;
private:
	bool _pressed;	// ������� ���������
private:
	bool get_pressed() {
		return _pressed;
	}
	void set_pressed(bool apressed) {
		_pressed = apressed;
		dirty = true;
	}

public:
	button_t() {
		_pressed = (false);
		layout = &levels_layout;
		pressed.init(this,&button_t::get_pressed, &button_t::set_pressed);
	}

	virtual void render(surface_t &dst) OVERRIDE {
		dst.ctx = ctx.sctx;
		if (!enabled) 
			dst.ctx.pen_color = ctx.bk_disabled_color;
		else if (pressed) 
			dst.ctx.pen_color = ctx.bk_pressed_color;
		else if (selected) 
			dst.ctx.pen_color = ctx.bk_sel_color;
		else 
			dst.ctx.pen_color = ctx.bk_color;

		s32 ax = 0, ay = 0;

		translate(ax,ay);

		dst.ctx.alfa = 0xff;
		dst.fill(ax,ay,w,h);

		dst.ctx.pen_color = 0xffffff;
		dst.ctx.alfa = 128;
		dst.rect(ax,ay,w,h);

	}


	virtual gobject_t* next_all(void* prev) OVERRIDE {
		if (!prev) return &l_top;
		else if (prev == &l_top) return &l_mid;
		else if (prev == &l_mid) return &l_bot;
		else return 0;
	}

};


#define INPUT_MAX_LEN 30
#define COLOR_SELECTED 0xAFBFCF
#define COLOR_CAPTURED 0x267F00

class text_box_t : public gobject_t, public focus_client_t, public focus_aware_t {
	typedef gobject_t super;
public:
	property_t<string_t , text_box_t> value;
	label_t lab;
	bool cursor_visible;
	u32 cursor_color;
private:
	string_impl_t<INPUT_MAX_LEN> _value;
	s32 cursor_pos[INPUT_MAX_LEN];
	s32 caret_pos;
private:
	void set_value(string_t cvalue) {
		_value=(cvalue);
		lab.text = _value;
		caret_pos = _value.length();
	}

	string_t get_value() {
		return _value;
	}
public:
	text_box_t() {
		value.init(this,&text_box_t::get_value, &text_box_t::set_value);
		lab.ctx.sctx.pen_color = 0x00;
		lab.visible = true;
		caret_pos = 0;
		cursor_visible = false;
		cursor_color = 0x000000;
	}

	void measure_cursor_pos() {
		for (s32 i=1; i <= _value.length(); i++) {
			lab.text = _value.sub(0,i);
			s32 lw, lh;
			lab.get_preferred_size(lw,lh);
			cursor_pos[i-1] = lw;
		}
		lab.text = _value;
	}

	virtual gobject_t* next_all(void* prev) OVERRIDE {
		if (!prev) return &lab;
		return 0;
	}

	virtual void get_preferred_size(s32 &aw, s32 &ah) OVERRIDE {
		measure_cursor_pos();
		lab.get_preferred_size(aw, ah);
		//if (h < lab.h) h = lab.h;
		//if (w < lab.w + 5) w = lab.w + 5;
	}

	virtual void do_layout() OVERRIDE {
		lab.x = 0;
		lab.y = 0;
		lab.w = w; // ����� ��� �������
		lab.h = h;
	}

	virtual void key_event(key_t::key_t key) OVERRIDE {

		s32 cur_len = _value.length();

		if (key == key_t::K_ENTER) {
			_MY_ASSERT(parent,return);
			if (!cursor_visible) {
				this->capture_focus();
				cursor_visible = true;
			} else {
				this->release_focus();
				cursor_visible = false;
			}
			goto lab_update_input;
		}

		if (key == key_t::K_LEFT) {
			if (caret_pos) caret_pos--;
			goto lab_update_input;
		}
		if (key == key_t::K_RIGHT) {
			if (caret_pos < cur_len) caret_pos++;
			goto lab_update_input;
		}

		if ((key == key_t::K_BKSP && caret_pos) || (key == key_t::K_DELETE && caret_pos < cur_len)) {
			if (key == key_t::K_BKSP) caret_pos--;
			_value.delete_at(caret_pos);
			goto lab_update_input;
		}

		if (caret_pos < INPUT_MAX_LEN-1) {
			u8 ch = 0;
			if (key == key_t::K_0) ch = '0';
			if (key == key_t::K_1) ch = '1';
			if (key == key_t::K_2) ch = '2';
			if (key == key_t::K_3) ch = '3';
			if (key == key_t::K_4) ch = '4';
			if (key == key_t::K_5) ch = '5';
			if (key == key_t::K_6) ch = '6';
			if (key == key_t::K_7) ch = '7';
			if (key == key_t::K_8) ch = '8';
			if (key == key_t::K_9) ch = '9';
			if (key == key_t::K_DOT) ch = '.';
			if (key == key_t::K_F1) ch = '+';
			if (key == key_t::K_F2) ch = '-';
			if (ch) {
				_value.insert_at(caret_pos, ch);
				caret_pos++;
				goto lab_update_input;
			}
		}
		return;
lab_update_input:
		lab.text = _value;
		s32 aw,ah;
		get_preferred_size(aw,ah);

		if (parent) {
			parent->child_request_size_change(this, aw,ah);
		}
		dirty = true;
	}

	virtual void set_dirty(bool dirty) OVERRIDE {
		parent->dirty = true;
		super::set_dirty(dirty);
	}

	virtual void set_selected(bool selected) {
		_MY_ASSERT(parent,return);
		dirty = true;
		focus_client_t::set_selected(selected);
	}

	virtual void render(surface_t &dst) OVERRIDE {
		s32 ax, ay;
		translate(ax,ay);
		if (selected) {
			dst.ctx.reset();
			dst.ctx.pen_color = cursor_visible ? COLOR_CAPTURED : COLOR_SELECTED;
			dst.fill(ax,ay,w,h);
		}
		// draw vline
		if (cursor_visible) {
			dst.ctx.pen_color = cursor_color;
			s32 acp = 0;
			_MY_ASSERT(caret_pos <= INPUT_MAX_LEN,return);
			if (caret_pos) 
				acp += cursor_pos[caret_pos-1];

			dst.line(ax + acp, ay, lab.h,true);
		}

//		render_children(dst);
	}

};

// ����� � �������� ������ �� ������ ��������
class combo_box_t : public gobject_t, public focus_client_t, public focus_aware_t {
	typedef gobject_t super;

	class empty_iterator_t : public iterator_t<string_t> {
	public:
		virtual string_t* next(void* prev) OVERRIDE {
			return 0;
		}
	};
public:
	label_t lab;
	wo_property_t<iterator_t<string_t>*,combo_box_t> values;
	property_t<string_t, combo_box_t> value;
private:
	string_impl_t<INPUT_MAX_LEN> _value;
	iterator_t<string_t> *_values;
	bool captured;
	string_t *sprev;
	reverse_iterator_t<string_t> revit;
	empty_iterator_t empty_iterator;
private:
	void set_values(iterator_t<string_t> *values) {
		_values = values;
		if (!values)
			_values = &empty_iterator;
		revit.assign(_values);
		sprev = 0;
	}

	void set_value(string_t value) {
		_value=(value);
		lab.text = _value;
	}

	string_t get_value() {
		return _value;
	}
public:

	combo_box_t() {
		captured = false;
		_values = 0;
		sprev = 0;
		lab.ctx.sctx.pen_color = 0x00;
		lab.visible = true;
		values.init(this,&combo_box_t::set_values);
		value.init(this,&combo_box_t::get_value, &combo_box_t::set_value);

		values = &empty_iterator;
	}

	virtual void set_dirty(bool dirty) OVERRIDE {
		parent->dirty = true;
		super::set_dirty(dirty);
	}

	virtual void get_preferred_size(s32 &aw, s32 &ah) OVERRIDE {
		lab.get_preferred_size(aw,ah);
		//if (h < lab.h) h = lab.h;
		//if (w < lab.w + 5) w = lab.w + 5;
	}

	virtual void do_layout() OVERRIDE {
		lab.x = 0;
		lab.y = 0;
		lab.w = w;
		lab.h = h;
	}

	virtual gobject_t* next_all(void* prev) OVERRIDE {
		if (!prev) return &lab;
		return 0;
	}

	virtual void key_event(key_t::key_t key) OVERRIDE {
		_MY_ASSERT(_values,return);

		if (key == key_t::K_ENTER) {
			_MY_ASSERT(parent,return);
			if (!captured) {
				this->capture_focus();
				captured = true;
			} else {
				this->release_focus();
				captured = false;
			}
			goto lab_update_cbox;
		}
		//_MY_ASSERT(captured,return); - ����� � �� ���� ��������, ���� focus_manager_t �� ���� ���������� ������

		if (key == key_t::K_UP) {
			sprev = revit.next(sprev);
			if (!sprev) {
				sprev = revit.next(sprev);
				if (!sprev)
					return;
			}
			goto lab_update_cbox;
		}
		if (key == key_t::K_DOWN) {
			sprev = _values->next(sprev);
			if (!sprev) {
				sprev = _values->next(sprev);
				if (!sprev)
					return;
			}
			goto lab_update_cbox;
		}
		return;

lab_update_cbox:

		if (sprev)
			_value=(*sprev);
		lab.text = _value;
		s32 aw,ah;
		get_preferred_size(aw,ah);
		if (parent)
			parent->child_request_size_change(this, aw,ah);
		dirty = true;
	}

	virtual void render(surface_t &dst) OVERRIDE {
		s32 ax, ay;
		translate(ax,ay);
		if (selected) {
			dst.ctx.reset();
			dst.ctx.pen_color = captured ? COLOR_CAPTURED : COLOR_SELECTED;
			dst.fill(ax,ay,w,h);
		}
//		render_children(dst);
	}

};


// ������� ��������� �������� - ������ ���������� � ������ ������ �����������

#define MAX_MODAL_ITEMS 4

class modal_overlay_t : public gobject_t {
private:
	s32 _prev;
public:
	stack_t<gobject_t *, MAX_MODAL_ITEMS> modals;

	void push_modal(gobject_t *modal) {
		modals.push(modal);
		modal->parent = this;
		modal->dirty = true;
	}

	void pop_modal(gobject_t *modal) {
		gobject_t *popped = modals.pop();
		_MY_ASSERT(modal == popped,return);
		modal->parent = 0;
		modals.last()->dirty = true;
	}

	virtual gobject_t* next_all(void* prev) OVERRIDE {
		_MY_ASSERT(modals.length(),return 0);
		if (!prev) {
			_prev = 0;
			return modals[0];
		}
		_MY_ASSERT(prev == modals[_prev],return 0);
		_prev++;
		if (_prev < modals.length())
			return modals[_prev];
		else
			return 0;
	}


};

namespace globals {
extern modal_overlay_t modal_overlay;
}

} // ns
#endif
