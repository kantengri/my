<#import "../inc/common.ftl" as common>
<#import "inc/link_utils.ftl" as utils>

<#macro emit_host_changed expected_types>
	<#list schema.schema.parameters.parameter as parameter>
		<@utils.emit_parameter parameter ; path, type, typedef>
			<#if expected_types?seq_contains(typedef.@id) || (typedef.@type == 'enum' && expected_types?seq_contains('enum'))>
				<#local dot_path = path?replace('_','.')>
		if (strcmp(path,"${dot_path}")) host->${path}_changed((${type})value);
			</#if>
		</@utils.emit_parameter>
	</#list>		
</#macro>

/*
* Определение и реализация транспортной части интерфейсов связи прикладного уровня для прямой и обратной связи с этой системой
* Работает на стороне хост-системы !
* WARNING: АВТОМАТИЧЕСКИ СГЕНЕРИРОВАННЫЙ ФАЙЛ
*/
#ifndef _LINK_APP_H
#define _LINK_APP_H

#include "exported_app.h"
#include "exported_sys.h"

namespace link {

// принимает сообщения об обновлении модели по интерфейсу связи системного уровня и обратно
// работает в связке с внешним обработчиком интерфейса связи системного уровня на стороне хост-системы
class app_link_decoder_t : 
	public link::host_system_interface_t, 
	public link::host_model_interface_t {
public:
	link::host_interface_t *host; // прикладной интерфейс, куда декодируются входящие сообщения
public:
	app_link_decoder_t() {
		host = 0;
	}

	void init(link::host_interface_t *ahost) {
		host = ahost;
	}

	// ответ на запрос на чтение данных из модели
	virtual void read_model_data_response(char * path, char * value, u32 code) OVERRIDE {
		<@emit_host_changed ['string'] />
	}

	virtual void read_model_data_response(char * path, s32 value, u32 code ) OVERRIDE {
		<@emit_host_changed ['u8','s8','u16', 's16', 'u32', 's32', 'enum'] />
	}

	virtual void read_model_data_response(char * path, double value, u32 code ) OVERRIDE {
		<@emit_host_changed ['float'] />
	}


	// ответ на запись данных в модель
	virtual void write_model_data_ack(u32 code) OVERRIDE {
	}

private:

    bool strcmp(const char* s1, const char* s2) {
        while(*s1 && (*s1++ == *s2++));
        return *s1 == *s2;
    }

};

// формирует сообщение для  SLAVE системы на обновление данных модели
class app_link_encoder_t : public link::exported_interface_t {
public:
	link::exported_model_interface_t *exported2; // системный интерфейс slave-системы для ответа
public:
	app_link_encoder_t() {
		exported2 = 0;
	}
	void init(link::exported_model_interface_t *aexported2) {
		exported2 = aexported2;
	}

<#list schema.schema.parameters.parameter as parameter>
	// ${parameter.@id}
	<@utils.emit_parameter parameter; path, origin_type, typedef>
		<#assign dot_path = path?replace('_','.')>
		<#assign type = utils.match_variant_type(typedef.@id)>
		<#if typedef.@type == 'enum'>
			<#assign type = 's32'>
		</#if>
	virtual void set_${path}(${origin_type} value) OVERRIDE {
		exported2->write_model_data("${dot_path}", (${type})value);
	}
	</@utils.emit_parameter>
</#list>

};



} // ns custom

#endif

