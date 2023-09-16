library verilog;
use verilog.vl_types.all;
entity uc is
    port(
        opcode          : in     vl_logic_vector(5 downto 0);
        z               : in     vl_logic;
        s_inc           : out    vl_logic;
        s_inm           : out    vl_logic;
        we3             : out    vl_logic;
        wez             : out    vl_logic;
        op_alu          : out    vl_logic_vector(2 downto 0)
    );
end uc;
