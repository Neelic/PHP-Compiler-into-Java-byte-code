enum ExprType
{
    int_val,
    float_val,
    string_val,
    command_string_val,
    variable,
    constant,
    assign_op,
    assign_ref_op,
    int_cast,
    float_cast,
    string_cast,
    array_cast,
    object_cast,
    bool_cast,
    class_inst_field_ref_op,
    class_inst_field_by_ref_op,
    class_field_ref_op,
    class_field_by_ref_op,
    branch,
    minus_op,
    plus_op,
    mult_op,
    div_op,
    mod_op,
    pow_op,
    concat_op,
    bool_less,
    bool_more,
    bool_or,
    bool_and,
    bool_equal,
    bool_equal_strict,
    bool_equal_more,
    bool_not,
    logic_or,
    logic_and,
    logic_xor,
    bool_equal_less,
    bitwise_shift_l,
    bitwise_shift_r,
    bitwise_xor,
    bitwise_and,
    bitwise_or,
    bitwise_not,
    u_minus_op,
    u_plus_op,
    clone_op,
    ternary_op,
    ref_op,
    get_array_val,
    add_array_val,
    call_anon_func,
    decl_anon_func,
    new_decl,
};