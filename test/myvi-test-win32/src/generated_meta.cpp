#include "generated_meta.h"

using namespace gen;

// types
dme_t_type_meta_t dme_t_type_meta;
dme_sfx_t_type_meta_t dme_sfx_t_type_meta;

type_meta_t * meta_registry_t::types[] = {
	&dme_t_type_meta,
	&dme_sfx_t_type_meta,
	0
};

// parameters complex
ch_parameter_meta_t ch_parameter_meta;
sfx_parameter_meta_t sfx_parameter_meta;
dme_parameter_meta_t dme_parameter_meta;

parameter_meta_t * meta_registry_t::parameters[] = {
	&ch_parameter_meta,
	&sfx_parameter_meta,
	&dme_parameter_meta,
	0
};


// views predefined
lab_view_meta_t lab_view_meta;
dme_view_meta_t dme_view_meta;
// views custom
menu_view_meta_t menu_view_meta;
root_view_meta_t root_view_meta;
item_template_view_meta_t item_template_view_meta;

view_meta_t * meta_registry_t::views[] = {
	&lab_view_meta,
	&dme_view_meta,
	&menu_view_meta,
	&root_view_meta,
	&item_template_view_meta,
	0
};

// menus
menu_menu_meta_t menu_menu_meta;

menu_meta_t * meta_registry_t::menus[] = {
	&menu_menu_meta,
	0
};

