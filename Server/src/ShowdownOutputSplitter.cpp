#include "ShowdownOutputSplitter.hpp"
#include <vector>
#include <functional>

void SplitOnCharacter(char c, const std::string& input, std::vector<std::string>& vec)
{
    size_t cursor = 0;

    while (cursor < input.size())
    {
        size_t found = input.find(c, cursor);

        if (found == input.npos)
        {
            // no more lines
            std::string substr = input.substr(cursor);
            vec.push_back(substr);
            cursor = input.size();
        }
        else
        {
            std::string substr = input.substr(cursor, found - cursor);
            vec.push_back(substr);
            cursor = found + 1;
        }
    }
}

void SplitShowdownOutput(const std::string& showdown_out, std::string* p1, std::string* p2)
{
    std::function<void(std::string*, const std::string&)> writeToPlayerBuf =
        [](std::string* out, const std::string& msg)
    {
        if (out == NULL) return;
        *out += msg + '\n';
    };
    
    std::vector<std::string> lines;
    SplitOnCharacter('\n', showdown_out, lines);

    // now we have a vector of all the individual lines of the output.
    // we just need to split them between the players.

    bool areSideUpdating = false;
    std::string* sideupdateTarget = NULL;

    for (int i = 0; i < lines.size(); i++)
    {
        const std::string& line = lines[i];

        if (line == "")
        {
            areSideUpdating = false;
            sideupdateTarget = NULL;
            continue;
        }

        // This indicates a comment
        if (line[0] != '|' && line[0] != 's') continue;

        if (areSideUpdating)
        {
            writeToPlayerBuf(sideupdateTarget, line);
        }

        std::vector<std::string> cmd;
        SplitOnCharacter('|', line, cmd);

        // Get the special cases out of the way
        if (cmd[0] == "sideupdate")
        {
            i++;
            const std::string& target = lines[i];
            if (target == "p1")
            {
                areSideUpdating = true;
                sideupdateTarget = p1;
            }
            else if (target == "p2")
            {
                areSideUpdating = true;
                sideupdateTarget = p2;
            }
        }
        else if (cmd[1] == "split")
        {
            i++;
            const std::string& secretline = lines[i];
            i++;
            const std::string& publicline = lines[i];

            if (cmd[2] == "p1")
            {
                writeToPlayerBuf(p1, secretline);
                writeToPlayerBuf(p2, publicline);
            }
            else if (cmd[2] == "p2")
            {
                writeToPlayerBuf(p1, publicline);
                writeToPlayerBuf(p2, secretline);
            }
            else
            {
                writeToPlayerBuf(p1, publicline);
                writeToPlayerBuf(p2, publicline);
            }
        }
        else
        {
            writeToPlayerBuf(p1, line);
            writeToPlayerBuf(p2, line);
        }
    }
}