// automatic generated by vnlib.
enum {
	PROD__0 = 0, // -> script
	PROD_attribute_0 = 26, //attribute -> @identifier '=' value
	PROD_number_0 = 10, //number -> @integer
	PROD_number_1 = 11, //number -> @long_integer
	PROD_number_2 = 12, //number -> @float
	PROD_number_3 = 13, //number -> @double
	PROD_object_0 = 16, //object -> object_head object_body
	PROD_object_1 = 17, //object -> '[' value_list ']'
	PROD_object_body_0 = 20, //object_body -> '{' '}'
	PROD_object_body_1 = 21, //object_body -> '{' object_member_list '}'
	PROD_object_head_0 = 18, //object_head -> '@'
	PROD_object_head_1 = 19, //object_head -> '@' @identifier
	PROD_object_member_0 = 24, //object_member -> value
	PROD_object_member_1 = 25, //object_member -> attribute
	PROD_object_member_list_0 = 22, //object_member_list -> object_member
	PROD_object_member_list_1 = 23, //object_member_list -> object_member_list object_member
	PROD_script_0 = 27, //script -> object_member_list
	PROD_value_0 = 1, //value -> 'null'
	PROD_value_1 = 2, //value -> 'true'
	PROD_value_2 = 3, //value -> 'false'
	PROD_value_3 = 4, //value -> number
	PROD_value_4 = 5, //value -> '+' number
	PROD_value_5 = 6, //value -> '-' number
	PROD_value_6 = 7, //value -> @string_literal
	PROD_value_7 = 8, //value -> @identifier
	PROD_value_8 = 9, //value -> object
	PROD_value_list_0 = 14, //value_list -> value
	PROD_value_list_1 = 15, //value_list -> value_list value
	PROD_COUNT = 28
};
