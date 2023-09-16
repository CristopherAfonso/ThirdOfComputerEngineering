library verilog;
use verilog.vl_types.all;
entity cd is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        s_inc           : in     vl_logic;
        s_inm           : in     vl_logic;
        s_rgj           : in     vl_logic;
        we3             : in     vl_logic;
        wez             : in     vl_logic;
        wes             : in     vl_logic;
        weo             : in     vl_logic;
        wep             : in     vl_logic;
        wed             : in     vl_logic;
        wext            : in     vl_logic;
        rws             : in     vl_logic;
        wsp             : in     vl_logic;
        wed_ext         : in     vl_logic;
        wess            : in     vl_logic;
        wro             : in     vl_logic_vector(1 downto 0);
        z               : out    vl_logic;
        s               : out    vl_logic;
        o               : out    vl_logic;
        p               : out    vl_logic;
        opcode          : out    vl_logic_vector(5 downto 0);
        dir_mem_ex      : out    vl_logic_vector(15 downto 0);
        data_bus        : inout  vl_logic_vector(15 downto 0);
        out_d0          : out    vl_logic_vector(6 downto 0);
        out_d1          : out    vl_logic_vector(6 downto 0);
        out_d2          : out    vl_logic_vector(6 downto 0);
        out_d3          : out    vl_logic_vector(6 downto 0)
    );
end cd;
