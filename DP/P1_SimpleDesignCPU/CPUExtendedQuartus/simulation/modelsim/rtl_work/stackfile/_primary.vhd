library verilog;
use verilog.vl_types.all;
entity stackfile is
    port(
        clk             : in     vl_logic;
        rws             : in     vl_logic;
        wsp             : in     vl_logic;
        push_d          : in     vl_logic_vector(15 downto 0);
        pop_d           : out    vl_logic_vector(15 downto 0)
    );
end stackfile;
