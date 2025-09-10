#pragma once

namespace keyCode
{
    // Mouse buttons
    constexpr int mouse_left = 0x01;
    constexpr int mouse_right = 0x02;
    constexpr int mouse_middle = 0x04;
    constexpr int mouse_x1 = 0x05;
    constexpr int mouse_x2 = 0x06;

    // Keyboard keys
    constexpr int key_backspace = 0x08;
    constexpr int key_tab = 0x09;
    constexpr int key_clear = 0x0C;
    constexpr int key_enter = 0x0D;
    constexpr int key_shift = 0x10;
    constexpr int key_ctrl = 0x11;
    constexpr int key_alt = 0x12;
    constexpr int key_pause = 0x13;
    constexpr int key_caps_lock = 0x14;
    constexpr int key_esc = 0x1B;
    constexpr int key_space = 0x20;
    constexpr int key_page_up = 0x21;
    constexpr int key_page_down = 0x22;
    constexpr int key_end = 0x23;
    constexpr int key_home = 0x24;
    constexpr int key_arrow_left = 0x25;
    constexpr int key_arrow_up = 0x26;
    constexpr int key_arrow_right = 0x27;
    constexpr int key_arrow_down = 0x28;
    constexpr int key_select = 0x29;
    constexpr int key_print = 0x2A;
    constexpr int key_execute = 0x2B;
    constexpr int key_print_screen = 0x2C;
    constexpr int key_insert = 0x2D;
    constexpr int key_delete = 0x2E;
    constexpr int key_help = 0x2F;

    // Number keys 0-9
    constexpr int key_0 = 0x30;
    constexpr int key_1 = 0x31;
    constexpr int key_2 = 0x32;
    constexpr int key_3 = 0x33;
    constexpr int key_4 = 0x34;
    constexpr int key_5 = 0x35;
    constexpr int key_6 = 0x36;
    constexpr int key_7 = 0x37;
    constexpr int key_8 = 0x38;
    constexpr int key_9 = 0x39;

    // Alphabet keys A-Z
    constexpr int key_a = 0x41;
    constexpr int key_b = 0x42;
    constexpr int key_c = 0x43;
    constexpr int key_d = 0x44;
    constexpr int key_e = 0x45;
    constexpr int key_f = 0x46;
    constexpr int key_g = 0x47;
    constexpr int key_h = 0x48;
    constexpr int key_i = 0x49;
    constexpr int key_j = 0x4A;
    constexpr int key_k = 0x4B;
    constexpr int key_l = 0x4C;
    constexpr int key_m = 0x4D;
    constexpr int key_n = 0x4E;
    constexpr int key_o = 0x4F;
    constexpr int key_p = 0x50;
    constexpr int key_q = 0x51;
    constexpr int key_r = 0x52;
    constexpr int key_s = 0x53;
    constexpr int key_t = 0x54;
    constexpr int key_u = 0x55;
    constexpr int key_v = 0x56;
    constexpr int key_w = 0x57;
    constexpr int key_x = 0x58;
    constexpr int key_y = 0x59;
    constexpr int key_z = 0x5A;

    // Numpad keys
    constexpr int key_numpad_0 = 0x60;
    constexpr int key_numpad_1 = 0x61;
    constexpr int key_numpad_2 = 0x62;
    constexpr int key_numpad_3 = 0x63;
    constexpr int key_numpad_4 = 0x64;
    constexpr int key_numpad_5 = 0x65;
    constexpr int key_numpad_6 = 0x66;
    constexpr int key_numpad_7 = 0x67;
    constexpr int key_numpad_8 = 0x68;
    constexpr int key_numpad_9 = 0x69;
    constexpr int key_multiply = 0x6A;
    constexpr int key_add = 0x6B;
    constexpr int key_separator = 0x6C;
    constexpr int key_subtract = 0x6D;
    constexpr int key_decimal = 0x6E;
    constexpr int key_divide = 0x6F;

    // Function keys
    constexpr int key_f1 = 0x70;
    constexpr int key_f2 = 0x71;
    constexpr int key_f3 = 0x72;
    constexpr int key_f4 = 0x73;
    constexpr int key_f5 = 0x74;
    constexpr int key_f6 = 0x75;
    constexpr int key_f7 = 0x76;
    constexpr int key_f8 = 0x77;
    constexpr int key_f9 = 0x78;
    constexpr int key_f10 = 0x79;
    constexpr int key_f11 = 0x7A;
    constexpr int key_f12 = 0x7B;
    constexpr int key_f13 = 0x7C;
    constexpr int key_f14 = 0x7D;
    constexpr int key_f15 = 0x7E;
    constexpr int key_f16 = 0x7F;
    constexpr int key_f17 = 0x80;
    constexpr int key_f18 = 0x81;
    constexpr int key_f19 = 0x82;
    constexpr int key_f20 = 0x83;
    constexpr int key_f21 = 0x84;
    constexpr int key_f22 = 0x85;
    constexpr int key_f23 = 0x86;
    constexpr int key_f24 = 0x87;

    // Other keys
    constexpr int key_num_lock = 0x90;
    constexpr int key_scroll_lock = 0x91;
    constexpr int key_left_shift = 0xA0;
    constexpr int key_right_shift = 0xA1;
    constexpr int key_left_ctrl = 0xA2;
    constexpr int key_right_ctrl = 0xA3;
    constexpr int key_left_alt = 0xA4;
    constexpr int key_right_alt = 0xA5;
    constexpr int key_semicolon = 0xBA;   // ;: key
    constexpr int key_plus = 0xBB;        // =+ key
    constexpr int key_comma = 0xBC;       // ,< key
    constexpr int key_minus = 0xBD;       // -_ key
    constexpr int key_period = 0xBE;      // .> key
    constexpr int key_slash = 0xBF;       // /? key
    constexpr int key_grave = 0xC0;       // `~ key
    constexpr int key_bracket_left = 0xDB;  // [{ key
    constexpr int key_backslash = 0xDC;   // \| key
    constexpr int key_bracket_right = 0xDD; // ]} key
    constexpr int key_quote = 0xDE;       // '" key
    constexpr int key_oem_8 = 0xDF;
    constexpr int key_process = 0xE5;
    constexpr int key_packet = 0xE7;
    constexpr int key_attn = 0xF6;
    constexpr int key_crsel = 0xF7;
    constexpr int key_exsel = 0xF8;
    constexpr int key_ereof = 0xF9;
    constexpr int key_play = 0xFA;
    constexpr int key_zoom = 0xFB;
    constexpr int key_noname = 0xFC;
    constexpr int key_pa1 = 0xFD;
    constexpr int key_oem_clear = 0xFE;

} // namespace keyCode