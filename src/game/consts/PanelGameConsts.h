#ifndef PANELGAMECONST_H
#define PANELGAMECONST_H

namespace C_Difficulty
{
    static const int DIFF_EASY = 0;
    static const int DIFF_MEDIUM = 1;
    static const int DIFF_HARD = 2;

    inline unsigned long GetMaxColorByDifficulty(int p_DifficultyID)
    {
        switch ( p_DifficultyID )
        {
            case C_Difficulty::DIFF_EASY:
                return 4;
                break;
            case C_Difficulty::DIFF_MEDIUM:
                return 5;
                break;
            case C_Difficulty::DIFF_HARD:
                return 6;
                break;
            default:
                return 4;
                break;
        }
    }
};

namespace C_BlockTimers
{
    static const unsigned long TIMERSWITCH_EASY = 80;
    static const unsigned long TIMERSWITCH_MEDIUM = 60;
    static const unsigned long TIMERSWITCH_HARD = 40;

    static const unsigned long TIMERSUSPEND_EASY = 300;
    static const unsigned long TIMERSUSPEND_MEDIUM = 220;
    static const unsigned long TIMERSUSPEND_HARD = 120;

    static const unsigned long TIMERFALL_EASY = 100;
    static const unsigned long TIMERFALL_MEDIUM = 60;
    static const unsigned long TIMERFALL_HARD = 20;

    static const unsigned long TIMERPOP_GROUPBASE = 1000;
    static const unsigned long TIMERPOP_EASY = 200;
    static const unsigned long TIMERPOP_MEDIUM = 150;
    static const unsigned long TIMERPOP_HARD = 100;

    inline unsigned long GetSwitchTime(int p_DifficultyID)
    {
        switch ( p_DifficultyID )
        {
            case C_Difficulty::DIFF_EASY:
                return TIMERSWITCH_EASY;
                break;
            case C_Difficulty::DIFF_MEDIUM:
                return TIMERSWITCH_MEDIUM;
                break;
            case C_Difficulty::DIFF_HARD:
                return TIMERSWITCH_HARD;
                break;
            default:
                return TIMERSWITCH_EASY;
                break;
        }
    }

    inline unsigned long GetSuspendTime(int p_DifficultyID)
    {
        switch ( p_DifficultyID )
        {
            case C_Difficulty::DIFF_EASY:
                return TIMERSUSPEND_EASY;
                break;
            case C_Difficulty::DIFF_MEDIUM:
                return TIMERSUSPEND_MEDIUM;
                break;
            case C_Difficulty::DIFF_HARD:
                return TIMERSUSPEND_HARD;
                break;
            default:
                return TIMERSUSPEND_EASY;
                break;
        }
    }

    inline unsigned long GetFallTime(int p_DifficultyID)
    {
        switch ( p_DifficultyID )
        {
            case C_Difficulty::DIFF_EASY:
                return TIMERFALL_EASY;
                break;
            case C_Difficulty::DIFF_MEDIUM:
                return TIMERFALL_MEDIUM;
                break;
            case C_Difficulty::DIFF_HARD:
                return TIMERFALL_HARD;
                break;
            default:
                return TIMERFALL_EASY;
                break;
        }
    }

    inline unsigned long GetPopTime(int p_DifficultyID)
    {
        switch ( p_DifficultyID )
        {
            case C_Difficulty::DIFF_EASY:
                return TIMERPOP_EASY;
                break;
            case C_Difficulty::DIFF_MEDIUM:
                return TIMERPOP_MEDIUM;
                break;
            case C_Difficulty::DIFF_HARD:
                return TIMERPOP_HARD;
                break;
            default:
                return TIMERPOP_EASY;
                break;
        }
    }

};

namespace C_Field
{

    static const unsigned long BLOCKSPERLINE = 6;
    static const unsigned long LINESPERFIELD = 12;
    static const unsigned long MAXBLOCKSONSEED = BLOCKSPERLINE*64;

    static const unsigned long TIMERLINERISE_BASE = 8000;
    static const unsigned long TIMERLINERISE_FAST = 500;

    inline unsigned long GetLineRiseTime(int p_Speed)
    {
        return TIMERLINERISE_BASE - floor( (float)(TIMERLINERISE_BASE - TIMERLINERISE_FAST) * ((float)p_Speed / (float)100.00) );
    }

    inline unsigned long GetFastLineRiseTime(int p_Speed)
    {
        return TIMERLINERISE_FAST;
    }

};

#endif // PANELGAMECONST_H
