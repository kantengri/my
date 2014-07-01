#include "gtest/gtest.h"
#include "custom_views.h"

using namespace custom;

custom::converter_factory_t custom::converter_factory_t::_instance;


TEST(ConverterTest, String) {
	converter_t *conv = converter_factory_t::instance().for_type(gen::variant_type_t::STRING);
	
	variant_t value;
	conv->from_string(myvi::string_t("123"), value);
	EXPECT_EQ(value.get_string_value() , "123");
}

TEST(ConverterTest, Int) {
	converter_t *conv = converter_factory_t::instance().for_type(gen::variant_type_t::INT);
	
	variant_t value;
	conv->from_string(myvi::string_t("123"), value);
	EXPECT_EQ(value.get_int_value() , 123);
}

TEST(ConverterTest, Float) {
	converter_t *conv = converter_factory_t::instance().for_type(gen::variant_type_t::FLOAT);
	
	variant_t value;
	conv->from_string(myvi::string_t("123"), value);
	EXPECT_EQ(value.get_float_value() , 123.0);
}