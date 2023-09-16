library verilog;
use verilog.vl_types.all;
entity cd is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        s_inc           : in     vl_logic;
        s_inm           : in     vl_logic;
        we3             : in     vl_logic;
        wez             : in     vl_logic;
        op_alu          : in     vl_logic_vector(2 downto 0);
        z               : out    vl_logic;
        opcode          : out    vl_logic_vector(5 downto 0)
    );
end cd;
