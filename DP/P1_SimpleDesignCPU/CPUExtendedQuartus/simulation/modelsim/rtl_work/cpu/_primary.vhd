library verilog;
use verilog.vl_types.all;
entity cpu is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        dir_mem_ex      : out    vl_logic_vector(17 downto 0);
        write_ext       : out    vl_logic;
        read_ext        : out    vl_logic;
        we_ram          : out    vl_logic;
        data_bus        : inout  vl_logic_vector(15 downto 0);
        out_d0          : out    vl_logic_vector(6 downto 0);
        out_d1          : out    vl_logic_vector(6 downto 0);
        out_d2          : out    vl_logic_vector(6 downto 0);
        out_d3          : out    vl_logic_vector(6 downto 0)
    );
end cpu;
