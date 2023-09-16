library verilog;
use verilog.vl_types.all;
entity alu is
    port(
        a               : in     vl_logic_vector(15 downto 0);
        b               : in     vl_logic_vector(15 downto 0);
        op_alu          : in     vl_logic_vector(2 downto 0);
        y               : out    vl_logic_vector(15 downto 0);
        zero            : out    vl_logic;
        sign            : out    vl_logic;
        overflow        : out    vl_logic;
        parity          : out    vl_logic
    );
end alu;
