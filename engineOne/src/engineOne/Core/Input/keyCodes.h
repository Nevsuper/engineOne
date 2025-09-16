#pragma once

namespace keyCode
{
    // Mouse buttons
    constexpr uint8_t mouse_left = 0x01;
    constexpr uint8_t mouse_right = 0x02;
    constexpr uint8_t mouse_middle = 0x04;
    constexpr uint8_t mouse_x1 = 0x05;
    constexpr uint8_t mouse_x2 = 0x06;

    // Keyboard keys
    constexpr uint8_t key_backspace = 0x08;
    constexpr uint8_t key_tab = 0x09;
    constexpr uint8_t key_clear = 0x0C;
    constexpr uint8_t key_enter = 0x0D;
    constexpr uint8_t key_shift = 0x10;
    constexpr uint8_t key_ctrl = 0x11;
    constexpr uint8_t key_alt = 0x12;
    constexpr uint8_t key_pause = 0x13;
    constexpr uint8_t key_caps_lock = 0x14;
    constexpr uint8_t key_esc = 0x1B;
    constexpr uint8_t key_space = 0x20;
    constexpr uint8_t key_page_up = 0x21;
    constexpr uint8_t key_page_down = 0x22;
    constexpr uint8_t key_end = 0x23;
    constexpr uint8_t key_home = 0x24;
    constexpr uint8_t key_arrow_left = 0x25;
    constexpr uint8_t key_arrow_up = 0x26;
    constexpr uint8_t key_arrow_right = 0x27;
    constexpr uint8_t key_arrow_down = 0x28;
    constexpr uint8_t key_select = 0x29;
    constexpr uint8_t key_print = 0x2A;
    constexpr uint8_t key_execute = 0x2B;
    constexpr uint8_t key_print_screen = 0x2C;
    constexpr uint8_t key_insert = 0x2D;
    constexpr uint8_t key_delete = 0x2E;
    constexpr uint8_t key_help = 0x2F;

    // Number keys 0-9
    constexpr uint8_t key_0 = 0x30;
    constexpr uint8_t key_1 = 0x31;
    constexpr uint8_t key_2 = 0x32;
    constexpr uint8_t key_3 = 0x33;
    constexpr uint8_t key_4 = 0x34;
    constexpr uint8_t key_5 = 0x35;
    constexpr uint8_t key_6 = 0x36;
    constexpr uint8_t key_7 = 0x37;
    constexpr uint8_t key_8 = 0x38;
    constexpr uint8_t key_9 = 0x39;

    // Alphabet keys A-Z
    constexpr uint8_t key_a = 0x41;
    constexpr uint8_t key_b = 0x42;
    constexpr uint8_t key_c = 0x43;
    constexpr uint8_t key_d = 0x44;
    constexpr uint8_t key_e = 0x45;
    constexpr uint8_t key_f = 0x46;
    constexpr uint8_t key_g = 0x47;
    constexpr uint8_t key_h = 0x48;
    constexpr uint8_t key_i = 0x49;
    constexpr uint8_t key_j = 0x4A;
    constexpr uint8_t key_k = 0x4B;
    constexpr uint8_t key_l = 0x4C;
    constexpr uint8_t key_m = 0x4D;
    constexpr uint8_t key_n = 0x4E;
    constexpr uint8_t key_o = 0x4F;
    constexpr uint8_t key_p = 0x50;
    constexpr uint8_t key_q = 0x51;
    constexpr uint8_t key_r = 0x52;
    constexpr uint8_t key_s = 0x53;
    constexpr uint8_t key_t = 0x54;
    constexpr uint8_t key_u = 0x55;
    constexpr uint8_t key_v = 0x56;
    constexpr uint8_t key_w = 0x57;
    constexpr uint8_t key_x = 0x58;
    constexpr uint8_t key_y = 0x59;
    constexpr uint8_t key_z = 0x5A;

    // Numpad keys
    constexpr uint8_t key_numpad_0 = 0x60;
    constexpr uint8_t key_numpad_1 = 0x61;
    constexpr uint8_t key_numpad_2 = 0x62;
    constexpr uint8_t key_numpad_3 = 0x63;
    constexpr uint8_t key_numpad_4 = 0x64;
    constexpr uint8_t key_numpad_5 = 0x65;
    constexpr uint8_t key_numpad_6 = 0x66;
    constexpr uint8_t key_numpad_7 = 0x67;
    constexpr uint8_t key_numpad_8 = 0x68;
    constexpr uint8_t key_numpad_9 = 0x69;
    constexpr uint8_t key_multiply = 0x6A;
    constexpr uint8_t key_add = 0x6B;
    constexpr uint8_t key_separator = 0x6C;
    constexpr uint8_t key_subtract = 0x6D;
    constexpr uint8_t key_decimal = 0x6E;
    constexpr uint8_t key_divide = 0x6F;

    // Function keys
    constexpr uint8_t key_f1 = 0x70;
    constexpr uint8_t key_f2 = 0x71;
    constexpr uint8_t key_f3 = 0x72;
    constexpr uint8_t key_f4 = 0x73;
    constexpr uint8_t key_f5 = 0x74;
    constexpr uint8_t key_f6 = 0x75;
    constexpr uint8_t key_f7 = 0x76;
    constexpr uint8_t key_f8 = 0x77;
    constexpr uint8_t key_f9 = 0x78;
    constexpr uint8_t key_f10 = 0x79;
    constexpr uint8_t key_f11 = 0x7A;
    constexpr uint8_t key_f12 = 0x7B;
    constexpr uint8_t key_f13 = 0x7C;
    constexpr uint8_t key_f14 = 0x7D;
    constexpr uint8_t key_f15 = 0x7E;
    constexpr uint8_t key_f16 = 0x7F;
    constexpr uint8_t key_f17 = 0x80;
    constexpr uint8_t key_f18 = 0x81;
    constexpr uint8_t key_f19 = 0x82;
    constexpr uint8_t key_f20 = 0x83;
    constexpr uint8_t key_f21 = 0x84;
    constexpr uint8_t key_f22 = 0x85;
    constexpr uint8_t key_f23 = 0x86;
    constexpr uint8_t key_f24 = 0x87;

    // Other keys
    constexpr uint8_t key_num_lock = 0x90;
    constexpr uint8_t key_scroll_lock = 0x91;
    constexpr uint8_t key_left_shift = 0xA0;
    constexpr uint8_t key_right_shift = 0xA1;
    constexpr uint8_t key_left_ctrl = 0xA2;
    constexpr uint8_t key_right_ctrl = 0xA3;
    constexpr uint8_t key_left_alt = 0xA4;
    constexpr uint8_t key_right_alt = 0xA5;
    constexpr uint8_t key_semicolon = 0xBA;   // ;: key
    constexpr uint8_t key_plus = 0xBB;        // =+ key
    constexpr uint8_t key_comma = 0xBC;       // ,< key
    constexpr uint8_t key_minus = 0xBD;       // -_ key
    constexpr uint8_t key_period = 0xBE;      // .> key
    constexpr uint8_t key_slash = 0xBF;       // /? key
    constexpr uint8_t key_grave = 0xC0;       // `~ key
    constexpr uint8_t key_bracket_left = 0xDB;  // [{ key
    constexpr uint8_t key_backslash = 0xDC;   // \| key
    constexpr uint8_t key_bracket_right = 0xDD; // ]} key
    constexpr uint8_t key_quote = 0xDE;       // '" key
    constexpr uint8_t key_oem_8 = 0xDF;
    constexpr uint8_t key_process = 0xE5;
    constexpr uint8_t key_packet = 0xE7;
    constexpr uint8_t key_attn = 0xF6;
    constexpr uint8_t key_crsel = 0xF7;
    constexpr uint8_t key_exsel = 0xF8;
    constexpr uint8_t key_ereof = 0xF9;
    constexpr uint8_t key_play = 0xFA;
    constexpr uint8_t key_zoom = 0xFB;
    constexpr uint8_t key_noname = 0xFC;
    constexpr uint8_t key_pa1 = 0xFD;
    constexpr uint8_t key_oem_clear = 0xFE;

} // namespace keyCode