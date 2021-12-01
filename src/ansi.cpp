//
// Created by volund on 11/25/21.
//

#include "ansi.h"

namespace dbat::ansi {

    std::map<uint8_t, std::string> default_choices = {
        /* COLOR_NORMAL */	{0, AA_NORMAL},
        /* COLOR_ROOMNAME */	{1, AA_NORMAL ANSISEPSTR AF_CYAN},
        /* COLOR_ROOMOBJS */	{2, AA_NORMAL ANSISEPSTR AF_GREEN},
        /* COLOR_ROOMPEOPLE */	{3, AA_NORMAL ANSISEPSTR AF_YELLOW},
        /* COLOR_HITYOU */	{4, AA_NORMAL ANSISEPSTR AF_RED},
        /* COLOR_YOUHIT */	{5, AA_NORMAL ANSISEPSTR AF_GREEN},
        /* COLOR_OTHERHIT */	{6, AA_NORMAL ANSISEPSTR AF_YELLOW},
        /* COLOR_CRITICAL */	{7, AA_BOLD ANSISEPSTR AF_YELLOW},
        /* COLOR_HOLLER */	{8, AA_BOLD ANSISEPSTR AF_YELLOW},
        /* COLOR_SHOUT */	{9, AA_BOLD ANSISEPSTR AF_YELLOW},
        /* COLOR_GOSSIP */	{10, AA_NORMAL ANSISEPSTR AF_YELLOW},
        /* COLOR_AUCTION */	{11, AA_NORMAL ANSISEPSTR AF_CYAN},
        /* COLOR_CONGRAT */	{12, AA_NORMAL ANSISEPSTR AF_GREEN},
        /* COLOR_TELL */	{13, AA_NORMAL ANSISEPSTR AF_RED},
        /* COLOR_YOUSAY */	{14, AA_NORMAL ANSISEPSTR AF_CYAN},
        /* COLOR_ROOMSAY */	{15, AA_NORMAL ANSISEPSTR AF_WHITE}
    };

    std::map<uint8_t, std::string> empty_choices;

    static const char RANDOM_COLORS[] = "bgcrmywBGCRMWY";

    /* Color replacement arrays. Orig. Renx -- 011100, now modified */
    static char *ANSI[] = {
            "@",
            AA_NORMAL,
            AA_NORMAL ANSISEPSTR AF_BLACK,
            AA_NORMAL ANSISEPSTR AF_BLUE,
            AA_NORMAL ANSISEPSTR AF_GREEN,
            AA_NORMAL ANSISEPSTR AF_CYAN,
            AA_NORMAL ANSISEPSTR AF_RED,
            AA_NORMAL ANSISEPSTR AF_MAGENTA,
            AA_NORMAL ANSISEPSTR AF_YELLOW,
            AA_NORMAL ANSISEPSTR AF_WHITE,
            AA_BOLD ANSISEPSTR AF_BLACK,
            AA_BOLD ANSISEPSTR AF_BLUE,
            AA_BOLD ANSISEPSTR AF_GREEN,
            AA_BOLD ANSISEPSTR AF_CYAN,
            AA_BOLD ANSISEPSTR AF_RED,
            AA_BOLD ANSISEPSTR AF_MAGENTA,
            AA_BOLD ANSISEPSTR AF_YELLOW,
            AA_BOLD ANSISEPSTR AF_WHITE,
            AB_BLACK,
            AB_BLUE,
            AB_GREEN,
            AB_CYAN,
            AB_RED,
            AB_MAGENTA,
            AB_YELLOW,
            AB_WHITE,
            AA_BLINK,
            AA_UNDERLINE,
            AA_BOLD,
            AA_REVERSE,
            "!"
    };

    static const char CCODE[] = "@ndbgcrmywDBGCRMYW01234567luoex!";
    /*
      Codes are:      @n - normal
      @d - black      @D - gray           @0 - background black
      @b - blue       @B - bright blue    @1 - background blue
      @g - green      @G - bright green   @2 - background green
      @c - cyan       @C - bright cyan    @3 - background cyan
      @r - red        @R - bright red     @4 - background red
      @m - magneta    @M - bright magneta @5 - background magneta
      @y - yellow     @Y - bright yellow  @6 - background yellow
      @w - white      @W - bright white   @7 - background white
      @x - random
    Extra codes:      @l - blink          @o - bold
      @u - underline  @e - reverse video  @@ - single @

      @[num] - user color choice num, [] are required
    */

    std::string processColors(const std::string& txt, bool parse, std::map<uint8_t, std::string>& choices) {

        char *dest_char, *source_char, *color_char, *save_pos, *replacement = nullptr;
        uint8_t i, temp_color;
        size_t wanted;

        if (!txt.size() || !strchr(txt.c_str(), '@')) /* skip out if no color codes     */
            return txt;


        std::string out;
        source_char = (char*)txt.c_str();

        save_pos = dest_char;
        for( ; *source_char; ) {
            /* no color code - just copy */
            if (*source_char != '@') {
                out.push_back(*source_char++);
                continue;
            }

            /* if we get here we have a color code */

            source_char++; /* source_char now points to the code */

            /* look for a random color code picks a random number between 1 and 14 */
            if (*source_char == 'x') {
                temp_color = (rand() % 14);
                *source_char = RANDOM_COLORS[temp_color];
            }

            if (*source_char == '\0') { /* string was terminated with color code - just put it in */
                out.push_back('@');
                /* source_char will now point to '\0' in the for() check */
                continue;
            }

            if (!parse) { /* not parsing, just skip the code, unless it's @@ */
                if (*source_char == '@') {
                    out.push_back('@');
                }
                if (*source_char == '[') { /* Multi-character code */
                    source_char++;
                    while (*source_char && isdigit(*source_char))
                        source_char++;
                    if (!*source_char)
                        source_char--;
                }
                source_char++; /* skip to next (non-colorcode) char */
                continue;
            }

            /* parse the color code */
            if (*source_char == '[') { /* User configurable color */
                source_char++;
                if (*source_char) {
                    i = atoi(source_char);
                    if (i < 0 || i >= NUM_COLOR)
                        i = COLOR_NORMAL;
                    replacement = (char*)default_choices[i].c_str();
                    if (choices.count(i))
                        replacement = (char*)choices[i].c_str();
                    while (*source_char && isdigit(*source_char))
                        source_char++;
                    if (!*source_char)
                        source_char--;
                }
            } else if (*source_char == 'n') {
                replacement = default_color_choices[COLOR_NORMAL];
                if (choices.count(COLOR_NORMAL))
                    replacement = (char*)choices[COLOR_NORMAL].c_str();
            } else {
                for (i = 0; CCODE[i] != '!'; i++) { /* do we find it ? */
                    if ((*source_char) == CCODE[i]) {           /* if so :*/
                        replacement = ANSI[i];
                        break;
                    }
                }
            }
            if (replacement) {
                if (isdigit(replacement[0]))
                    for(color_char = ANSISTART ; *color_char ; )
                        out.push_back(*color_char++);
                for(color_char = replacement ; *color_char ; )
                    out.push_back(*color_char++);
                if (isdigit(replacement[0]))
                    out.push_back(ANSIEND);
                replacement = nullptr;
            }
            /* If we couldn't find any correct color code, or we found it and
             * substituted above, let's just process the next character.
             * - Welcor
             */
            source_char++;

        } /* for loop */

        return out;
    }

    std::string stripAnsi(const std::string& txt) {
        return processColors(txt, false, empty_choices);
    }

}