#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cctype>
#include <iomanip>

constexpr auto NL = "\n";

// program errors
enum ERRNO
{
    MISSING_PARAMETER,
    FAILED_TO_OPEN_FILE,
};

// column of scores file
enum TOKENS
{
    PERCENTAGE,
    BLOCKS,
    POS_X,
    POS_Y,
    POS_Z,
    ROT_X,
    ROT_Y,
    ROT_Z,
};

typedef struct
{
    float        percentage;
    unsigned int blocks;
} score_t;

typedef struct
{
    std::vector<score_t> scores;
    float                pos_x,
                         pos_y,
                         pos_z;
    int                  rot_x,
                         rot_y,
                         rot_z;
} match_t;

score_t find_highscore(std::vector<score_t>& scores)
{
    score_t hscore{ 0 };
    for (auto& score : scores)
    {
        if (score.blocks > hscore.blocks && score.percentage > hscore.percentage)
        {
            hscore = score;
        }
    }
    return hscore;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "missing parameters" << std::endl;
        std::cout << "Usage: " << argv[0] << " scores.txt" << std::endl;
        return MISSING_PARAMETER;
    }

    // read file
    std::ifstream score_f(argv[1]);
    if (!score_f.is_open())
    {
        std::cerr << "failed to open scores file '" << argv[1] << std::endl;
        return FAILED_TO_OPEN_FILE;
    }

    std::vector<match_t> matches;
    std::string line;
    unsigned int line_num = 0;
    while (std::getline(score_f, line))
    {
        // skip csv header and empty lines
        if (line.length() == 0 || !std::isdigit(line[0]))
        {
            continue;
        }

        std::stringstream ss(line);
        std::string token;
        unsigned int token_num = 0;
        score_t score{ 0 };
        match_t match;
        while (std::getline(ss, token, ','))
        {
            switch (token_num)
            {
            case PERCENTAGE:
                score.percentage = std::stof(token);
                break;
            case BLOCKS:
                score.blocks = std::stoul(token);
                break;
            case POS_X:
                match.pos_x = std::stof(token);
                break;
            case POS_Y:
                match.pos_y = std::stof(token);
                break;
            case POS_Z:
                match.pos_z = std::stof(token);
                break;
            case ROT_X:
                match.rot_x = std::stoul(token);
                break;
            case ROT_Y:
                match.rot_y = std::stoul(token);
                break;
            case ROT_Z:
                match.rot_z = std::stoul(token);
                break;
            }
            token_num++;
        }
        bool found = false;
        for (auto& m : matches)
        {
            if (match.pos_x == m.pos_x
                && match.pos_y == m.pos_y
                && match.pos_z == m.pos_z
                && match.rot_x == m.rot_x
                && match.rot_y == m.rot_y
                && match.rot_z == m.rot_z)
            {
                m.scores.push_back(score);
                found = true;
                break;
            }
        }
        if (!found)
        {
            match.scores.push_back(score);
            matches.push_back(match);
        }
    }
    score_f.close();

    // print data
    std::cout << "   %   | blocks | count | PosX | PosY | PosZ | RotX | RotY | RotZ" << NL
              << "---------------------------------------------------------" << NL;
    for (auto& match : matches)
    {
        score_t highscore = find_highscore(match.scores);
        std::cout << " " << std::fixed << std::setprecision(2) << highscore.percentage << " | "
                  << std::setfill(' ') << std::setw(6) << highscore.blocks << " | "
                  << std::setfill(' ') << std::setw(5) << match.scores.size() << " | "
                  << std::setfill(' ') << std::setw(4) << match.pos_x << " | "
                  << std::setfill(' ') << std::setw(4) << match.pos_y << " | "
                  << std::setfill(' ') << std::setw(4) << match.pos_z << " | "
                  << std::setfill(' ') << std::setw(4) << match.rot_x << " | "
                  << std::setfill(' ') << std::setw(4) << match.rot_y << " | "
                  << std::setfill(' ') << std::setw(4) << match.rot_z
                  << NL;
    }
    std::cout << std::endl;
    return 0;
}