library verilog;
use verilog.vl_types.all;
entity CPU is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        opcode          : out    vl_logic_vector(5 downto 0);
        op_alu          : out    vl_logic_vector(2 downto 0)
    );
end CPU;
