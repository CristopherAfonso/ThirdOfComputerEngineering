library verilog;
use verilog.vl_types.all;
entity memprog is
    port(
        clk             : in     vl_logic;
        a               : in     vl_logic_vector(9 downto 0);
        rd              : out    vl_logic_vector(31 downto 0)
    );
end memprog;
