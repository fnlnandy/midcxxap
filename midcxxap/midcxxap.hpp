#ifndef __MIDCXXAP_HPP_INCLUDED
#define __MIDCXXAP_HPP_INCLUDED

#include <string>
#include <vector>
#include <exception>
#include <unordered_map>
#include <iostream>

namespace MidCxxAP
{
    inline bool __startswith(const std::string &s, const std::string &pref)
    {
        return s.find(pref) == 0;
    }

    inline bool __keyexists(const std::unordered_map<std::string, std::string> &m, const std::string &key)
    {
        for (auto it = m.cbegin(); it != m.cend(); ++it)
        {
            if (it->first == key)
                return true;
        }

        return false;
    }

    inline void __warning(const std::string &text)
    {
        std::printf("\033[34m\033[1mMidCxxAP-warning:\033[0m %s\n", text.c_str());
    }

    inline void __error(const std::string &text)
    {
        std::printf("\033[31m\033[1mMidCxxAP-error:\033[0m %s\n", text.c_str());
        std::exit(1);
    }

    class Argument
    {
    public:
        Argument(const std::string &posArgName);
        Argument(const std::string &shortArg, const std::string &longArg, const bool isRequired = false);

        void setAbsent(const bool isAbsent) { m_isAbsent = isAbsent; }

    public:
        std::string getArgName() const;

    public:
        std::string getShort() const { return m_short; }
        std::string getLong() const { return m_long; }
        bool getAbsent() const { return m_isAbsent; }

    private:
        union
        {
            bool m_isRequired;
            bool m_isAbsent;
        };
        bool m_isPositional;
        std::string m_short;
        std::string m_long;
    };

    class ArgumentParser
    {
    public:
        ArgumentParser(const std::string &progName = "", const std::string &progDesc = "");

        // TODO: Add a "dest" parameter.
        Argument &addArgument(const std::string &posArgName);
        Argument &addArgument(const std::string &shortArg, const std::string &longArg, const bool isRequired = false);

        std::unordered_map<std::string, std::string> &parseArgs(const int argc, char **const argv);
        Argument *findMatchingArg(const std::string &s, bool isShort);
        void tryToParseNonPositional(const std::string &currentArg, bool &parseNextArgument, Argument **parsingArg, bool isShort);

    public:
        std::string tryToSplitArg(const std::string &s, bool returnArg = false) const;
        void tryToPrintUsage(void) const;

    private:
        std::string m_progName;
        std::string m_progDesc;

        std::vector<Argument> m_posArgs;
        std::vector<Argument> m_stdArgs;

        std::string m_posArgsString;
        std::string m_stdArgsString;

        std::unordered_map<std::string, std::string> m_parsedArgs;
    };
}

#endif // __MIDCXXAP_HPP_INCLUDED