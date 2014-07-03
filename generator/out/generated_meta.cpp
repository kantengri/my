
#include "generated_meta.h"

using namespace gen;

void meta_registry_t::init() {

/*
 * =================== ���� ==========================
*/
	
	// string
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","string")
			->set_string_param("type","base")
		);
	// float
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","float")
			->set_string_param("type","base")
		);
	// u8
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","u8")
			->set_string_param("type","base")
		);
	// s8
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","s8")
			->set_string_param("type","base")
		);
	// u16
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","u16")
			->set_string_param("type","base")
		);
	// s16
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","s16")
			->set_string_param("type","base")
		);
	// u32
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","u32")
			->set_string_param("type","base")
		);
	// s32
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","s32")
			->set_string_param("type","base")
		);
	// dme_sfx_t
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","dme_sfx_t")
			->set_string_param("name","������� DME")
			->set_string_param("type","enum")
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","X")
					->set_string_param("name","X")
					->set_int_param("value",0)
			)
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","Y")
					->set_string_param("name","Y")
					->set_int_param("value",1)
			)
		);
	// dme_t
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","dme_t")
			->set_string_param("name","��������� DME")
			->set_string_param("type","complex")
			->add_parameter(
				(new dynamic_parameter_meta_t())
					->set_int_param("hi",128)
					->set_string_param("id","ch")
					->set_int_param("initial",127)
					->set_int_param("lo",-127)
					->set_string_param("name","����� DME")
					->set_string_param("type","u8")
					->set_string_param("validators","range")
			)
			->add_parameter(
				(new dynamic_parameter_meta_t())
					->set_string_param("id","sfx")
					->set_int_param("initial",0)
					->set_string_param("name","������� DME")
					->set_string_param("type","dme_sfx_t")
			)
		);
	// measure_ctl_t
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","measure_ctl_t")
			->set_string_param("name","���������")
			->set_string_param("type","enum")
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("glyph","A")
					->set_string_param("id","START")
					->set_string_param("name","�����")
					->set_int_param("value",0)
			)
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("glyph","B")
					->set_string_param("id","STOP")
					->set_string_param("name","����")
					->set_int_param("value",1)
			)
		);
	// output_mode_t
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","output_mode_t")
			->set_string_param("name","�����")
			->set_string_param("type","enum")
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","ANT")
					->set_string_param("name","�������")
					->set_int_param("value",0)
			)
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","ATTEN")
					->set_string_param("name","�����.")
					->set_int_param("value",1)
			)
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","CALIB")
					->set_string_param("name","������.")
					->set_int_param("value",2)
			)
		);
	// start_ctl_t
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","start_ctl_t")
			->set_string_param("name","������")
			->set_string_param("type","enum")
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","HIP")
					->set_string_param("name","���")
					->set_int_param("value",0)
			)
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","CALIB")
					->set_string_param("name","�����")
					->set_int_param("value",1)
			)
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","EXT")
					->set_string_param("name","����")
					->set_int_param("value",2)
			)
		);
	// mode_ctl_t
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","mode_ctl_t")
			->set_string_param("name","�����")
			->set_string_param("type","enum")
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","REQESTER")
					->set_string_param("name","���������")
					->set_int_param("value",0)
			)
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","RESPONDER")
					->set_string_param("name","��������")
					->set_int_param("value",1)
			)
		);
	// dsp_onoff_t
	types.push_back(
		(new dynamic_type_meta_t())
			->set_string_param("id","dsp_onoff_t")
			->set_string_param("name","���������� �������")
			->set_string_param("type","enum")
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","NO")
					->set_string_param("name","���")
					->set_int_param("value",0)
			)
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","1_MIN")
					->set_string_param("name","1 ������")
					->set_int_param("value",0)
			)
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","5_MIN")
					->set_string_param("name","5 �����")
					->set_int_param("value",1)
			)
			->add_enum_value(
				(new dynamic_enum_meta_t())
					->set_string_param("id","10_MIN")
					->set_string_param("name","10 �����")
					->set_int_param("value",2)
			)
		);
	
/*
 * =================== ��������� ==========================
*/
	// dme
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","dme")
			->set_string_param("name","��������� DME")
			->set_string_param("type","dme_t")
			->set_string_param("view","dme")
		);
	// measure_ctl
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","measure_ctl")
			->set_int_param("initial",0)
			->set_string_param("name","���������")
			->set_string_param("type","measure_ctl_t")
		);
	// output_mode
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","output_mode")
			->set_int_param("initial",0)
			->set_string_param("name","�����")
			->set_string_param("type","output_mode_t")
		);
	// out_level
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_int_param("hi",70)
			->set_string_param("id","out_level")
			->set_float_param("initial",72.3)
			->set_string_param("label","dBm")
			->set_int_param("lo",-70)
			->set_string_param("name","�������")
			->set_float_param("precision",0.1)
			->set_string_param("type","float")
			->set_string_param("validators","range")
			->set_string_param("view","tbox_label")
		);
	// start_ctl
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","start_ctl")
			->set_int_param("initial",0)
			->set_string_param("name","������")
			->set_string_param("type","start_ctl_t")
		);
	// req_period
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","req_period")
			->set_int_param("initial",1300)
			->set_string_param("label","���. ���\\�")
			->set_string_param("name","������ ��������")
			->set_string_param("type","u32")
			->set_string_param("view","tbox_label")
		);
	// req_freq
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","req_freq")
			->set_float_param("initial",1050.20)
			->set_string_param("label","���")
			->set_string_param("name","������� �������")
			->set_string_param("type","float")
			->set_string_param("view","tbox_label")
		);
	// req_span
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","req_span")
			->set_float_param("initial",12.0)
			->set_string_param("label","���")
			->set_string_param("name","������� �������� �������")
			->set_string_param("type","float")
			->set_string_param("view","tbox_label")
		);
	// rsp_span
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","rsp_span")
			->set_float_param("initial",12.0)
			->set_string_param("label","���")
			->set_string_param("name","������� �������� ������")
			->set_string_param("type","float")
			->set_string_param("view","tbox_label")
		);
	// mode_ctl
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","mode_ctl")
			->set_int_param("initial",0)
			->set_string_param("name","�����")
			->set_string_param("type","mode_ctl_t")
		);
	// brightness
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","brightness")
			->set_int_param("initial",100)
			->set_string_param("label","%")
			->set_string_param("name","�������")
			->set_string_param("type","u8")
			->set_string_param("view","tbox_label")
		);
	// dsp_onoff
	parameters.push_back(
		(new dynamic_parameter_meta_t())
			->set_string_param("id","dsp_onoff")
			->set_int_param("initial",0)
			->set_string_param("name","���������� �������")
			->set_string_param("type","dsp_onoff_t")
		);
	
/*
 * =================== ���� ==========================
*/

	// tbox
	views.push_back(
		(new dynamic_view_meta_t())
			->set_string_param("controller","tbox")
			->set_string_param("id","tbox")
			->set_string_param("kind","predefined")
		);
	// cbox
	views.push_back(
		(new dynamic_view_meta_t())
			->set_string_param("controller","cbox")
			->set_string_param("id","cbox")
			->set_string_param("kind","predefined")
		);
	// lab
	views.push_back(
		(new dynamic_view_meta_t())
			->set_string_param("controller","lab")
			->set_string_param("id","lab")
			->set_string_param("kind","predefined")
		);
	// tbox_cbox
	views.push_back(
		(new dynamic_view_meta_t())
			->set_string_param("id","tbox_cbox")
			->set_string_param("layout","stack")
			->set_string_param("preferred_item_size","true")
			->set_string_param("vertical","false")
		->add_child(
			(new dynamic_view_meta_t())
				->set_string_param("id","tbox")
				->set_string_param("inherited","tbox")
		)
		->add_child(
			(new dynamic_view_meta_t())
				->set_string_param("id","cbox")
				->set_string_param("inherited","cbox")
		)
		);
	// tbox_label
	views.push_back(
		(new dynamic_view_meta_t())
			->set_string_param("id","tbox_label")
			->set_string_param("layout","stack")
			->set_string_param("preferred_item_size","true")
			->set_string_param("vertical","false")
		->add_child(
			(new dynamic_view_meta_t())
				->set_string_param("id","tbox")
				->set_string_param("inherited","tbox")
		)
		->add_child(
			(new dynamic_view_meta_t())
				->set_string_param("id","lab")
				->set_string_param("inherited","lab")
				->set_string_param("labelSource","label")
		)
		);
	// dme
	views.push_back(
		(new dynamic_view_meta_t())
			->set_string_param("id","dme")
			->set_string_param("layout","stack")
			->set_string_param("preferred_item_size","true")
			->set_string_param("vertical","false")
		->add_child(
			(new dynamic_view_meta_t())
				->set_string_param("id","ch")
				->set_string_param("inherited","tbox")
				->set_string_param("parameterPath",".ch")
		)
		->add_child(
			(new dynamic_view_meta_t())
				->set_string_param("id","sfx")
				->set_string_param("inherited","cbox")
				->set_string_param("parameterPath",".sfx")
		)
		);
	// default_composite_template
	views.push_back(
		(new dynamic_view_meta_t())
			->set_string_param("id","default_composite_template")
			->set_string_param("layout","stack")
			->set_string_param("vertical","true")
		);
	// root
	views.push_back(
		(new dynamic_view_meta_t())
			->set_string_param("background","BACKGROUND_LIGHT")
			->set_string_param("drawer","background")
			->set_string_param("id","root")
			->set_string_param("layout","stack")
			->set_string_param("preferred_item_size","true")
			->set_string_param("vertical","true")
		->add_child(
			(new dynamic_view_meta_t())
				->set_string_param("id","menu_name_lab")
				->set_string_param("inherited","lab")
				->set_string_param("staticText","menu_name")
		)
		->add_child(
			(new dynamic_view_meta_t())
				->set_string_param("id","scroll_window")
				->set_string_param("kind","predefined")
			->add_child(
				(new dynamic_view_meta_t())
					->set_string_param("id","scroll_interior")
					->set_string_param("layout","stretch")
				->add_child(
					(new dynamic_view_meta_t())
						->set_string_param("controller","window_selector")
						->set_string_param("id","window_selector")
						->set_string_param("initial","main")
						->set_string_param("layout","stack")
						->set_string_param("nameLabelPath","root.menu_name_lab")
						->set_string_param("preferred_item_size","true")
						->set_string_param("vertical","true")
					->add_child(
						(new dynamic_view_meta_t())
							->set_string_param("controller","menu")
							->set_string_param("id","main")
							->set_string_param("itemTemplateView","menu_item")
							->set_string_param("layout","stack")
							->set_string_param("menuRef","main")
							->set_string_param("name","�������")
							->set_string_param("vertical","true")
		)
					->add_child(
						(new dynamic_view_meta_t())
							->set_string_param("controller","menu")
							->set_string_param("id","parameters")
							->set_string_param("itemTemplateView","menu_item")
							->set_string_param("layout","stack")
							->set_string_param("menuRef","parameters")
							->set_string_param("name","���������")
							->set_string_param("vertical","true")
		)
					->add_child(
						(new dynamic_view_meta_t())
							->set_string_param("controller","menu")
							->set_string_param("id","screen")
							->set_string_param("itemTemplateView","menu_item")
							->set_string_param("layout","stack")
							->set_string_param("menuRef","screen")
							->set_string_param("name","�����")
							->set_string_param("vertical","true")
		)
		)
		)
		)
		);
	// menu_item
	views.push_back(
		(new dynamic_view_meta_t())
			->set_string_param("id","menu_item")
			->set_string_param("layout","menu_item")
		->add_child(
			(new dynamic_view_meta_t())
				->set_string_param("id","lab")
				->set_string_param("inherited","lab")
		)
		);

/*
 * =================== ���� ==========================
*/

	// main
	menus.push_back(
		(new dynamic_menu_meta_t())
			->set_string_param("id","main")
			->set_string_param("name","�������")
			->add_child("measure_ctl")
			->add_child("dme")
			->add_child("output_mode")
			->add_child("out_level")
			->add_child("start_ctl")
		);
	// parameters
	menus.push_back(
		(new dynamic_menu_meta_t())
			->set_string_param("id","parameters")
			->set_string_param("name","���������")
			->add_child("req_period")
			->add_child("req_freq")
			->add_child("req_span")
			->add_child("rsp_span")
			->add_child("mode_ctl")
		);
	// screen
	menus.push_back(
		(new dynamic_menu_meta_t())
			->set_string_param("id","screen")
			->set_string_param("name","�����")
			->add_child("brightness")
			->add_child("dsp_onoff")
		);

}