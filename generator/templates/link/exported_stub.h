<#import "../inc/common.ftl" as common>
<#import "inc/link_utils.ftl" as utils>

/*
* ������ ���������� ����������
*
* WARNING: ������������� ��������������� ����
*/

#ifndef _EXPORTED_STUB_H
#define _EXPORTED_STUB_H


#include "exported_app.h"


namespace link {

// ��������� ��������� ������� ��� ��������� ���������
class host_interface_empty_impl_t : public host_interface_t {
public:
public:

<#list schema.schema.parameters.parameter as parameter>
	// ${parameter.@id}
	<@utils.emit_parameter parameter; path, type >
	virtual void ${path}_changed(${type} v) OVERRIDE {}
	</@utils.emit_parameter>
</#list>
};


// ��������� "����" ��� ��������� �������
class exported_interface_empty_impl_t : public exported_interface_t {
public:

<#list schema.schema.parameters.parameter as parameter>
	// ${parameter.@id}
	<@utils.emit_parameter parameter; path, type >
	virtual void set_${path}(${type} v) OVERRIDE {}
	</@utils.emit_parameter>
</#list>
};

} // ns gen

#endif
