library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
entity keyboard is
generic
(
  ROTATE                    : boolean := false;   -- 是否行列互换
  LAYOUT                    : integer := 1        -- 键盘布局 0：0123/4567/89AB/CDEF
                                                  --          1: 123A/456B/789C/*0#D
);
port
(
  clk_10kHz                 : in  std_logic;
  col                       : in  std_logic_vector(3 downto 0);
  row                       : out std_logic_vector(3 downto 0);
  key_data                  : out std_logic_vector(3 downto 0);
  key_pressed               : out std_logic;
  key_0_5                   : out std_logic;
  key_0_9                   : out std_logic;
  key_A                     : out std_logic;
  key_B                     : out std_logic;
  key_C                     : out std_logic;
  key_D                     : out std_logic;
  key_E                     : out std_logic;
  key_F                     : out std_logic
);
end entity keyboard;

architecture behavior_keyboard of keyboard is

signal reg_clk_div1         : std_logic_vector(4 downto 0) := (others => '0');
signal reg_clk_div2         : std_logic_vector(6 downto 0) := (others => '0');
signal reg_clk_scan         : std_logic;
signal reg_clk_debounce     : std_logic;
signal reg_scan             : std_logic_vector(3 downto 0) := (others => '0');
signal reg_scan_en          : std_logic;
signal reg_key_press        : std_logic;
signal reg_key_lock         : std_logic;
signal reg_key_value        : std_logic_vector(3 downto 0) := (others => '0');
signal reg_key_index        : integer;

type  state is (wait_key_press,stop_key_scan,lock_key_value,wait_key_release);
signal pre_state            : state;

type  key_value is array(0 to 16) of std_logic_vector(3 downto 0);

constant key_layout_A : key_value :=
(
  "0000","0001","0010","0011",
  "0100","0101","0110","0111",
  "1000","1001","1010","1011",
  "1100","1101","1110","1111",
  "0000"
);

constant key_layout_B : key_value :=
(
  "0001","0010","0011","1010",
  "0100","0101","0110","1011",
  "0111","1000","1001","1100",
  "1110","0000","1111","1101",
  "0000"
);

begin

-- 时钟分频
process(clk_10kHz)
begin
  if rising_edge(clk_10kHz) then
    if reg_clk_div1 = 24 then
      reg_clk_div1 <= (others => '0');
      reg_clk_debounce <= not reg_clk_debounce;
    else
      reg_clk_div1 <= reg_clk_div1 + 1;
    end if;
  end if;
end process;

process(clk_10kHz)
begin
  if rising_edge(clk_10kHz) then
    if reg_clk_div2 = 99 then
      reg_clk_div2 <= (others => '0');
      reg_clk_scan <= not reg_clk_scan;
    else
      reg_clk_div2 <= reg_clk_div2 + 1;
    end if;
  end if;
end process;

-- 扫描行线
process(reg_clk_scan)
begin
  if rising_edge(reg_clk_scan) then
    if reg_scan = 0 then
      reg_scan <= "1110";
    else
      if reg_scan_en = '1' then
        reg_scan(3 downto 1) <= reg_scan(2 downto 0);
        reg_scan(0) <= reg_scan(3);
      end if;
    end if;
  end if;
end process;

-- 消抖，判断有键按下
process(reg_clk_debounce)
begin
  if rising_edge(reg_clk_debounce) then
    if col = "1111" then
      reg_key_press <= '0';
    else
      reg_key_press <= '1';
    end if;
  end if;
end process;

process(reg_scan,col)
variable var_value        : std_logic_vector(7 downto 0);
begin
  if ROTATE then
    var_value := col & reg_scan;
  else
    var_value := reg_scan & col;
  end if;
  case var_value is
    when "11101110" =>
      reg_key_index <= 0;
    when "11101101" =>
      reg_key_index <= 1;
    when "11101011" =>
      reg_key_index <= 2;
    when "11100111" =>
      reg_key_index <= 3;
    when "11011110" =>
      reg_key_index <= 4;
    when "11011101" =>
      reg_key_index <= 5;
    when "11011011" =>
      reg_key_index <= 6;
    when "11010111" =>
      reg_key_index <= 7;
    when "10111110" =>
      reg_key_index <= 8;
    when "10111101" =>
      reg_key_index <= 9;
    when "10111011" =>
      reg_key_index <= 10;
    when "10110111" =>
      reg_key_index <= 11;
    when "01111110" =>
      reg_key_index <= 12;
    when "01111101" =>
      reg_key_index <= 13;
    when "01111011" =>
      reg_key_index <= 14;
    when "01110111" =>
      reg_key_index <= 15;
    when others =>
      reg_key_index <= 16;
  end case;
end process;

reg_key_value <= key_layout_A(reg_key_index) when LAYOUT = 0 else key_layout_B(reg_key_index);

-- 控制
process(clk_10kHz)
begin
  if rising_edge(clk_10kHz) then
    case pre_state is
      when wait_key_press =>
        reg_scan_en <= '1';
        reg_key_lock <= '0';
        if reg_key_press = '0' then
          pre_state <= wait_key_press;
        else
          pre_state <= stop_key_scan;
        end if;
      when stop_key_scan =>
        reg_scan_en <= '0';
        reg_key_lock <= '0';
        pre_state <= lock_key_value;
      when lock_key_value =>
        reg_scan_en <= '0';
        reg_key_lock <= '1';
        pre_state <= wait_key_release;
      when wait_key_release =>
        reg_scan_en <= '0';
        reg_key_lock <= '0';
        if reg_key_press = '0' then
          pre_state <= wait_key_press;
        else
          pre_state <= wait_key_release;
        end if;
      when others =>
          reg_scan_en <= '0';
          reg_key_lock <= '0';
          pre_state <= wait_key_press;
    end case;
  end if;
end process;

row           <= reg_scan;
key_data      <= reg_key_value;
key_pressed   <= reg_key_lock;
key_0_5       <= '1' when reg_key_value < 6  and reg_key_lock = '1' else '0';
key_0_9       <= '1' when reg_key_value < 10 and reg_key_lock = '1' else '0';
key_A         <= '1' when reg_key_value = 10 and reg_key_lock = '1' else '0';
key_B         <= '1' when reg_key_value = 11 and reg_key_lock = '1' else '0';
key_C         <= '1' when reg_key_value = 12 and reg_key_lock = '1' else '0';
key_D         <= '1' when reg_key_value = 13 and reg_key_lock = '1' else '0';
key_E         <= '1' when reg_key_value = 14 and reg_key_lock = '1' else '0';
key_F         <= '1' when reg_key_value = 15 and reg_key_lock = '1' else '0';

end architecture behavior_keyboard;